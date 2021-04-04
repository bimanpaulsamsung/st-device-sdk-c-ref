#!/usr/bin/env python

import platform
import sys
import os

BSP_NAME=sys.argv[1]

CORE_PATH = os.path.join(os.environ["STDK_CORE_PATH"])
BSP_PATH = os.path.join(os.environ["STDK_REF_PATH"],"bsp",BSP_NAME)
PATCH_PATH = os.path.join(os.environ["STDK_REF_PATH"], "patches", BSP_NAME)

# MBED_OS_TAG = "mbed-os-5.15.2"
# 
# def patch_apply():
#     if os.path.isdir(PATCH_PATH):
#     	os.chdir(os.path.join(BSP_PATH, "mbed-os"))
# 	os.system("git checkout " + MBED_OS_TAG)
#         for patchfile in sorted(os.listdir(PATCH_PATH)):
#             if patchfile.endswith(".patch"):
#                 os.system("git am " + os.path.join(PATCH_PATH, patchfile))
#     os.chdir(os.readlink(os.path.join(BSP_PATH, "iot-core")))
#     os.system("git submodule update --init src/deps/libsodium/libsodium")
#     os.system("git submodule update --init src/deps/json/cJSON")

def pre_setup():	
    print("Clone STM32CubeL4...")
    os.chdir(os.path.join(BSP_PATH, "stm32l4"))
    print(os.getcwd())
    os.system("git submodule update --init --recursive")
    os.system("git submodule foreach --recursive git reset --hard")

#    if cmd_status == 0:
#    	print("Apply mbed os Patches..")
# 	patch_apply()      	
#     else:
# 	if not os.path.exists(os.path.join(BSP_PATH, "mbed-os")):
# 		print("Failed to find source code in mbed-os..")
# 	else:
# 		print("Apply mbed os Patches in existing repo..")
# 		patch_apply()
# 
#     os.chdir(os.path.join(BSP_PATH))
#     print("Check source code in mbed-os")
   
def create_link():
	if not os.path.exists(os.path.join(BSP_PATH)):
		os.mkdir(os.path.join(BSP_PATH))	
	if os.path.islink(os.path.join(BSP_PATH, "iot-core")):
		os.remove(os.path.join(BSP_PATH, "iot-core"))
		os.symlink(os.path.join(CORE_PATH),os.path.join(BSP_PATH, "iot-core"))

create_link()
pre_setup()
