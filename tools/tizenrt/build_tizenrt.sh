#!/bin/bash

export OS_NAME=${1}
export BOARD_NAME=${2}
export CHIP_NAME=${1}_${2}
PROJECT="${PWD}/bsp/${OS_NAME}"

export STDK_PATH="${PWD}"

IOT_OUTPUT_PATH="${PWD}/output/${CHIP_NAME}"
OUTPUT_BUILD=y

print_usage () {
	echo "    Usage: ./build.sh tizenrt BORAD_NAME APP_NAME"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh tizenrt esp32 st_lamp"
	echo "    ex) ./build.sh tizenrt esp32 st_lamp download"
	echo "    ex) ./build.sh tizenrt esp32wr st_lamp"
	echo "    ex) ./build.sh tizenrt artik053 st_lamp"
	echo
}

if [ "${4}" == "clean" ]; then
	OUTPUT_BUILD=n

	pushd ${PROJECT}/os/ > /dev/null
	#make clean
	./dbuild.sh clean
	popd > /dev/null
	exit
fi

if [ "${BOARD_NAME}" == "" ] || [ "${3}" == "" ]; then
	print_usage
	exit
fi
#if [ ! -d ${PROJECT_PATH} ]; then
#	echo "    Error: Fail to find ${PROJECT_PATH}"
#	exit 1
#fi


pushd ${PROJECT}/os/ > /dev/null

if [ "${4}" == "download" ]; then
#make download ALL
./dbuild.sh build download ALL
else
#make
./dbuild.sh
fi

popd > /dev/null

if [ ${OUTPUT_BUILD} == y ]; then
	BINARY_PATH=${PROJECT}/build/output/bin
	if [ "${BOARD_NAME}" == "esp32" ] || [ "${BOARD_NAME}" == "esp32wr" ] ; then
		export OUTPUT_FILE_LIST="${BINARY_PATH}/romfs.img ${BINARY_PATH}/tinyara.elf.bin"
	elif [ "${BOARD_NAME}" == "artik053" ]; then
		export OUTPUT_FILE_LIST="${BINARY_PATH}/romfs.img ${BINARY_PATH}/tinyara.bin"
	fi
	export DEBUG_FILE_LIST="${BINARY_PATH}/tinyara.map"
	${STDK_PATH}/tools/common/generate_output.sh
fi
