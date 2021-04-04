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

#Genarate Root CA
os.chdir(os.path.join(CORE_PATH))
os.system("cat src/include/certs/boilerplate.h > src/iot_root_ca.c")
os.system("xxd -i src/certs/" + CA_CERT_FILE + ">> src/iot_root_ca.c")
os.system("sed -i.bak 's/src.*pem/st_root_ca/g' src/iot_root_ca.c")
os.system("sed -i.bak 's/unsigned/const unsigned/g' src/iot_root_ca.c")
os.system("rm -f src/iot_root_ca.c.bak")
os.chdir(os.path.join(STDK_PATH))

os.chdir(os.path.join(BSP_PATH))
build_cmd = os.path.join("make -DBSP_PATH=", BSP_PATH, " -DAPP_PATH=",IOT_APPS_PATH, "-DCORE_PATH", CORE_PATH)
build_ret=os.system(build_cmd)
if build_ret:
    exit(1)
    