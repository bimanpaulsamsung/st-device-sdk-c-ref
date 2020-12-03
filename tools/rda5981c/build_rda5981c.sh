#!/bin/bash

export CHIP_NAME=${1}
export PROJECT_TITLE=${2}

export STDK_PATH="${PWD}"
export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${CHIP_NAME}"
PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"

OUTPUT_BUILD=y

CA_CERT_FILE="root_ca_prod_k8.pem"

print_usage () {
  echo "    Usage: ./build.sh CHIP_NAME PROJECT_NAME [make_option]"
  echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch flash"
  echo
}

if [ "${3}" = "clean" ]; then
	OUTPUT_BUILD=n
fi

if [ ! -d bsp/${CHIP_NAME}/apps ]; then
  mkdir bsp/${CHIP_NAME}/apps
  ln -s ${PROJECT_PATH} bsp/${CHIP_NAME}/apps
fi

if [ "${PROJECT_TITLE}" = "" ]; then
  print_usage
  exit
fi

if [ ! -d ${PROJECT_PATH} ]; then
  echo "    Error: Fail to find ${PROJECT_PATH}"
  print_usage
  exit 1
fi

# Add config file and Generate .mbedignore file
cp ${PROJECT_PATH}/mbed_app.json ./bsp/${CHIP_NAME}
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
/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ld -r -b binary -o onboarding_config.o onboarding_config.json
/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ld -r -b binary -o device_info.o device_info.json
cd ${STDK_PATH}

cd ./bsp/${CHIP_NAME}
### Build

if [ ${OUTPUT_BUILD} = y ]; then
        mbed compile -m UNO_91H -t GCC_ARM
        BINARY_PATH=${STDK_PATH}/bsp/${CHIP_NAME}/BUILD/UNO_91H/GCC_ARM/
        export OUTPUT_FILE_LIST="${BINARY_PATH}/${CHIP_NAME}.bin"
        export DEBUG_FILE_LIST="${BINARY_PATH}/${CHIP_NAME}.map"
        ${STDK_PATH}/tools/common/generate_output.sh
else
	echo "Cleaning..."
	rm -rf BUILD
	rm apps/${PROJECT_TITLE}/main/*.o
fi
