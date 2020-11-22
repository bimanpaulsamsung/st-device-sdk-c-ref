#!/usr/bin/env python

import platform
import sys
import os
import shutil

CA_CERT_FILE = "root_ca_prod_k8.pem"

BSP_NAME = sys.argv[1]
APP_NAME = sys.argv[2]
EXTRA_ARGS = sys.argv[3:]

BSP_PATH = os.path.join(os.environ["STDK_REF_PATH"],"bsp", BSP_NAME)
IOT_APPS_PATH = os.path.join(os.environ["STDK_REF_PATH"], "apps", BSP_NAME, APP_NAME)
CORE_PATH = os.path.join(os.environ["STDK_REF_PATH"],"iot-core")
STDK_PATH = os.path.join(os.environ["STDK_REF_PATH"])

if not (os.path.exists(BSP_PATH) and os.path.exists(IOT_APPS_PATH)):
    print("Fail to find path.")
    exit(1)

if not (os.path.exists(os.path.join(BSP_PATH, "apps"))):
    os.mkdir(os.path.join(BSP_PATH, "apps"))
    os.symlink(os.path.join(IOT_APPS_PATH),os.path.join(BSP_PATH, "apps", APP_NAME))

FLASH_OPTION = ""
CONFIG_OPTION = ""
if EXTRA_ARGS:
    for args in EXTRA_ARGS:
        if args == "config":
            CONFIG_OPTION=True
            continue
        FLASH_OPTION = FLASH_OPTION + " --" + args

#Generate Configuration in Json formate
CONFIG_FILENAME=os.path.join(os.environ["STDK_REF_PATH"], ".config")
KCONFIG_CMD="kconfig-conf"

CMD_RETVAL = os.system("command -v " + KCONFIG_CMD + " >/dev/null 2>&1")
if not CMD_RETVAL:
    OUTPUT_FILE=os.path.join(CORE_PATH, "mbed_lib.json")
    CONFIG_SCRIPT=os.path.join(CORE_PATH, "src", "ConfigToJson.py")
    if CONFIG_OPTION:
        os.system(KCONFIG_CMD + " " + os.path.join(CORE_PATH, "src", "kconfig-mbed"))
        os.system("python " + CONFIG_SCRIPT + " " + CONFIG_FILENAME + " " + OUTPUT_FILE)
    else:
        if os.path.isfile(CONFIG_FILENAME):
            print("Using existing .config. Use menuconfig option to regenerate .config")
        else:
            os.system(KCONFIG_CMD + " " + os.path.join(CORE_PATH, "src", "kconfig-mbed"))
            os.system("python " + CONFIG_SCRIPT + " " + CONFIG_FILENAME + " " + OUTPUT_FILE)
else:
    print("Install kconfig-frontends")

#Add config file and Generate .mbedignore file
shutil.copy(os.path.join(IOT_APPS_PATH, "usr_config.py"), os.path.join(CORE_PATH, "src"))
os.chdir(os.path.join(BSP_PATH))
os.system("python " + os.path.join(CORE_PATH, "src", "component.py"))
os.remove(os.path.join(CORE_PATH, "src", "usr_config.py"))

#Genarate Root CA
os.chdir(os.path.join(CORE_PATH))
os.system("cat src/include/certs/boilerplate.h > src/iot_root_ca.c")
os.system("xxd -i src/certs/" + CA_CERT_FILE + ">> src/iot_root_ca.c")
os.system("sed -i.bak 's/src.*pem/st_root_ca/g' src/iot_root_ca.c")
os.system("sed -i.bak 's/unsigned/const unsigned/g' src/iot_root_ca.c")
os.system("rm -f src/iot_root_ca.c.bak")
os.chdir(os.path.join(STDK_PATH))

os.chdir(os.path.join(IOT_APPS_PATH, "main"))
os.system("arm-none-eabi-ld -r -b binary -o onboarding_config.o onboarding_config.json")
os.system("arm-none-eabi-ld -r -b binary -o device_info.o device_info.json")

os.chdir(os.path.join(BSP_PATH))
build_cmd = "mbed compile -m" + " DISCO_L475VG_IOT01A" + " -t GCC_ARM " + FLASH_OPTION
build_ret=os.system(build_cmd)
if build_ret:
    exit(1)

