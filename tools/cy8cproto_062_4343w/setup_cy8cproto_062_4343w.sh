#!/bin/bash

CHIP_NAME=${1}

MBED_OS_TAG="mbed-os-5.15.1"

echo "Start Setup for mbed os"
mbed new . &> /dev/null
if [ "$?" == "0" ]; then
	echo "Apply mbed os Patches"
	cd mbed-os
	git checkout ${MBED_OS_TAG}
	git am ../patches/${CHIP_NAME}/*.patch
	cd ..
	cd iot-core
	git submodule update --init src/deps/libsodium/libsodium
	git submodule update --init src/deps/json/cJSON
	cd ..
else
	if [ "$(ls mbed-os)" == "" ]; then
		echo "Failed to find source code in mbed-os"
	else
		echo "Apply mbed os Patches in existing repo"
		cd mbed-os
		git checkout ${MBED_OS_TAG}
		for patch in ../patches/${CHIP_NAME}/*
			do patch -f -p1 < ${patch}
		done
		cd ..
		cd iot-core
		git submodule update --init src/deps/libsodium/libsodium
		git submodule update --init src/deps/json/cJSON
		cd ..
	fi
	echo "Check source code in mbed-os"
fi