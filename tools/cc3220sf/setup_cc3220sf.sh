#!/bin/bash

CHIP_NAME=${1}

git submodule status bsp/${CHIP_NAME} &> /dev/null

if [ "$?" == "0" ]; then
	cd bsp/${CHIP_NAME}
	git am ../../patches/${CHIP_NAME}/*.patch
	if [ -e FreeRTOS-10.1.1 ]; then
        	echo "FreeRTOS have existed!!!!"
        	exit 0
	fi
	echo "start clone FreeRTOS-10.1.1, please waitting...."
	echo ".................................................................."
	git clone https://github.com/FreeRTOS/FreeRTOS.git FreeRTOS-10.1.1 -b V10.1.1
	echo "FreeRTOS-10.1.1, ready!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
else
	if [ "$(ls bsp/${CHIP_NAME})" == "" ]; then
		echo "Failed to find source code in bsp/${CHIP_NAME}"
	else
		cd bsp/${CHIP_NAME}
		for patch in ../../patches/${CHIP_NAME}/*
			do patch -f -p1 < ${patch}
		done
	fi  
	echo "Check source code in bsp/${CHIP_NAME}"
fi

