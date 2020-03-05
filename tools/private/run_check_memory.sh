#!/bin/bash

CHIP_NAME=${1}
APP_NAME=${2}
USRENG=${3}
RETRY_N=${4}
GIT_PATH=${5}

TIME=`date +%Y%m%d_%H%M%S`

BUILD_SH="build.sh"

print_usage () {
  echo "Usage: tools/run_check_memory.sh CHIP_NAME APP_NAME usr/eng RETRY_N GIT_PATH"
  echo 
  echo "ex) tools/run_check_memory.sh esp8266 smart_switch usr 5 bsp/esp8266"
  echo "ex) tools/run_check_memory.sh esp32 smart_lamp eng 5 iot-core"
  exit
}

if [ "${APP_NAME}" = "" ]; then
  print_usage
fi

if [ "${GIT_PATH}" = "" ]; then
  GIT_PATH="iot-core"
fi

if [ "${USRENG}" = "usr" ]; then
  BUILD_SH="usr_build.sh"
fi

STDK_PATH=${PWD}
TOOL_PATH=${PWD}/tools
OUTPUT_PATH=${STDK_PATH}/run_output_${TIME}

mkdir ${OUTPUT_PATH}
mkdir ${OUTPUT_PATH}/binary
mkdir ${OUTPUT_PATH}/patch

mkdir ${STDK_PATH}/run_check_memory_tmp
mkdir ${STDK_PATH}/run_check_memory_tmp/output
mkdir ${STDK_PATH}/run_check_memory_tmp/patch

echo "----- move ./*.patch files to ./run_check_memory_tmp/patch"
mv ${STDK_PATH}/*.patch ${STDK_PATH}/run_check_memory_tmp/patch


echo "----- move ./output to ./run_check_memory_tmp/output"
mv ${STDK_PATH}/output/* ${STDK_PATH}/run_check_memory_tmp/output/

cp ${TOOL_PATH}/check_memory.sh ${TOOL_PATH}/tmp_check_memory.sh
cp ${TOOL_PATH}/map_parser.py ${TOOL_PATH}/tmp_map_parser.py

num=1

until [ "${num}" -eq "${RETRY_N}" ]
do
  cd ${STDK_PATH}
  NUM=`seq -f "%04g" ${num} ${num}`
  cp -f ${TOOL_PATH}/tmp_check_memory.sh ${TOOL_PATH}/check_memory.sh
  cp -f ${TOOL_PATH}/tmp_map_parser.py ${TOOL_PATH}/map_parser.py

  echo "----- ${NUM} : build!"
  ./${BUILD_SH} ${CHIP_NAME} ${APP_NAME}

  echo "----- ${NUM} : check_mem!"
  ${TOOL_PATH}/check_memory.sh ${CHIP_NAME} ${APP_NAME}

  echo "----- ${NUM} : copy files!"
  rename s/result_/${NUM}_result_/ ./result*.csv
  mv ${NUM}_result_*.csv ${OUTPUT_PATH}

  cd ${STDK_PATH}/${GIT_PATH}
  git format-patch -1
  rename s/0001-/${NUM}-/ ./0001-*patch
  mv ./${NUM}-*.patch ${OUTPUT_PATH}/patch/

  rename s/iot/${NUM}_iot/ ${STDK_PATH}/output/${CHIP_NAME}/iot*
  mv ${STDK_PATH}/output/${CHIP_NAME}/* ${OUTPUT_PATH}/binary/

  git reset --hard HEAD^
  num=`expr ${num} + 1`
done

rm ${TOOL_PATH}/tmp_check_memory.sh
rm ${TOOL_PATH}/tmp_map_parser.py

echo "----- restore ./*.patch files"
mv ${STDK_PATH}/run_check_memory_tmp/patch/*.patch ${STDK_PATH}/

echo "----- restore ./output"
mv ${STDK_PATH}/run_check_memory_tmp/output/* ${STDK_PATH}/output/
rm -rf ${STDK_PATH}/run_check_memory_tmp
