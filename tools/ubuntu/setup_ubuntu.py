#!/usr/bin/env python

import sys
import os

BSP_NAME=sys.argv[1]
CORE_PATH = os.path.join(os.environ["STDK_CORE_PATH"])

os.chdir(CORE_PATH)
os.system("git submodule update --init --recursive")
os.system("git submodule foreach --recursive git reset --hard")
