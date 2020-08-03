#!/usr/bin/env python3
#-*- coding: utf-8 -*-

import os
import sys
import shutil

PYTHON_FILE_PATH = os.path.dirname(os.path.abspath(__file__))
STDK_REF_PATH = PYTHON_FILE_PATH + "/../../../"
CAPS_APPS_PATH = os.path.abspath(STDK_REF_PATH + "apps")
CAPS_FILE_PATH = os.path.abspath(STDK_REF_PATH + "apps/capability_sample")

if len(sys.argv) >= 2:
    CAPS_APPS_PATH = os.path.abspath(sys.argv[1])
if len(sys.argv) >= 3:
    CAPS_FILE_PATH = os.path.abspath(sys.argv[2])

for (path, dir, files) in os.walk(CAPS_APPS_PATH):
    for filename in files:
        if filename.endswith(("c", "h")) and filename.startswith("caps") and path.endswith("main"):
            try:
                shutil.copy(os.path.join(CAPS_FILE_PATH, filename), path)
            except:
                print("fail to copy " + os.path.join(CAPS_FILE_PATH, filename) + " to " + path)

print("\nDone!")
print("  usage : ./" +  os.path.basename( __file__ ) + "   CAPS_APPS_PATH(dst)   CAPS_FILE_PATH(src)")
print("  CAPS_APPS_PATH(dst) : " + CAPS_APPS_PATH)
print("  CAPS_FILE_PATH(src) : " + CAPS_FILE_PATH)
