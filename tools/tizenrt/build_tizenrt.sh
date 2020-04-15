#!/bin/bash

export OS_NAME=${1}
export BOARD_NAME=${2}
export CHIP_NAME=${1}_${2}
export APP_NAME=${3}
PROJECT="${PWD}/bsp/${OS_NAME}"

export STDK_PATH="${PWD}"

IOT_OUTPUT_PATH="${PWD}/output/${CHIP_NAME}"
OUTPUT_BUILD=y

print_usage () {
	echo "    Usage: ./build.sh tizenrt BOARD_NAME APP_NAME"
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

pushd ${PROJECT}/os/ > /dev/null

#check board name is consistent with setup
if [ "${BOARD_NAME}" == "esp32" ]; then
	if [ `grep -c "CONFIG_ARCH_BOARD_ESP32_DEVKITC=y" .config` -eq '0' ]; then
		echo "trying to build board esp32 devkit, but the config is inconsistent, please check setup"
		exit
	fi
elif [ "${BOARD_NAME}" == "esp32wr" ]; then
	if [ `grep -c "CONFIG_ARCH_BOARD_ESP_WROVER_KIT=y" .config` -eq '0' ]; then
		echo "trying to build board esp32 wroverkit, but the config is inconsistent, please check setup"
		exit
	fi
elif [ "${BOARD_NAME}" == "artik053" ]; then
	if [ `grep -c "CONFIG_ARCH_BOARD_ARTIK05X_FAMILY=y" .config` -eq '0' ]; then
		echo "trying to build board artik053, but the config is inconsistent, please check setup"
		exit
	fi
else
	echo "unrecognized board name"
	print_usage
	exit
fi

#check app name, and update config if necessary
if [ "${APP_NAME}" == "st_lamp" ]; then

	if [ `grep -c "CONFIG_EXAMPLES_STDK_SMART_LAMP" .config` -eq '0' ]; then
	echo "No config for st_lamp, update config and clean first"
	sed -i "s/CONFIG_EXAMPLES_STDK_SMART_SWITCH/CONFIG_EXAMPLES_STDK_SMART_LAMP/g" .config
	sed -i "s/smart_switch_main/smart_lamp_main/g" .config
	./dbuild.sh clean
	fi

elif [ "${APP_NAME}" == "st_switch" ]; then

	if [ `grep -c "CONFIG_EXAMPLES_STDK_SMART_SWITCH" .config` -eq '0' ]; then
	echo "No config for st_switch, update config and clean first"
	sed -i "s/CONFIG_EXAMPLES_STDK_SMART_LAMP/CONFIG_EXAMPLES_STDK_SMART_SWITCH/g" .config
	sed -i "s/smart_lamp_main/smart_switch_main/g" .config
	./dbuild.sh clean
	fi

fi

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
