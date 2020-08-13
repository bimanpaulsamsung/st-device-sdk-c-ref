#!/bin/bash

export CHIP_NAME=${1}
export PROJECT_TITLE=${2}

export STDK_PATH="${PWD}"
export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${CHIP_NAME}"
PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"

CA_CERT_FILE="root_ca_prod.pem"

print_usage () {
  echo "    Usage: ./build.sh CHIP_NAME PROJECT_NAME [make_option]"
  echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch flash"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch clean"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch clean flash"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch menuconfig"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch menuconfig flash"
  echo
}

if [ "${PROJECT_TITLE}" = "" ]; then
  print_usage
  exit
fi

if [ ! -d ${PROJECT_PATH} ]; then
  echo "    Error: Fail to find ${PROJECT_PATH}"
  print_usage
  exit 1
fi

for arg in "$@"; do
    if [ "$arg" == "flash" -o  "$arg" == "clean" ]; then
        FLASH_OPTION="${FLASH_OPTION} --$arg"
    fi
    if [ "$arg" == "menuconfig" ]; then
	    CONFIG_OPTION=true
    fi
done

#Generate Configuration in Json formate
if [ "$CONFIG_OPTION" ]; then cmd=kconfig-mconf; else cmd=kconfig-conf; fi
if which $cmd &> /dev/null; then
    if [ $CONFIG_OPTION ]; then
        kconfig-mconf "${CORE_PATH}/src/kconfig-mbed"
    else
        kconfig-conf "${CORE_PATH}/src/kconfig-mbed"
    fi
    INPUT_FILE="${PWD}/.config"
    OUTPUT_FILE="${CORE_PATH}/mbed_lib.json"
    CONFIG_SCRIPT="${CORE_PATH}/src/ConfigToJson.py"
    python $CONFIG_SCRIPT $INPUT_FILE $OUTPUT_FILE
else
    echo "Install kconfig-frontends"
fi

# Add config file and Generate .mbedignore file
cp ${PROJECT_PATH}/usr_config.py ${CORE_PATH}/src
cd ./bsp/${CHIP_NAME}
python iot-core/src/component.py
rm ${CORE_PATH}/src/usr_config.*

#Genarate Root CA
cd ${CORE_PATH}
cat src/include/certs/boilerplate.h > src/iot_root_ca.c
xxd -i src/certs/${CA_CERT_FILE} >> src/iot_root_ca.c
sed -i.bak 's/src.*pem/st_root_ca/g' src/iot_root_ca.c
sed -i.bak 's/unsigned/const unsigned/g' src/iot_root_ca.c
rm -f src/iot_root_ca.c.bak
cd ${STDK_PATH}

#Genarate Object Files from json files
cd ${PROJECT_PATH}/main
arm-none-eabi-ld -r -b binary -o onboarding_config.o onboarding_config.json
arm-none-eabi-ld -r -b binary -o device_info.o device_info.json
cd ${STDK_PATH}

cd ./bsp/${CHIP_NAME}
### Build
mbed compile -m cy8cproto_062_4343w -t GCC_ARM ${FLASH_OPTION}
