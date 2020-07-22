#!/bin/bash

CORE_PATH="iot-core"
CHIP_NAME=${1}
ARGUMENTS=$@

print_usage () {
	echo "    Usage: ./setup.sh CHIP_NAME"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./setup.sh esp8266"
	echo "    ex) ./setup.sh esp32"
	echo "    ex) ./setup.sh rtl8195"
	echo "    ex) ./setup.sh rtl8720c"
	echo "    ex) ./setup.sh rtl8721c"
	echo "    ex) ./setup.sh mt7682_ss"
	echo "    ex) ./setup.sh emw3166"
	echo "    ex) ./setup.sh emw3080"
	echo "    ex) ./setup.sh tizenrt esp32"
	echo "    ex) ./setup.sh tizenrt esp32wr"
	echo "    ex) ./setup.sh cc3220sf"
	echo "    ex) ./setup.sh lpc54018"
	echo "    ex) ./setup.sh cy8cproto_062_4343w"
	echo "    ex) ./setup.sh rda5981c"
	echo "    ex) ./setup.sh pic32mz"
	echo
}

if [ "${CHIP_NAME}" == "esp32" ]; then
    CHIP_NAME="esp32_v3.3"
    ARGUMENTS=`echo ${ARGUMENTS} | sed 's/esp32/esp32_v3.3/'`
fi

if [ "${CHIP_NAME}" == "" ]; then
	print_usage
	exit 0
fi

if [ ! -e tools/${CHIP_NAME}/setup_${CHIP_NAME}.sh ]; then
	echo "Failed to find tools/${CHIP_NAME}/setup_${CHIP_NAME}.sh"
	print_usage
	exit 1
fi

git submodule status ${CORE_PATH} &> /dev/null
if [ "$?" == "0" ]; then
	git submodule sync ${CORE_PATH}
	git submodule init ${CORE_PATH}
	git submodule update ${CORE_PATH}
	cd ${CORE_PATH}
	git reset --hard HEAD
	cd ..
fi

git submodule status bsp/${CHIP_NAME} &> /dev/null
if [ "$?" == "0" ]; then
	git submodule sync bsp/${CHIP_NAME}
	git submodule init bsp/${CHIP_NAME}
	git submodule update bsp/${CHIP_NAME}
	cd bsp/${CHIP_NAME}
	git reset --hard HEAD
	cd ../..
fi

tools/${CHIP_NAME}/setup_${CHIP_NAME}.sh ${ARGUMENTS}
