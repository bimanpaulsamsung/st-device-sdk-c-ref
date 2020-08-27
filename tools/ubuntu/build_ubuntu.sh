#!/bin/bash

export BSP_NAME=${1}
export PROJECT_TITLE=${2}

export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${BSP_NAME}"
PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"
EXECUTABLE_PATH="${PROJECT_PATH}/main/${PROJECT_TITLE}"

MAKE_OPTION=all

print_usage () {
	echo "    Usage: ./build.sh BSP_NAME APP_NAME [make_option]"
	echo "- - - - - - - - - - - - - - - - - - -"
	echo "    ex) ./build.sh ubuntu switch_example"
	echo "    ex) ./build.sh ubuntu switch_example clean"
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

cd ${CORE_PATH}

MAKE_LOG_FILE="make.log"
make ${MAKE_OPTION} 2> >(tee ${MAKE_LOG_FILE})
MAKE_OUTPUT=${?}
TIME_WARNING_MSG="modification time"
if cat ${MAKE_LOG_FILE} | grep -q "${TIME_WARNING_MSG}"; then
	REMAKE=true
	echo
	echo "    Clock skew detected! Please Wait..."
	echo
	find . -exec touch {} \;
fi
if [ ! "${MAKE_OUTPUT}" = "0" ]; then
	rm ${MAKE_LOG_FILE}
	exit ${MAKE_OUTPUT}
fi
if [ "${REMAKE}" = "true" ]; then
	make ${MAKE_OPTION}
fi
rm ${MAKE_LOG_FILE}

cd "${PROJECT_PATH}/main"

echo
make ${MAKE_OPTION}
if [ ! "${?}" = "0" ]; then
	exit ${?}
fi

echo
echo "    Run the application using: $ sudo ${EXECUTABLE_PATH}"
