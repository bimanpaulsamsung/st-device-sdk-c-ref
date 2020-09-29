#!/bin/bash

BSP_NAME=${1}
PROJECT_TITLE=${2}
PROJECT_PATH="${PWD}/apps/${BSP_NAME}/${PROJECT_TITLE}"

if [ -L bsp/${BSP_NAME}/iot-core ]; then
	rm bsp/${BSP_NAME}/iot-core
fi

if [ ! -d bsp/${BSP_NAME} ]; then
	mkdir bsp/${BSP_NAME}
fi

ln -s ../../iot-core bsp/${BSP_NAME}/iot-core

if [ ! -d bsp/${BSP_NAME}/apps ]; then
        mkdir bsp/${BSP_NAME}/apps
        ln -s ${PROJECT_PATH} bsp/${BSP_NAME}/apps
fi

cd bsp/${BSP_NAME}

MBED_OS_TAG="mbed-os-5.15.2"

echo "Start Setup for mbed os"
mbed new . &> /dev/null
if [ "$?" == "0" ]; then
	echo "Apply mbed os Patches"
	cd mbed-os
	git checkout ${MBED_OS_TAG}
	git am ../../../patches/${BSP_NAME}/*.patch
	cd ../iot-core
	git submodule update --init src/deps/libsodium/libsodium
	git submodule update --init src/deps/json/cJSON
	cd ../../..
else
	if [ "$(ls mbed-os)" == "" ]; then
		echo "Failed to find source code in mbed-os"
	else
		echo "Apply mbed os Patches in existing repo"
		cd mbed-os
		git checkout ${MBED_OS_TAG}
		for patch in ../../../patches/${BSP_NAME}/*
			do patch -f -p1 < ${patch}
		done
		cd ../iot-core
		git submodule update --init src/deps/libsodium/libsodium
		git submodule update --init src/deps/json/cJSON
		cd ../../../
	fi
	echo "Check source code in mbed-os"
fi
