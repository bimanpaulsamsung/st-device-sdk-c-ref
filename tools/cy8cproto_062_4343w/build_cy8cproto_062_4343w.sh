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


#Generate .mbedignore
File=".mbedignore"
INCLUDE_FILES="MbedIncludeList.txt"

echo "Generating .mbedignore ..."
index=0
while read line; do
        LIST[$index]="$line"
        index=$(expr $index + 1)
done < "$PWD/$INCLUDE_FILES"

find iot-core/ -name "*.c" > $File
find iot-core/ -name "*.cpp" >> $File
find iot-core/ -name "*.a" >> $File
find apps/ -name "*.c" >> $File
find apps/ -name "*.cpp" >> $File

echo "Processing...."
for i in "${LIST[@]}"
do
        sed -i -e "/"${i//\//\\\/}"/d" $File
done
echo "Generated .mbedignore"

#Check Options
if [ "${3}" = "flash" ]; then
  shift 2
  FLASH_OPTION="--$@"
fi


#Genarate Root CA
#cd ${CORE_PATH}
#cat src/include/certs/boilerplate.h > src/iot_root_ca.c
#xxd -i src/certs/${CA_CERT_FILE} >> src/iot_root_ca.c
#sed -i.bak 's/src.*pem/st_root_ca/g' src/iot_root_ca.c
#sed -i.bak 's/unsigned/const unsigned/g' src/iot_root_ca.c
#rm -f src/iot_root_ca.c.bak
#cd ${STDK_PATH}

#Genarate Object Files from json files
cd ${PROJECT_PATH}/main
arm-none-eabi-ld -r -b binary -o onboarding_config.o onboarding_config.json
arm-none-eabi-ld -r -b binary -o device_info.o device_info.json
cd ${STDK_PATH}

### Build
mbed compile -m cy8cproto_062_4343w -t GCC_ARM ${FLASH_OPTION}
