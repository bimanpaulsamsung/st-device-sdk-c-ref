#!/usr/bin/env python

import sys
import os
import re

BSP_NAME = sys.argv[1]
APP_NAME = sys.argv[2]
EXTRA_ARGS = sys.argv[3:]

APP_PATH = os.path.join(os.environ["STDK_REF_PATH"], "apps", BSP_NAME, APP_NAME)
EXECUTABLE_PATH = os.path.join(APP_PATH, "main", APP_NAME)
SDKCONFIG = os.path.join(APP_PATH, "sdkconfig")

if not (os.path.exists(APP_PATH)):
    print("Fail to find path.")
    exit(1)

if EXTRA_ARGS:
    MAKE_OPTION = ""
    for args in EXTRA_ARGS:
        MAKE_OPTION = MAKE_OPTION + " " + args
else:
    MAKE_OPTION = "all"

CFLAGS_CONFIG = ""
Read_File = open(SDKCONFIG, 'r')
for lines in Read_File:
    match = re.search("(\w+=\w+)", lines)
    if match:
        CFLAGS_CONFIG = CFLAGS_CONFIG + " -D" + match.group()
Read_File.close()
os.environ["CFLAGS_CONFIG"] = CFLAGS_CONFIG

build_cmd = "make" + " " + MAKE_OPTION
os.chdir(os.environ["STDK_CORE_PATH"])
os.system(build_cmd)

os.chdir(os.path.join(APP_PATH, "main"))
os.system(build_cmd)
