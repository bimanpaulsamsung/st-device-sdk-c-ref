#!/bin/bash

CHIP_NAME=${1}

##iot-core soft link
CORE_PATH="../../../../../../../iot-core"
PIC32MZ_IOT_CORE="src/system_config/pic32mz_ef_curiosity/framework/stdk/iot-core"
if [ -L ${PIC32MZ_IOT_CORE} ]
then
	rm -rf ${PIC32MZ_IOT_CORE}
fi


git submodule status bsp/${CHIP_NAME} &> /dev/null
if [ "$?" == "0" ]; then
	cd bsp/${CHIP_NAME}
	find . -type f -exec dos2unix {} &>/dev/null \;
	git add --all
	git commit -m "change dos file format to unix format with dos2unix"
	git am ../../patches/${CHIP_NAME}/*.patch
	ln -s ${CORE_PATH} ${PIC32MZ_IOT_CORE}
else
	if [ "$(ls bsp/${CHIP_NAME})" == "" ]; then
		echo "Failed to find source code in bsp/${CHIP_NAME}"
	else
		cd bsp/${CHIP_NAME}
		find . -type f -exec dos2unix {} &>/dev/null \;
		git add --all
		git commit -m "change dos file format to unix format with dos2unix"
		for patch in ../../patches/${CHIP_NAME}/*
			do patch -f -p1 < ${patch}
		done
		ln -s ${CORE_PATH} ${PIC32MZ_IOT_CORE}
	fi  
	echo "Check source code in bsp/${CHIP_NAME}"
fi

