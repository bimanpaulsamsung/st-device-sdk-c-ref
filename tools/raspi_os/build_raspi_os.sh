#!/bin/bash

export BSP_NAME=${1}
export PROJECT_TITLE=${2}

IOT_APPS_PATH="${PWD}/apps/${BSP_NAME}"
PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"
EXECUTABLE_PATH="${PROJECT_PATH}/main/${PROJECT_TITLE}"

SDKCONFIG="${PROJECT_PATH}/sdkconfig"

MAKE_OPTION=all

print_usage () {
	echo "    Usage: ./build.sh BSP_NAME APP_NAME [make_option]"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh raspi_os switch_example"
	echo "    ex) ./build.sh raspi_os switch_example clean"
	echo
}

if [ "${PROJECT_TITLE}" = "" ]; then
	print_usage
	exit 0
fi

if [ ! -d ${PROJECT_PATH} ]; then
	echo "    Error: Fail to find ${PROJECT_PATH}"
	print_usage
	exit 1
fi

### Build

if [ ! "${3}" = "" ]; then
	shift 2
	MAKE_OPTION=$@
fi

while read -r flag; do
	if [ ! -z "${flag}" ] && case "${flag}" in \#*) false;; *) true;; esac; then
		STDK_CONFIGS="${STDK_CONFIGS} -D${flag}"
	fi
done < "${SDKCONFIG}"
export STDK_CONFIGS

cd ${STDK_CORE_PATH}
make ${MAKE_OPTION}
if [ ! "${?}" = "0" ]; then
	exit ${?}
fi

cd "${PROJECT_PATH}/main"
echo
make ${MAKE_OPTION}
if [ ! "${?}" = "0" ]; then
	exit ${?}
fi

echo
echo "    Run the application using: $ sudo ${EXECUTABLE_PATH}"
