#!/bin/bash

CHIP_NAME=${1}
PROJECT_TITLE=${2}
PROJECT_PATH="${PWD}/apps/${CHIP_NAME}/${PROJECT_TITLE}"

if [ -L bsp/${CHIP_NAME}/iot-core ]; then
	rm bsp/${CHIP_NAME}/iot-core
fi

if [ ! -d bsp/${CHIP_NAME} ]; then
	mkdir bsp/${CHIP_NAME}
fi

ln -s ../../iot-core bsp/${CHIP_NAME}/iot-core

if [ ! -d bsp/${CHIP_NAME}/apps ]; then
        mkdir bsp/${CHIP_NAME}/apps
        ln -s ${PROJECT_PATH} bsp/${CHIP_NAME}/apps
fi

cd bsp/${CHIP_NAME}

MBED_OS_TAG="mbed-os-5.15.2"

echo "Start Setup for mbed os"
mbed new . &> /dev/null
if [ "$?" == "0" ]; then
	echo "Apply mbed os Patches"
	cd mbed-os
	git checkout ${MBED_OS_TAG}
	git am ../../../patches/${CHIP_NAME}/*.patch
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
		for patch in ../../../patches/${CHIP_NAME}/*
			do patch -f -p1 < ${patch}
		done
		cd ../iot-core
		git submodule update --init src/deps/libsodium/libsodium
		git submodule update --init src/deps/json/cJSON
		cd ../../../
	fi
	echo "Check source code in mbed-os"
fi
