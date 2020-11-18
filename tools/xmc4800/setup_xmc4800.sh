#!/bin/bash

CHIP_NAME=${1}

if [ -L bsp/${CHIP_NAME}/iot-core ]; then
    rm bsp/${CHIP_NAME}/iot-core 
fi

if [ ! -d bsp/${CHIP_NAME} ]; then
    mkdir bsp/${CHIP_NAME}
fi

git submodule status bsp/${BSP_NAME} &> /dev/null
if [ "$?" == "0" ]; then
	git submodule update --init bsp/xmc4800/FreeRTOS
	cd bsp/${CHIP_NAME}/FreeRTOS
	git submodule update --init freertos_kernel
	git submodule update --init libraries/3rdparty/pkcs11
	git am ../../../patches/${CHIP_NAME}/*.patch
	cd ../../../iot-core
	git submodule update --init src/deps/libsodium/libsodium
	git submodule update --init src/deps/json/cJSON
	cd ../
else
	if [ "$(ls bsp/xmc4800/FreeRTOS)" == "" ]; then
		echo "Failed to find source code"
	else
		echo "Apply patches in existing repo"
		cd bsp/${CHIP_NAME}/FreeRTOS
		git submodule update --init freertos_kernel
		git submodule update --init libraries/3rdparty/pkcs11
		for patch in ../../../patches/${CHIP_NAME}/*
			do patch -f -p1 < ${patch}
		done
		cd ../../../iot-core
		git submodule update --init src/deps/libsodium/libsodium
		git submodule update --init src/deps/json/cJSON
		cd ../
	fi
fi

