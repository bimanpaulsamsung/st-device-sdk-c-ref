#!/usr/bin/env python

import platform
import sys
import os

BSP_NAME=sys.argv[1]

CORE_PATH = os.path.join(os.environ["STDK_CORE_PATH"])
BSP_PATH = os.path.join(os.environ["STDK_REF_PATH"],"bsp",BSP_NAME)
PATCH_PATH = os.path.join(os.environ["STDK_REF_PATH"], "patches", BSP_NAME)
NTP_PATCH_PATH = os.path.join(os.environ["STDK_REF_PATH"], "patches", BSP_NAME, "ntp-client")
WIFI_PATCH_PATH = os.path.join(os.environ["STDK_REF_PATH"], "patches", BSP_NAME, "wifi")

MBED_OS_TAG = "mbed-os-5.15.2"

def patch_apply():
    if os.path.isdir(PATCH_PATH):
    	os.chdir(os.path.join(BSP_PATH, "mbed-os"))
	os.system("git checkout " + MBED_OS_TAG)
        for patchfile in sorted(os.listdir(PATCH_PATH)):
            if patchfile.endswith(".patch"):
                os.system("git am " + os.path.join(PATCH_PATH, patchfile))
    os.chdir(os.readlink(os.path.join(BSP_PATH, "iot-core")))
    os.system("git submodule update --init src/deps/libsodium/libsodium")
    os.system("git submodule update --init src/deps/json/cJSON")

def pre_setup():	
    print("Start Setup for mbed os..")
    os.chdir(os.path.join(BSP_PATH))
    cmd_status = os.system("mbed new . ")

    if cmd_status == 0:
    	print("Apply mbed os Patches..")
	patch_apply()      	
    else:
	if not os.path.exists(os.path.join(BSP_PATH, "mbed-os")):
		print("Failed to find source code in mbed-os..")
	else:
		print("Apply mbed os Patches in existing repo..")
		patch_apply()

    os.chdir(os.path.join(BSP_PATH))
    print("Check source code in mbed-os")
   
def create_link():
	if not os.path.exists(os.path.join(BSP_PATH)):
        	os.mkdir(os.path.join(BSP_PATH))	
	if os.path.islink(os.path.join(BSP_PATH, "iot-core")):
		os.remove(os.path.join(BSP_PATH, "iot-core"))
    	os.symlink(os.path.join(CORE_PATH),os.path.join(BSP_PATH, "iot-core"))

def ntp_setup():
    print("Start cloning ntp-client..")
    os.chdir(os.path.join(BSP_PATH))
    cmd_status = os.system("mbed add https://github.com/ARMmbed/ntp-client")

    if cmd_status == 0:
        print("Apply ntp Patches..")
        if os.path.isdir(NTP_PATCH_PATH):
            os.chdir(os.path.join(BSP_PATH, "ntp-client"))
            os.system("git checkout " + "master")
            for patchfile in sorted(os.listdir(NTP_PATCH_PATH)):
                if patchfile.endswith(".patch"):
                    os.system("git am " + os.path.join(NTP_PATCH_PATH, patchfile))
        else:
            print("Patch folder not present!!")
    else:
        print("Failed to clone ntp-client!!!")

def wifi_setup():
    print("Start cloning wifi library..")
    os.chdir(os.path.join(BSP_PATH))
    cmd_status = os.system("mbed add https://os.mbed.com/teams/ST/code/DISCO_L475VG_IOT01A_wifi")

    if cmd_status == 0:
        print("Apply wifi Patches..")
        if os.path.isdir(WIFI_PATCH_PATH):
            os.chdir(os.path.join(BSP_PATH, "DISCO_L475VG_IOT01A_wifi"))
            for patchfile in sorted(os.listdir(WIFI_PATCH_PATH)):
                if patchfile.endswith(".patch"):
                    os.system("hg import " + os.path.join(WIFI_PATCH_PATH, patchfile))
        else:
            print("Patch folder not present!!")
    else:
        print("Failed to clone wifi library!!!")

create_link()
pre_setup()
ntp_setup()
wifi_setup()
