#!/usr/bin/env python2
#-*- coding: utf-8 -*-

import re
import os
import shutil
import codecs
import sys

import yaml # need to install pyyaml: "pip install pyyaml"

NEED_TO_CHECK_VALID_LIST = True
NEED_TO_COPY_FILE = False
VALID_ID_LIST = []

ATTR_NO_INFO_OK_LIST = ["windowShadePreset", "windowShade", "videoCamera", "mediaTrackControl", "momentary", "statelessAudioMuteButton", "statelessPowerButton",
                        "statelessPowerToggleButton", "statelessAudioVolumeButton", "statelessCustomButton", "statelessChannelButton", "statelessTemperatureButton",
                        "statelessFanspeedButton", "refresh", "tone", "notification", "healthCheck", "audioVolume", "videoStream", "tvChannel", "ovenOperatingState",
                        "audioNotification", "mediaPresets", "videoCapture", ]
PARSE_FILE_PATH = os.path.dirname(os.path.abspath(__file__))
STDK_REF_PATH = PARSE_FILE_PATH + "/../../../"
STC_PATH = STDK_REF_PATH + "../SmartThingsCapabilities/"
YAML_PATH = STC_PATH + "capabilities/"
HELPER_OUTPUT_PATH = STDK_REF_PATH + "iot-core/src/include/caps/"
CAPS_OUTPUT_PATH = STDK_REF_PATH + "apps/capability_sample"
SAMPLE_C_PATH = os.path.dirname(os.path.abspath(__file__)) + "/sample/"

COPYRIGHT_FILE_PATH = PARSE_FILE_PATH + "/COPYRIGHT"
CAP_LIST_FILE_PATH =  PARSE_FILE_PATH + "/caps_id_list.txt"

ENUM_PREFIX = "CAP_ENUM_"

class variable_data:
    def __init__(self, name, type, init):
        self.name = name
        self.type = type
        self.init = init
    name = ""
    type = ""
    init = ""

class function_data:
    def __init__(self, name, type, argstring):
        self.name = name
        self.type = type
        self.argstring = argstring
    name = ""
    type = ""
    argstring = ""
    initcall = True


class caps_data:
    name = ""
    id = ""
    status = ""
    attr_list = []
    cmds_list = []

    variable_list = []
    function_list = []
    needJSON = False

class attr_enum_cmd_data:
    command = ""
    value = ""

class attr_data:
    name = ""
    datatype = ""
    jsonSchema = ""
    value = ""
    unit_name = ""
    unit_enum_list = []
    setter = ""
    enum_cmds_list = []
    act_cmds_list = []
    required = []
    property = ""
    data = ""

class cmds_data:
    name = ""
    arguments = []
    arg_num = 0
    isSetter = False
    isEnumCmd = False
    isActCmd = False
    target_attr_name = ""
    target_attr_value = ""
    target_attr_datatype = ""

class jsonSchema_data:
    name = ""
    title = ""
    type = ""
    enum_list = []

    min = ""
    max = ""
    maxlength = ""

    data = ""
    isArray = False
    isRequired = False

def get_c_type_from_jsonSchema_type(type):
    if type == "integer":
        type_string = "int "
    elif type == "number":
        type_string = "double "
    elif type == "string":
        type_string = "char *"
    elif type == "object":
        type_string = "JSON_H *"
    elif type == "boolean":
        type_string = "bool "
    else:
        print("ERROR: fail to convert type: " + type)
        type_string = "void *"
    return type_string

def get_property_string(attr):
    property_string = ""
    if (attr.jsonSchema.min):
        property_string = property_string + "ATTR_SET_VALUE_MIN | "
    if (attr.jsonSchema.max):
        property_string = property_string + "ATTR_SET_VALUE_MAX | "
    if ("value" in attr.required):
        property_string = property_string + "ATTR_SET_VALUE_REQUIRED | "
    if ("unit" in attr.required):
        property_string = property_string + "ATTR_SET_UNIT_REQUIRED | "
    if (attr.jsonSchema.maxlength):
        property_string = property_string + "ATTR_SET_MAX_LENGTH | "
    if (attr.jsonSchema.isArray):
        property_string = property_string + "ATTR_SET_VALUE_ARRAY | "
    property_string = property_string.rstrip("| ")
    if not (property_string):
        property_string = "NULL"
    property_string = property_string + ","
    return property_string

def get_unit_string(enum_unit, attr_name):
    # "%" -> "PERCENT"
    # "μg/m^3" -> "UG_PER_M3"
    # "斤" -> "CATTY"
    enum_unit = enum_unit.replace('%', "percent")
    enum_unit = enum_unit.replace('/', "_per_")
    enum_unit = enum_unit.replace('^', "")
    enum_unit = enum_unit.replace('μg', "ug")
    enum_unit = enum_unit.replace('斤', "catty")
    return enum_unit.replace(' ', '_')

def update_related_cmds_info(yaml_attr, attr, caps):
    if ("setter" in yaml_attr):
        attr.setter = yaml_attr["setter"]
    if ("enumCommands" in yaml_attr):
        yaml_enumCommands = yaml_attr["enumCommands"]
        enumlist = list()
        for enumCommands in yaml_enumCommands:
            cmd_item = attr_enum_cmd_data()
            cmd_item.command = enumCommands["command"]
            cmd_item.value = enumCommands["value"]
            enumlist.append(cmd_item)
        attr.enum_cmds_list = enumlist
    if ("actedOnBy" in yaml_attr):
        yaml_actedOnBy = yaml_attr["actedOnBy"]
        actlist = list()
        for actCmd in yaml_actedOnBy:
            actlist.append(actCmd)
        attr.act_cmds_list = actlist

def isValidCapability(caps):
    if not NEED_TO_CHECK_VALID_LIST: return True
    if not VALID_ID_LIST:
        with open(CAP_LIST_FILE_PATH, 'r') as f:
            for line in f: VALID_ID_LIST.append(line.rstrip("\n"))

    if caps.id in VALID_ID_LIST or caps.name in VALID_ID_LIST:
        if caps.id != "healthCheck":
            return True
    return False

def replace_sampleString_with_caps_attr_cmds(string, caps, attr, cmds):
    if (caps):
        string = string.replace("$CAPS_ID$", caps.id)
        string = string.replace("$CAPS_ID_UP$", caps.id.upper())
    if (attr):
        string = string.replace("$ATTR_NAME$", attr.name)
        string = string.replace("$ATTR_NAME_UP$", attr.name.upper())
    if (cmds):
        string = string.replace("$CMDS_NAME$", cmds.name)
        string = string.replace("$CMDS_NAME_UP$", cmds.name.upper())

    string = string.replace("$ENUM_PREFIX$", ENUM_PREFIX)
    return string

def get_caps_data_arg_str(caps):
    return "struct caps_" + caps.id + "_data *caps_data"

def preproc_caps(caps):
    caps.variable_list = []
    caps.function_list = []

    for attr in caps.attr_list:
    # exception:
        # if there is no MIN value, forcely set min value for init value.
        if (attr.jsonSchema.type in ["interger", "number"] and not attr.jsonSchema.min):
            print("WARN : no MIN value! //caps.id : " + caps.id + ", attr.name : " + attr.name)
            attr.jsonSchema.min = "0"

        # if datatpye is array of JSON_OBJECT, manage as JSON_OBJECT with array inside.
        if (attr.datatype == "JSON_OBJECT" and attr.jsonSchema.type != "string" and attr.jsonSchema.isArray):
            attr.jsonSchema.isArray = False

        #other esceptions.
        if caps.id == "objectDetection" and attr.name == "supportedValues": # typo?
            attr.datatype = "JSON_OBJECT"
        elif (caps.id == "elevatorCall"): # "call" command looks like enumCommand but it is not setted.
            enum_item = attr_enum_cmd_data()
            enum_item.command = "call"
            enum_item.value = "called"
            attr.enum_cmds_list.append(enum_item)
        elif (caps.id == "fanOscillationMode"): # it looks like that setter is not setted
            attr.setter = "setFanOscillationMode"


    for attr in caps.attr_list:
        if (attr.setter):
            for cmds in caps.cmds_list:
                if (cmds.name == attr.setter) :
                    cmds.isSetter = True
                    cmds.target_attr_name = attr.name
                    cmds.target_attr_datatype = attr.datatype
        for actCmds in attr.act_cmds_list:
            for cmds in caps.cmds_list:
                if (cmds.name == actCmds):
                    cmds.isActCmd = True
        for enumCmds in attr.enum_cmds_list:
            for cmds in caps.cmds_list:
                if (cmds.name == enumCmds.command):
                    cmds.isEnumCmd = True
                    cmds.target_attr_name = attr.name
                    cmds.target_attr_value = enumCmds.value
                    cmds.target_attr_datatype = attr.datatype

        if attr.datatype in ["JSON_OBJECT", "VECTOR3", "COLOR_MAP"] or caps.id in ["colorControl"]:
            caps.needJSON = True

# get variable and function  list
        if (caps.id == "colorControl" and attr.name == "color"):
            caps.function_list.append(function_data("set_" + attr.name + "_value", "void ", get_caps_data_arg_str(caps) + ", double hue, double saturaiton"))
            caps.function_list.append(function_data("attr_" + attr.name + "_send", "void ", get_caps_data_arg_str(caps)))
        elif (attr.datatype == "VECTOR3"):
            caps.variable_list.append(variable_data(attr.name+"_value", "JSON_H *", None))
            caps.function_list.append(function_data("get_" + attr.name + "_value", "const JSON_H *", get_caps_data_arg_str(caps)))
            caps.function_list.append(function_data("set_" + attr.name + "_value", "void ", get_caps_data_arg_str(caps) + ", int x, int y, int z"))
            caps.function_list.append(function_data("attr_" + attr.name + "_send", "void ", get_caps_data_arg_str(caps)))
        else:
            type_string = get_c_type_from_jsonSchema_type(attr.jsonSchema.type)
            array_arg_string = ""
            init_string = ""
            if (attr.jsonSchema.isArray):
                type_string = type_string + "*"
                array_arg_string = ", int arraySize"
            if (attr.jsonSchema.min):
                init_string = attr.jsonSchema.min
            caps.variable_list.append(variable_data(attr.name + "_value", type_string, init_string))
            if (attr.jsonSchema.isArray):
                caps.variable_list.append(variable_data(attr.name + "_arraySize", "int ", None))

            if (type_string.endswith("*")):
                type_string = "const " + type_string
            caps.function_list.append(function_data("get_" + attr.name + "_value", type_string, get_caps_data_arg_str(caps)))
            caps.function_list.append(function_data("set_" + attr.name + "_value", "void ", get_caps_data_arg_str(caps) + ", " + type_string + "value" + array_arg_string))
            if (attr.datatype == "ENUM"):
                caps.function_list.append(function_data("attr_" + attr.name + "_str2idx", "int ", "const char *value"))

            if (attr.unit_enum_list):
                caps.variable_list.append(variable_data(attr.name + "_unit", "char *", None))
                caps.function_list.append(function_data("get_" + attr.name + "_unit", "const char *", get_caps_data_arg_str(caps)))
                caps.function_list.append(function_data("set_" + attr.name + "_unit", "void ", get_caps_data_arg_str(caps) + ", const char *unit"))
            caps.function_list.append(function_data("attr_" + attr.name + "_send", "void ", get_caps_data_arg_str(caps)))

def parse_jsonSchema(yaml_jsonSchema, jsonSchema_name):
    jsonSchema = jsonSchema_data()
    jsonSchema.name = jsonSchema_name

    if ("$ref" in yaml_jsonSchema):
        jsonSchema.title = yaml_jsonSchema["$ref"]
        data_path = STC_PATH + "datatypes/" + jsonSchema.title + ".yml"
        yaml_jsonSchema = yaml.load(codecs.open(data_path, "r", "utf-8"), Loader=yaml.FullLoader)
    elif ("items" in yaml_jsonSchema):
        jsonSchema.isArray = True
        yaml_jsonSchema = yaml_jsonSchema["items"]
        if ("$ref" in yaml_jsonSchema):
            jsonSchema.title = yaml_jsonSchema["$ref"]
            data_path = STC_PATH + "datatypes/" + jsonSchema.title + ".yml"
            yaml_jsonSchema = yaml.load(codecs.open(data_path, "r", "utf-8"), Loader=yaml.FullLoader)

    if ("type" in yaml_jsonSchema):
        jsonSchema.type = yaml_jsonSchema["type"]
    else:
        print("ERROR: PARSE FAIL ///" + caps.id + "Fail to find type of " + jsonSchema.name + ", title: "+ jsonSchema.title)

    if ("title" in yaml_jsonSchema):
        jsonSchema.title = yaml_jsonSchema["title"]

    if ("enum" in yaml_jsonSchema):
        jsonSchema.enum_list = yaml_jsonSchema["enum"]
    if ("minimum" in yaml_jsonSchema):
        jsonSchema.min = str(yaml_jsonSchema["minimum"])
    if ("maximum" in yaml_jsonSchema):
        jsonSchema.max = str(yaml_jsonSchema["maximum"])
    if ("maxLength" in yaml_jsonSchema):
        jsonSchema.maxlength = str(yaml_jsonSchema["maxLength"])

    return jsonSchema

def parse_yaml(yaml_path):
    caps = caps_data()

    caps_yaml = yaml.load(codecs.open(yaml_path, "r", "utf-8"), Loader=yaml.FullLoader)

    caps.name = caps_yaml["name"]
    caps.status = caps_yaml["status"]
    caps.id = caps_yaml["id"]

    if not isValidCapability(caps):
        return None

    yaml_attr_list = caps_yaml["attributes"]
    yaml_cmds_list = caps_yaml["commands"]

    attr_list = list()
    for value in yaml_attr_list:
        attr_item = parse_attr(yaml_attr_list[value], value, caps)
        attr_list.append(attr_item)
    caps.attr_list = attr_list

    cmds_list = list()
    for value in yaml_cmds_list:
        cmds_item = parse_cmds(yaml_cmds_list[value], value, caps)
        cmds_list.append(cmds_item)
    caps.cmds_list = cmds_list

    return caps

def parse_attr(yaml_attr, attr_name, caps):
    attr = attr_data()
    attr.name = attr_name

    attr.datatype = yaml_attr["type"]

    update_related_cmds_info(yaml_attr, attr, caps)

    yaml_schema = yaml_attr["schema"]
    if ("$ref" in yaml_schema):
        schema_path = STC_PATH + "datatypes/" + yaml_schema["$ref"] + ".yml"
        yaml_schema = yaml.load(codecs.open(schema_path, "r", "utf-8"), Loader=yaml.FullLoader)

    if ("required" in yaml_schema):
        attr.required = yaml_schema["required"]

    yaml_properties = yaml_schema["properties"]

    yaml_value = yaml_properties["value"]
    attr.jsonSchema = parse_jsonSchema(yaml_value, "")

    if ("unit" in yaml_properties):
        yaml_unit = yaml_properties["unit"]
        if ("$ref" in yaml_unit):
            attr.unit_name = yaml_unit["$ref"]
            unit_data_path = STC_PATH + "datatypes/" + attr.unit_name + ".yml"
            yaml_unit = yaml.load(codecs.open(unit_data_path, "r", "utf-8"), Loader=yaml.FullLoader)

        if ("enum" in yaml_unit):
            attr.unit_enum_list = yaml_unit["enum"]

    if ("data" in yaml_properties):
        print("WARN: attr has 'data' // caps.id : " + caps.id + ", attr.name : " +attr.name)
        attr.data = "data"

    return attr

def parse_cmds(yaml_cmds, cmds_name, caps):
    cmds = cmds_data()
    cmds.name = cmds_name

    yaml_args = yaml_cmds["arguments"]

    arg_list = list()
    for yaml_arg in yaml_args:
        cmds.arg_num = cmds.arg_num + 1
        arg_item = parse_jsonSchema(yaml_arg["schema"], "")

        arg_item.name = yaml_arg["name"]
        arg_item.isRequired = yaml_arg["required"]
        arg_list.append(arg_item)
    cmds.arguments = arg_list

    return cmds

def make_helper_c_header_output(caps):
    isData = False

    if not (os.path.exists(HELPER_OUTPUT_PATH)):
        os.makedirs(HELPER_OUTPUT_PATH)

    output_name = HELPER_OUTPUT_PATH + "/iot_caps_helper_" + caps.id + ".h"

    shutil.copy(COPYRIGHT_FILE_PATH, output_name)
    output = open(output_name,'a')

    #exception check
    sample_path = SAMPLE_C_PATH + "/exception_" + caps.id + "_helperCheader.h"
    if (os.path.isfile(sample_path)):
        output.write(replace_sampleString_with_caps_attr_cmds(open(sample_path, 'r').read(), caps, None, None))
        return

    output.write(            "#ifndef _IOT_CAPS_HELPER_" + caps.name.replace(' ', '_').upper() + "_\n")
    output.write(            "#define _IOT_CAPS_HELPER_" + caps.name.replace(' ', '_').upper() + "_\n")
    output.write(            "\n")
    output.write(            "#include \"iot_caps_helper.h\"\n")
    output.write(            "\n")
    output.write(            "#ifdef __cplusplus\n")
    output.write(            "extern \"C\" {\n")
    output.write(            "#endif\n")
    output.write(            "\n")
# HELPER C HEADER : ENUM for VALUE
    already_check_value_type = []
    for attr in caps.attr_list:
        if attr.jsonSchema.enum_list:
            if attr.name[:len("supported")] == "supported":
                output.write("#define " + (ENUM_PREFIX + caps.id + "_" + attr.name + "_VALUE_MAX ").upper() + str(len(attr.jsonSchema.enum_list)) + "\n")
                continue
            if attr.jsonSchema.title and attr.jsonSchema.title in already_check_value_type:
                continue
            already_check_value_type.append(attr.jsonSchema.title)

            output.write(    "enum {\n")
            for enum_value in attr.jsonSchema.enum_list:
                output.write("    " + (ENUM_PREFIX + caps.id + "_" + attr.name + "_VALUE_" + enum_value.replace(' ', '_')).upper() + ",\n")
            output.write(    "    " + (ENUM_PREFIX + caps.id + "_" + attr.name + "_VALUE_MAX").upper() + "\n")
            output.write(    "};\n\n")

# HELPER C HEADER : ENUM for UNIT
    already_check_unit_type = []
    for attr in caps.attr_list:
        if attr.unit_enum_list:
            if attr.unit_name and attr.unit_name in already_check_unit_type:
                continue
            already_check_unit_type.append(attr.jsonSchema.title)

            output.write(    "enum {\n")
            for enum_unit in attr.unit_enum_list:
                enum_unit = enum_unit.encode('utf8')
                output.write("    " + (ENUM_PREFIX + caps.id + "_" + attr.name + "_UNIT_" + get_unit_string(enum_unit, attr.name)).upper() + ",\n")
            output.write(    "    " + (ENUM_PREFIX + caps.id + "_" + attr.name + "_UNIT_MAX").upper() + "\n")
            output.write(    "};\n")
            output.write(    "\n")


# HELPER C HEADER : DEFINITION CAPS
    output.write(            "const static struct iot_caps_"+caps.id+" {\n")
    output.write(            "    const char *id;\n")

    for attr in caps.attr_list:
        output.write(        "    const struct "+caps.id+"_attr_"+attr.name+" {\n")
        output.write(        "        const char *name;\n")
        output.write(        "        const unsigned char property;\n")
        output.write(        "        const unsigned char valueType;\n")
        if (attr.jsonSchema.enum_list):
            output.write(    "        const char *values[" + (ENUM_PREFIX + caps.id + "_" + attr.name + "_VALUE_MAX").upper() + "];\n")
            for enum_value in attr.jsonSchema.enum_list:
                output.write("        const char *value_" + enum_value.replace(' ', '_') + ";\n")
        if (attr.unit_enum_list):
            output.write(    "        const char *units[" + (ENUM_PREFIX + caps.id + "_" + attr.name + "_UNIT_MAX").upper() + "];\n")
            for enum_unit in attr.unit_enum_list:
                output.write("        const char *unit_" + get_unit_string(enum_unit.encode('utf8'), attr.name) + ";\n")

        value_type_string = get_c_type_from_jsonSchema_type(attr.jsonSchema.type)
        if (attr.jsonSchema.min):
            output.write(    "        const " + value_type_string + "min;\n")
        if (attr.jsonSchema.max):
            output.write(    "        const " + value_type_string + "max;\n")
        if (attr.jsonSchema.maxlength):
            output.write(    "        const unsigned int max_length;\n")
        if (attr.data):
            isData = True
        output.write(        "    } attr_"+attr.name+";\n")

    for cmds in caps.cmds_list:
        output.write(        "    const struct "+caps.id+"_cmd_"+cmds.name+" { const char* name; } cmd_"+cmds.name+";\n")

    output.write(            "} caps_helper_" + caps.id +" = {\n")

# HELPER C HEADER : INITIALIZE
    output.write(            "    .id = \""+caps.id+"\",\n")

    for attr in caps.attr_list:
        output.write(        "    .attr_"+attr.name+" = {\n")
        output.write(        "        .name = \"" + attr.name + "\",\n")

        property_string = get_property_string(attr)
        output.write(        "        .property = " + property_string + "\n")

        if not (attr.jsonSchema.type):
            output.write(    "        .valueType = NULL,\n")
        else:
            output.write(    "        .valueType = VALUE_TYPE_" + attr.jsonSchema.type.upper() + ",\n")

        if (attr.jsonSchema.enum_list):
            enum_string = ""
            for enum_value in attr.jsonSchema.enum_list:
                enum_string = enum_string + "\""+enum_value+"\", "
            output.write(    "        .values = {" + enum_string.rstrip(" ,") + "},\n")
            for enum_value in attr.jsonSchema.enum_list:
                output.write("        .value_" + enum_value.replace(' ', '_') + " = \"" + enum_value + "\",\n")


        if (attr.unit_enum_list):
            enum_string = ""
            for enum_unit in attr.unit_enum_list:
                enum_unit = enum_unit.encode('utf8')
                enum_string = enum_string + "\""+enum_unit+"\", "
            output.write(    "        .units = {" + enum_string.rstrip(" ,") + "},\n")
            for enum_unit in attr.unit_enum_list:
                output.write("        .unit_" + get_unit_string(enum_unit.encode('utf8'), attr.name) + " = \"" + enum_unit.encode('utf8') + "\",\n")

        if (attr.jsonSchema.min):
            output.write(    "        .min = " + attr.jsonSchema.min + ",\n")
        if (attr.jsonSchema.max):
            output.write(    "        .max = " + attr.jsonSchema.max + ",\n")
        if (attr.jsonSchema.maxlength):
            output.write(    "        .max_length = " + attr.jsonSchema.maxlength + ",\n")
        output.write(        "    },\n")

    for cmds in caps.cmds_list:
        output.write(        "    .cmd_"+cmds.name+" = { .name = \""+cmds.name+"\" },")

        if (cmds.arguments):
            output.write(" // arguments: ")
            for arg in cmds.arguments:
                output.write(arg.name + "(" + arg.type + ") ")

        output.write("\n")

    output.write(            "};\n")
    output.write(            "\n")
    output.write(            "#ifdef __cplusplus\n")
    output.write(            "}\n")
    output.write(            "#endif\n")
    output.write(            "\n")
    output.write(            "#endif /* _IOT_CAPS_HERLPER_"+caps.name.replace(' ', '_').upper()+"_ */\n")

    output.close()

    if NEED_TO_COPY_FILE:
        if not (os.path.exists(HELPER_OUTPUT_PATH + caps.status)):
            os.makedirs(HELPER_OUTPUT_PATH + caps.status)
        shutil.copy(output_name, HELPER_OUTPUT_PATH + caps.status)

        if (isData):
            if not (os.path.exists(HELPER_OUTPUT_PATH + "data")):
                os.makedirs(HELPER_OUTPUT_PATH + "data")
            shutil.copy(output_name, HELPER_OUTPUT_PATH + "data")


def make_caps_c_header_output(caps):
    isData = False

    if not (os.path.exists(CAPS_OUTPUT_PATH)):
        os.makedirs(CAPS_OUTPUT_PATH)

    output_name = CAPS_OUTPUT_PATH + "/caps_" + caps.id + ".h"

    shutil.copy(COPYRIGHT_FILE_PATH, output_name)
    output = open(output_name,'a')

    #exception check
    sample_path = SAMPLE_C_PATH + "/exception_" + caps.id + "_capsCheader.h"
    if (os.path.isfile(sample_path)):
        output.write(replace_sampleString_with_caps_attr_cmds(open(sample_path, 'r').read(), caps, attr, None))
        return

    output.write(            "#include \"caps/iot_caps_helper_" + caps.id + ".h\"\n")
    if (caps.needJSON):
        output.write(        "#include \"JSON.h\"\n")
    output.write(            "\n")
    output.write(            "#ifdef __cplusplus\n")
    output.write(            "extern \"C\" {\n")
    output.write(            "#endif\n")
    output.write(            "\n")
    output.write(            "typedef struct caps_" + caps.id + "_data {\n")
    output.write(            "    IOT_CAP_HANDLE* handle;\n")
    output.write(            "    void *usr_data;\n")
    output.write(            "    void *cmd_data;\n")
    output.write(            "\n")

    for variable in caps.variable_list:
        output.write(        "    " + variable.type + variable.name + ";\n")
    output.write(            "\n")

    for function in caps.function_list:
        output.write(        "    " + function.type + "(*" + function.name + ")(" + function.argstring + ");\n")
    output.write(            "\n")

    output.write(            "    void (*init_usr_cb)(" + get_caps_data_arg_str(caps) + ");\n")
    if caps.cmds_list:
        output.write(        "\n")

    for cmds in caps.cmds_list:
        output.write(        "    void (*cmd_" + cmds.name + "_usr_cb)(" + get_caps_data_arg_str(caps) + ");\n")
    output.write(            "} caps_" + caps.id + "_data_t;\n")
    output.write(            "\n")
    output.write(            "caps_" + caps.id + "_data_t *caps_" + caps.id + "_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);\n")
    output.write(            "#ifdef __cplusplus\n")
    output.write(            "}\n")
    output.write(            "#endif\n")
    output.write(            "\n")
    output.close()

    if NEED_TO_COPY_FILE:
        if not (os.path.exists(CAPS_OUTPUT_PATH + caps.status)):
            os.makedirs(CAPS_OUTPUT_PATH + caps.status)
        shutil.copy(output_name, CAPS_OUTPUT_PATH + caps.status)

        if (isData):
            if not (os.path.exists(CAPS_OUTPUT_PATH + "data")):
                os.makedirs(CAPS_OUTPUT_PATH + "data")
            shutil.copy(output_name, CAPS_OUTPUT_PATH + "data")



def make_caps_c_file_output(caps):
    isData = False

    if not (os.path.exists(CAPS_OUTPUT_PATH)):
        os.makedirs(CAPS_OUTPUT_PATH)

    output_name = CAPS_OUTPUT_PATH + "/caps_" + caps.id + ".c"

    shutil.copy(COPYRIGHT_FILE_PATH, output_name)
    output = open(output_name,'a')

    sample_path = SAMPLE_C_PATH + "/exception_" + caps.id + "_capsCsrc.c"
    if (os.path.isfile(sample_path)):
        output.write(replace_sampleString_with_caps_attr_cmds(open(sample_path, 'r').read(), caps, None, None))
        return

    output.write(            "#include <string.h>\n")
    output.write(            "#include <stdio.h>\n")
    output.write(            "#include <stdlib.h>\n")
    output.write(            "\n")
    output.write(            "#include \"st_dev.h\"\n")
    output.write(            "#include \"caps_" + caps.id + ".h\"\n")
    output.write(            "\n")

    for attr in caps.attr_list:
        # capsCsrc_cmd : exception check

        sample_path = SAMPLE_C_PATH + "/exception_" + caps.id + "_capsCsrc_attr_" + attr.name + ".c"
        if (os.path.isfile(sample_path)):
            output.write(replace_sampleString_with_caps_attr_cmds(open(sample_path, 'r').read(), caps, attr, None) + "\n")
            continue
        # CAPS C FILE : ATTR_FUNC : use sample files
        elif (attr.datatype == "NUMBER"):
            if (attr.unit_enum_list): arrayString = "_with_unit"
            else: arrayString = ""
            sample_path = SAMPLE_C_PATH + "/attrFunc_NUMBER_" + attr.jsonSchema.type + arrayString + ".c"
        elif (attr.jsonSchema.type == "string" and attr.jsonSchema.isArray):
            sample_path = SAMPLE_C_PATH + "/attrFunc_JSON_OBJECT_string_array.c"
        elif (attr.datatype  == "JSON_OBJECT"):
            if (attr.jsonSchema.type == "object"):
                sample_path = SAMPLE_C_PATH + "/attrFunc_JSON_OBJECT_object.c"
            else:
                print("ERROR : fail to find sample path for JSON_OBJECT!  //caps.id : " + caps.id + "attr.name : "+attr.name+", attr.datatype : " + attr.datatype + ", schematype : " + attr.jsonSchema.type + ", array? : " + str(attr.jsonSchema.isArray))
                sample_path = ""
        elif (attr.datatype in ("DATE", "DYNAMIC_ENUM")):
            sample_path = SAMPLE_C_PATH + "/attrFunc_STRING.c"
        elif (attr.datatype in ("STRING", "ENUM", "COLOR_MAP", "VECTOR3")):
            sample_path = SAMPLE_C_PATH + "/attrFunc_"+attr.datatype+".c"
        else:
            print("ERROR : fail to find sample path  //caps.id : " + caps.id + "attr.name : "+attr.name+", attr.datatype : " + attr.datatype + ", schematype : " + attr.jsonSchema.type + ", array? : " + str(attr.jsonSchema.isArray))
            sample_path = ""

        if (os.path.isfile(sample_path)):
            output.write(replace_sampleString_with_caps_attr_cmds(open(sample_path, 'r').read(), caps, attr, None) + "\n")
            output.write("\n")


    for cmds in caps.cmds_list:
# capsCsrc_cmd : exception check
        sample_path = SAMPLE_C_PATH + "/exception_" + caps.id + "_capsCsrc_cmd_" + cmds.name + ".c"
        if (os.path.isfile(sample_path)):
            output.write(replace_sampleString_with_caps_attr_cmds(open(sample_path, 'r').read(), caps, None, cmds) + "\n")
            continue

        if (cmds.arg_num >= 2):
            print("WARN : arg num is " + str(cmds.arg_num) + " // caps.id : " + caps.id + ", cmds.name : " + cmds.name)

        #for arg_num 0 or 1
        output.write(        "static void caps_"+caps.id+"_cmd_"+cmds.name+"_cb(IOT_CAP_HANDLE *handle, iot_cap_cmd_data_t *cmd_data, void *usr_data)\n")
        output.write(        "{\n")
        output.write(        "    caps_"+caps.id+"_data_t *caps_data = (caps_"+caps.id+"_data_t *)usr_data;\n")
        cmd_arg_type = ""
        if (cmds.isEnumCmd) :
            output.write(    "    const char* value = caps_helper_"+caps.id+".attr_"+cmds.target_attr_name+".values[" + (ENUM_PREFIX + caps.id+"_"+cmds.target_attr_name+"_VALUE_"+cmds.target_attr_value.replace(" ","_")).upper()+"];\n")
        for arg in cmds.arguments:
            cmd_arg_type = arg.type
            if (cmds.isSetter):
                output.write(    "    " + get_c_type_from_jsonSchema_type(cmd_arg_type) + "value;\n")
            break;
        if ((cmd_arg_type == "string") and (cmds.isSetter) and cmds.target_attr_datatype == "ENUM"):
            output.write(    "    int index;\n")

        output.write(        "\n")
        output.write(        "    printf(\"called [%s] func with num_args:%u\\n\", __func__, cmd_data->num_args);\n")
        output.write(        "\n")

        if (cmds.isSetter):
            if (cmd_arg_type == "string" and cmds.target_attr_datatype == "ENUM"):
                output.write("    index = caps_"+caps.id+"_attr_"+cmds.target_attr_name+"_str2idx(cmd_data->cmd_data[0].string);\n")
                output.write("    if (index < 0) {\n")
                output.write("        printf(\"%s is not supported value for "+cmds.name+"\\n\", cmd_data->cmd_data[0].string);\n")
                output.write("        return;\n")
                output.write("    }\n")
                output.write("    value = (char *)caps_helper_"+caps.id+".attr_"+cmds.target_attr_name+".values[index];\n")
            else:
                output.write("    value = cmd_data->cmd_data[0]."+cmd_arg_type+";\n")
            output.write(    "\n")

        if (cmds.isSetter or cmds.isEnumCmd):
            output.write(    "    "+"caps_"+caps.id+"_set_"+cmds.target_attr_name+"_value(caps_data, value);\n")

        output.write(        "    if (caps_data && caps_data->cmd_"+cmds.name+"_usr_cb)\n")
        output.write(        "        caps_data->cmd_"+cmds.name+"_usr_cb(caps_data);\n")
        if (cmds.target_attr_name) :
            output.write(    "    caps_"+caps.id+"_attr_"+cmds.target_attr_name+"_send(caps_data);\n")
        output.write(        "}\n")
        output.write(        "\n")

        if not (cmds.isSetter or cmds.isEnumCmd):
            if caps.id not in ATTR_NO_INFO_OK_LIST:
                print("WARN : no info about attr //caps.id : " + caps.id + ", cmds.name: " + cmds.name)

#CAPS C FILE : INIT_CB
    output.write(            "static void caps_"+caps.id+"_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)\n")
    output.write(            "{\n")
    output.write(            "    caps_"+caps.id+"_data_t *caps_data = usr_data;\n")
    output.write(            "    if (caps_data && caps_data->init_usr_cb)\n")
    output.write(            "        caps_data->init_usr_cb(caps_data);\n")
    for attr in caps.attr_list:
        output.write(        "    caps_"+caps.id+"_attr_"+attr.name+"_send(caps_data);\n")
    output.write(            "}\n")
    output.write(            "\n")
#CAPS C FILE : INITIALIZE FUNC
    output.write(            "caps_"+caps.id+"_data_t *caps_"+caps.id+"_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)\n")
    output.write(            "{\n")
    output.write(            "    caps_"+caps.id+"_data_t *caps_data = NULL;\n")
    if (caps.cmds_list):
        output.write(        "    int err;\n")
    output.write(            "\n")
    output.write(            "    caps_data = malloc(sizeof(caps_"+caps.id+"_data_t));\n")
    output.write(            "    if (!caps_data) {\n")
    output.write(            "        printf(\"fail to malloc for caps_"+caps.id+"_data\\n\");\n")
    output.write(            "        return NULL;\n")
    output.write(            "    }\n")
    output.write(            "\n")
    output.write(            "    memset(caps_data, 0, sizeof(caps_"+caps.id+"_data_t));\n")
    output.write(            "\n")
    output.write(            "    caps_data->init_usr_cb = init_usr_cb;\n")
    output.write(            "    caps_data->usr_data = usr_data;\n")
    output.write(            "\n")

    for function in caps.function_list:
        output.write(        "    caps_data->" + function.name + " = caps_" + caps.id + "_" + function.name + ";\n")


    for variable in caps.variable_list:
        if (variable.init):
            output.write(        "    caps_data->" + variable.name + " = ");
            if (variable.type.endswith("*")): output.write("("+ variable.type+")")
            output.write(variable.init + ";\n")

    output.write(            "    if (ctx) {\n")
    output.write(            "        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_" + caps.id + ".id, caps_" + caps.id + "_init_cb, caps_data);\n")
    output.write(            "    }\n")
    if (caps.cmds_list):
        output.write(        "    if (caps_data->handle) {\n")
        for cmds in caps.cmds_list:
            output.write(    "        err = st_cap_cmd_set_cb(caps_data->handle, caps_helper_"+caps.id+".cmd_"+cmds.name+".name, caps_"+caps.id+"_cmd_"+cmds.name+"_cb, caps_data);\n")
            output.write(    "        if (err) {\n")
            output.write(    "            printf(\"fail to set cmd_cb for " + cmds.name + " of " + caps.id + "\\n\");\n")
            output.write(    "    }\n")
        output.write(        "    } else {\n")
    else:
        output.write(        "    if (!caps_data->handle) {\n")
    output.write(            "        printf(\"fail to init "+caps.id+" handle\\n\");\n")
    output.write(            "    }\n")
    output.write(            "\n")
    output.write(            "    return caps_data;\n")
    output.write(            "}\n")
    output.close()

    if NEED_TO_COPY_FILE:
        if not (os.path.exists(CAPS_OUTPUT_PATH + caps.status)):
            os.makedirs(CAPS_OUTPUT_PATH + caps.status)
        shutil.copy(output_name, CAPS_OUTPUT_PATH + caps.status)

        if (isData):
            if not (os.path.exists(CAPS_OUTPUT_PATH + "data")):
                os.makedirs(CAPS_OUTPUT_PATH + "data")
            shutil.copy(output_name, CAPS_OUTPUT_PATH + "data")



path_list = [
#if you want to add yml path, write it on this list:
#STC_PATH + "capabilities/acceleration-sensor.src/additional_capability.yml",
]

dir_list = os.listdir(YAML_PATH)
for dir_file in dir_list:
    # add "YAML_PATH/*.yml" file
    if (dir_file.endswith(".yml")):
        path = YAML_PATH + "/" + dir_file
        path_list.append(path)

    # add "YAML_PATH/*/*.yml"
    elif os.path.isdir(YAML_PATH + "/" + dir_file):
        file_list = os.listdir(YAML_PATH + "/" + dir_file)
        for file in file_list:
            if (file.endswith(".yml")):
                path = YAML_PATH + "/" + dir_file + "/" + file
                path_list.append(path)

for path in path_list:
    caps = parse_yaml(path)
    if (caps):
        preproc_caps(caps)
        make_helper_c_header_output(caps)
        make_caps_c_header_output(caps)
        make_caps_c_file_output(caps)
