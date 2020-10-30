#!/usr/bin/env python

import sys
import os

BSP_NAME=sys.argv[1]

os.chdir(os.environ["STDK_CORE_PATH"])
os.system("git submodule update --init --recursive")
os.system("git submodule foreach --recursive git reset --hard")
