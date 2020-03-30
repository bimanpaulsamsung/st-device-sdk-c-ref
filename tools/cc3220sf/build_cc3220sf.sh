#!/bin/bash

export CHIP_NAME=${1}
PROJECT_KERNEL="${PWD}/bsp/${CHIP_NAME}/kernel"
PROJECT_DRV="${PWD}/bsp/${CHIP_NAME}/source/ti/drivers"
PROJECT_NET="${PWD}/bsp/${CHIP_NAME}/source/ti/net"
PROJECT="${PWD}/bsp/${CHIP_NAME}/examples/rtos/CC3220SF_LAUNCHXL/stdk"
export PROJECT_TITLE=${2}

export STDK_PATH="${PWD}"
export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${CHIP_NAME}"
IOT_OUTPUT_PATH="${PWD}/output/${CHIP_NAME}"
OUTPUT_BUILD=y
export COMPONENT_PATH="${PWD}/iot-core/src"
export PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"

print_usage () {
	echo "    Usage: ./build.sh cc3220sf APP_NAME"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh cc3220sf st_switch"
	echo "    ex) ./build.sh cc3220sf st_lamp"
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

export XDCTOOLS_JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

# Build TI sdk kernel
pushd ${PROJECT_KERNEL} > /dev/null
make ${3}
popd > /dev/null

# Build TI drivers
pushd ${PROJECT_DRV} > /dev/null
make ${3}
popd > /dev/null

# Build TI network libs
pushd ${PROJECT_NET} > /dev/null
make ${3}
popd > /dev/null

# Build TI network utils libs
pushd ${PROJECT_NET}/utils > /dev/null
make ${3}
popd > /dev/null

# Build TI drvier wifi libs
pushd ${PROJECT_DRV}/net/wifi > /dev/null
make ${3}
popd > /dev/null

# Build TI drvier netif libs
pushd ${PROJECT_DRV}/net/wifi/slnetif > /dev/null
make ${3}
popd > /dev/null

# Build App
pushd ${PROJECT} > /dev/null
make ${3}
popd > /dev/null

if [ ${OUTPUT_BUILD} = y ]; then
	BINARY_PATH=${PROJECT}/
	export OUTPUT_FILE_LIST="${BINARY_PATH}/stdk.bin"
	export DEBUG_FILE_LIST="${BINARY_PATH}/stdk.map"
	${STDK_PATH}/tools/common/generate_output.sh
fi
