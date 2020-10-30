#!/usr/bin/env python

import sys
import os

BSP_NAME = sys.argv[1]
APP_NAME = sys.argv[2]
EXTRA_ARGS = sys.argv[3:]

APP_PATH = os.path.join(os.environ["STDK_REF_PATH"], "apps", BSP_NAME, APP_NAME)

if not (os.path.exists(APP_PATH)):
    print("Fail to find path.")
    exit(1)

if EXTRA_ARGS:
    MAKE_OPTION = ""
    for args in EXTRA_ARGS:
        MAKE_OPTION = MAKE_OPTION + " " + args
else:
    MAKE_OPTION = "all"

build_cmd = "make" + " " + MAKE_OPTION
os.chdir(os.environ["STDK_CORE_PATH"])
os.system(build_cmd)

os.chdir(os.path.join(APP_PATH, "main"))
os.system(build_cmd)