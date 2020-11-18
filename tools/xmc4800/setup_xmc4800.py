#!/usr/bin/env python

import sys
import os

CHIP_NAME = sys.argv[1]

if not os.path.isdir("bsp/" + CHIP_NAME):
    os.mkdir("bsp/" + CHIP_NAME)

def bsp_setup():
    os.chdir("bsp/" + CHIP_NAME + "/FreeRTOS")
    os.system("git submodule update --init freertos_kernel")
    os.system("git submodule update --init libraries/3rdparty/pkcs11")
    os.system("git am ../../../patches/" + CHIP_NAME + "/*.patch")
    os.chdir("../../../iot-core")
    os.system("git submodule update --init src/deps/libsodium/libsodium")
    os.system("git submodule update --init src/deps/json/cJSON")
    os.chdir("../")

RET = os.system("git submodule status bsp/" + CHIP_NAME + " &> /dev/null")
if RET == "":
    os.system("git submodule update --init bsp/xmc4800/FreeRTOS")
    bsp_setup()
else:
    if not os.path.isdir("bsp/xmc4800/FreeRTOS"):
        print("Failed to find source code")
    else:
        print("Apply patches in existing repo")
        bsp_setup()
