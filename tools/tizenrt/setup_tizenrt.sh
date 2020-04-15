#!/bin/bash

CHIP_NAME=${1}
BOARD_NAME=${2}

git submodule status bsp/${CHIP_NAME} &> /dev/null
if [ "$?" == "0" ]; then
	cd bsp/${CHIP_NAME}
	
	git submodule update --init build/tools/esp32/esptool_py/esptool
	#git submodule update --init external/stdk/st-device-sdk-c
	git submodule update --init external/libsodium/libsodium
	
	cp -r ../../iot-core/* external/stdk/st-device-sdk-c/
	
	git am ../../patches/${CHIP_NAME}/*.patch
	
	cd os
	#source ./tools/setenv.sh ~/gcc-arm-none-eabi-6-2017-q1-update/bin
	#config with different board
	./dbuild.sh clean
	./dbuild.sh distclean
	if [ "${BOARD_NAME}" == "esp32" ]; then
		./tools/configure.sh esp32_DevKitC/STDK
	elif [ "${BOARD_NAME}" == "esp32wr" ]; then
		./tools/configure.sh esp_wrover_kit/STDK
	elif [ "${BOARD_NAME}" == "artik053" ]; then
		./tools/configure.sh artik053/STDK
	fi

else
	if [ "$(ls bsp/${CHIP_NAME})" == "" ]; then
		echo "Failed to find source code in bsp/${CHIP_NAME}"
	else
		cd bsp/${CHIP_NAME}
		
		git submodule deinit build/tools/esp32/esptool_py/esptool
		git submodule deinit external/libsodium/libsodium
		git submodule update --init build/tools/esp32/esptool_py/esptool
		git submodule update --init external/libsodium/libsodium
		#ln -s ../../../../../iot-core/src external/stdk/st-device-sdk-c/src
		
		rm -rf external/stdk/st-device-sdk-c/*
		cp -r ../../iot-core/* external/stdk/st-device-sdk-c/
		
		for patch in ../../patches/${CHIP_NAME}/*
			do patch -f -p1 < ${patch}
		done
		
		cd os
		#config with different board
		./dbuild.sh clean
		./dbuild.sh distclean
		if [ "${BOARD_NAME}" == "esp32" ]; then
			./tools/configure.sh esp32_DevKitC/STDK
		elif [ "${BOARD_NAME}" == "esp32wr" ]; then
			./tools/configure.sh esp_wrover_kit/STDK
		elif [ "${BOARD_NAME}" == "artik053" ]; then
			./tools/configure.sh artik053/STDK
		fi
	fi
	echo "Check source code in bsp/${CHIP_NAME}"
fi

