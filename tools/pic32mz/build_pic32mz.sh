#!/bin/bash
export CHIP_NAME=${1}
export PROJECT_TITLE=${2}
export STDK_PATH="${PWD}"

## app soft link
PROJECT_PATH="../../../../../../../../apps/${CHIP_NAME}/${PROJECT_TITLE}"
PIC32MZ_APP="bsp/${CHIP_NAME}/src/system_config/pic32mz_ef_curiosity/framework/stdk/apps/${PROJECT_TITLE}"

if [ -L ${PIC32MZ_APP} ]
then
	rm -rf ${PIC32MZ_APP}
fi

ln -s ${PROJECT_PATH} ${PIC32MZ_APP}


## Compile
IOT_OUTPUT_PATH="${PWD}/output/${CHIP_NAME}"
OUTPUT_BUILD=y
export COMPONENT_PATH="${PWD}/iot-core/src"
print_usage () {
	echo "    Usage: ./build.sh pic32mz APP_NAME"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh pic32mz st_switch"
	echo
}

if [ "${3}" = "clean" ]; then
	pushd ${PWD}/bsp/${CHIP_NAME}/wifi_rgb_easy_configuration.X > /dev/null
	make clean
	popd > /dev/null
	exit
fi

if [ "${PROJECT_TITLE}" = "" ]; then
	print_usage
	exit
fi

pushd ${PWD}/bsp/${CHIP_NAME}/wifi_rgb_easy_configuration.X/ > /dev/null
make -f nbproject/Makefile-pic32mz_ef_curiosity.mk SUBPROJECTS= .build-conf
popd > /dev/null

if [ ${OUTPUT_BUILD} = y ]; then
	BINARY_PATH=${PWD}/bsp/${CHIP_NAME}/wifi_rgb_easy_configuration.X/dist/pic32mz_ef_curiosity/production
	export OUTPUT_FILE_LIST="${BINARY_PATH}/wifi_rgb_easy_configuration.X.production.elf"
	export DEBUG_FILE_LIST="${BINARY_PATH}/wifi_rgb_easy_configuration.X.production.map"
	${STDK_PATH}/tools/common/generate_output.sh
fi
