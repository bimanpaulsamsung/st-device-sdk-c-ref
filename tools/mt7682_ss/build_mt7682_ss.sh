#!/bin/bash
export BOARD_NAME=mt7682_hdk
export CHIP_NAME=${1}
PROJECT="${PWD}/bsp/${CHIP_NAME}/project/mt7682_hdk/apps/stdk_project"
export PROJECT_TITLE=${2}

export STDK_PATH="${PWD}"
export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${CHIP_NAME}"
IOT_OUTPUT_PATH="${PWD}/output/${CHIP_NAME}"
OUTPUT_BUILD=y
export COMPONENT_PATH="${PWD}/iot-core/src"
export PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"

print_usage () {
	echo "    Usage: ./build.sh mt7682 APP_NAME"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh mt7682 smart_switch"
	echo "    ex) ./build.sh mt7682 smart_lamp"
	echo
}

if [ "${3}" = "clean" ]; then
	OUTPUT_BUILD=n
	pushd ${PWD}/bsp/${CHIP_NAME} > /dev/null
	find . -name *.o | xargs rm
	find . -name *.d | xargs rm
	./build.sh ${BOARD_NAME} clean
	./build.sh ${BOARD_NAME} stdk_project clean
	if [ -d "out" ]; then
		rm -rf out
	fi
	popd > /dev/null
	exit
fi

if [ ! `command -v gawk` ];then
	echo " Enviroment: gawk lost, please install, sudo apt-get install gawk"
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

pushd ${PWD}/bsp/${CHIP_NAME}/ > /dev/null
./build.sh ${BOARD_NAME} stdk_project
popd > /dev/null
if [ ${OUTPUT_BUILD} = y ]; then
	BINARY_PATH=${PWD}/bsp/${CHIP_NAME}/out/${BOARD_NAME}/stdk_project
	export OUTPUT_FILE_LIST="${BINARY_PATH}/mt7682_bootloader.bin ${BINARY_PATH}/mt768x_default_PerRate_TxPwr.bin ${BINARY_PATH}/stdk_project.bin ${BINARY_PATH}/flash_download.cfg"
	export DEBUG_FILE_LIST="${BINARY_PATH}/stdk_project.map"
	${STDK_PATH}/tools/common/generate_output.sh
fi
