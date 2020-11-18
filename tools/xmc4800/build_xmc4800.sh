#!/bin/bash
# Add your toolchain here
export CROSS_COMPILE=/opt/gcc-arm-none-eabi-9-2019-q4-major 

export CHIP_NAME=${1}
export PROJECT="${PWD}/bsp/${CHIP_NAME}/FreeRTOS/projects/infineon/xmc4800_iotkit/dave4/aws_demos"
export PROJECT_TITLE=${2}

export STDK_PATH="${PWD}"
export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${CHIP_NAME}"
IOT_OUTPUT_PATH="${PWD}/output/${CHIP_NAME}"
OUTPUT_BUILD=y
export COMPONENT_PATH="${PWD}/iot-core/src"
export PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"

print_usage () {
	echo "    Usage: ./build.sh xmc4800 APP_NAME"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh xmc4800 switch_example"
	echo "    ex) ./build.sh xmc4800 light_example"
	echo
}

if [ "${3}" = "clean" ]; then
	OUTPUT_BUILD=n
fi

if [ ! `command -v gawk` ];then
	echo " Environment: gawk lost, please install, sudo apt-get install gawk"
	exit
fi

if [ "${PROJECT_TITLE}" = "" ]; then
	print_usage
	exit
fi
if [ ! -d ${PROJECT_PATH} ]; then
	echo "    Error: Fail to find ${PROJECT_PATH}"
	exit 1
fi

cd ${PROJECT_PATH}/main
${CROSS_COMPILE}/bin/arm-none-eabi-ld -r -b binary -o onboarding_config.o onboarding_config.json
${CROSS_COMPILE}/bin/arm-none-eabi-ld -r -b binary -o device_info.o device_info.json
mv onboarding_config.o device_info.o ${PROJECT}/build/stdk/apps
cd ${PROJECT}/build/

# Build App
if [ "${3}" = "clean" ]; then
	make clean
else
	make all
fi

if [ ${OUTPUT_BUILD} = y ]; then
	BINARY_PATH=${PROJECT}/build
	export OUTPUT_FILE_LIST="${BINARY_PATH}/stdk.hex"
	export DEBUG_FILE_LIST="${BINARY_PATH}/stdk.map"
	${STDK_PATH}/tools/common/generate_output.sh
fi
