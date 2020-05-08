#!/bin/bash
export BOARD_NAME=lpc54018
export CHIP_NAME=${1}
PROJECT="${PWD}/bsp/${CHIP_NAME}/boards/lpc54018iotmodule/stdk"
PROJECT_COMPILE_PATH="${PROJECT}/armgcc"
export PROJECT_TITLE=${2}

export STDK_PATH="${PWD}"
export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${CHIP_NAME}"
IOT_OUTPUT_PATH="${PWD}/output/${CHIP_NAME}"
OUTPUT_BUILD=y
export COMPONENT_PATH="${PWD}/iot-core/src"
export PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"

print_usage () {
	echo "    Usage: ./build.sh lpc54018 APP_NAME"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh lpc54018 st_switch"
	echo
}

if [ "${3}" = "clean" ]; then
	OUTPUT_BUILD=n
	pushd ${PROJECT_COMPILE_PATH} > /dev/null
	chmod 777 ./clean.sh
	./clean.sh
	popd > /dev/null
	rm ${PROJECT_PATH}/main/*.o
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

pushd ${PROJECT_COMPILE_PATH} > /dev/null
echo " ${PWD}"
chmod 777 ./build_debug.sh
./build_debug.sh
popd > /dev/null

if [ ${OUTPUT_BUILD} = y ]; then
	BINARY_PATH=${PROJECT_COMPILE_PATH}/debug
	export OUTPUT_FILE_LIST="${BINARY_PATH}/stdk_${PROJECT_TITLE}.bin"
	export DEBUG_FILE_LIST="${PROJECT_COMPILE_PATH}/output.map"
	${STDK_PATH}/tools/common/generate_output.sh
fi
