#!/usr/bin/env python

import platform
import sys
import os
import shutil
import subprocess

# Add your toolchain here
CROSS_COMPILE="/opt/gcc-arm-none-eabi-9-2019-q4-major"
if CROSS_COMPILE == "":
    print("Please set your toolchain in tools/xmc4800/build_xmc4800.py first")
    exit(1)
else:
    print("*NOTE*: Your toolchain path: "+ CROSS_COMPILE)
os.environ["CROSS_COMPILE"] = CROSS_COMPILE

CHIP_NAME = sys.argv[1]
os.environ["CHIP_NAME"] = CHIP_NAME

PROJECT = os.path.join(os.environ["STDK_REF_PATH"], "bsp", CHIP_NAME,"FreeRTOS/projects/infineon/xmc4800_iotkit/dave4/aws_demos")
os.environ["PROJECT"] = PROJECT

PROJECT_TITLE = sys.argv[2]
os.environ["PROJECT_TITLE"] = PROJECT_TITLE

STDK_PATH = os.environ["STDK_REF_PATH"]
os.environ["STDK_PATH"] = STDK_PATH

CORE_PATH = os.environ["STDK_CORE_PATH"]
os.environ["CORE_PATH"] = CORE_PATH

IOT_APPS_PATH = os.path.join(os.environ["STDK_REF_PATH"], "apps", CHIP_NAME)
IOT_OUTPUT_PATH = os.path.join(os.environ["STDK_REF_PATH"], "output", CHIP_NAME)

OUTPUT_BUILD= "y"

COMPONENT_PATH = os.path.join(os.environ["STDK_CORE_PATH"],"src")
os.environ["COMPONENT_PATH"] = COMPONENT_PATH

PROJECT_PATH = os.path.join(IOT_APPS_PATH, PROJECT_TITLE)
os.environ["PROJECT_PATH"] = PROJECT_PATH

def print_usage():
    print("    Usage: ./build.sh xmc4800 APP_NAME")
    print("- - - - - - - - - - - - - - - - - - -")
    print("    ex) ./build.sh xmc4800 switch_example")
    print("    ex) ./build.sh xmc4800 light_example")
    print()

if len(sys.argv) == 4 and sys.argv[3] == "clean":
    OUTPUT_BUILD = "n"

if PROJECT_TITLE == "":
    print_usage()
    exit(1)

if not os.path.isdir(PROJECT_PATH):
    print_usage()
    exit(1)

os.chdir(PROJECT_PATH + "/main")
os.system(CROSS_COMPILE + "/bin/arm-none-eabi-ld -r -b binary -o onboarding_config.o onboarding_config.json")
os.system(CROSS_COMPILE + "/bin/arm-none-eabi-ld -r -b binary -o device_info.o device_info.json")
os.system("mv onboarding_config.o device_info.o " + PROJECT + "/build/stdk/apps")
os.chdir(PROJECT + "/build")

# Build App
if len(sys.argv) == 4 and sys.argv[3] == "clean":
    os.system("make clean")
else:
    os.system("make all")

if OUTPUT_BUILD == "y":
    BINARY_PATH = PROJECT + "/build"
    OUTPUT_FILE_LIST = BINARY_PATH + "/stdk.hex"
    DEBUG_FILE_LIST  = BINARY_PATH + "/stdk.map"
    os.environ["OUTPUT_FILE_LIST"] = OUTPUT_FILE_LIST
    os.environ["DEBUG_FILE_LIST"]  = DEBUG_FILE_LIST
    os.system(STDK_PATH + "/tools/common/generate_output.py")
