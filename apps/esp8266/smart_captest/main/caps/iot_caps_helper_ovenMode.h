/* ***************************************************************************
 *
 * Copyright 2019-2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _IOT_CAPS_HELPER_OVEN_MODE_
#define _IOT_CAPS_HELPER_OVEN_MODE_

#include "iot_caps_helper.h"

#define CAP_ENUM_OVENMODE_SUPPORTEDOVENMODES_VALUE_MAX 26
enum {
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_HEATING,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_GRILL,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_WARMING,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_DEFROSTING,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_CONVENTIONAL,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_BAKE,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_BOTTOMHEAT,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_CONVECTIONBAKE,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_CONVECTIONROAST,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_BROIL,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_CONVECTIONBROIL,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_STEAMCOOK,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_STEAMBAKE,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_STEAMROAST,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_STEAMBOTTOMHEATPLUSCONVECTION,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_MICROWAVE,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_MWPLUSGRILL,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_MWPLUSCONVECTION,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_MWPLUSHOTBLAST,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_MWPLUSHOTBLAST2,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_SLIMMIDDLE,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_SLIMSTRONG,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_SLOWCOOK,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_PROOF,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_DEHYDRATE,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_OTHERS,
    CAP_ENUM_OVENMODE_OVENMODE_VALUE_MAX
};

const static struct iot_caps_ovenMode {
    const char *id;
    const struct ovenMode_attr_supportedOvenModes {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_OVENMODE_SUPPORTEDOVENMODES_VALUE_MAX];
    } attr_supportedOvenModes;
    const struct ovenMode_attr_ovenMode {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_OVENMODE_OVENMODE_VALUE_MAX];
    } attr_ovenMode;
    const struct ovenMode_cmd_setOvenMode { const char* name; } cmd_setOvenMode;
} caps_helper_ovenMode = {
    .id = "ovenMode",
    .attr_supportedOvenModes = {
        .name = "supportedOvenModes",
        .property = ATTR_SET_VALUE_ARRAY,
        .value_type = VALUE_TYPE_STRING,
        .values = {"heating", "grill", "warming", "defrosting", "Conventional", "Bake", "BottomHeat", "ConvectionBake", "ConvectionRoast", "Broil", "ConvectionBroil", "SteamCook", "SteamBake", "SteamRoast", "SteamBottomHeatplusConvection", "Microwave", "MWplusGrill", "MWplusConvection", "MWplusHotBlast", "MWplusHotBlast2", "SlimMiddle", "SlimStrong", "SlowCook", "Proof", "Dehydrate", "Others"},
    },
    .attr_ovenMode = {
        .name = "ovenMode",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"heating", "grill", "warming", "defrosting", "Conventional", "Bake", "BottomHeat", "ConvectionBake", "ConvectionRoast", "Broil", "ConvectionBroil", "SteamCook", "SteamBake", "SteamRoast", "SteamBottomHeatplusConvection", "Microwave", "MWplusGrill", "MWplusConvection", "MWplusHotBlast", "MWplusHotBlast2", "SlimMiddle", "SlimStrong", "SlowCook", "Proof", "Dehydrate", "Others"},
    },
    .cmd_setOvenMode = { .name = "setOvenMode" }, // arguments: mode(string) 
};

#endif /* _IOT_CAPS_HERLPER_OVEN_MODE_ */
