#!/bin/bash

WARN_MSG="WARN : This script will be DEPRECATED. Please use 'build.py'"

export STDK_CORE_PATH=`readlink -e -n "./iot-core"`

BSP_NAME=${1}
APP_NAME=${2}
ARGUMENTS=$@

if [[ "${1}" == "apps/"*"/"?* ]]; then
    BSP_NAME=`echo ${1} | awk -F'/' '{print $2}'`
    APP_NAME=`echo ${1} | awk -F'/' '{print $3}'`
    shift 1
    ARGUMENTS="${BSP_NAME} ${APP_NAME} $@"
fi

print_usage () {
    echo "    Usage: ./build.sh BSP_NAME APP_NAME"
    echo "                     or "
    echo "           ./build.sh apps/BSP_NAME/APP_NAME"
    echo "- - - - - - - - - - - - - - - - - - -"
    echo "    ex) ./build.sh apps/esp8266/switch_example"
    echo "    ex) ./build.sh apps/esp32/switch_example"
    echo "    ex) ./build.sh rtl8195 switch_example"
    echo "    ex) ./build.sh rtl8720c switch_example"
    echo "    ex) ./build.sh rtl8721c switch_example"
    echo "    ex) ./build.sh emw3166 switch_example"
    echo "    ex) ./build.sh emw3080 switch_example"
    echo "    ex) ./build.sh ubuntu switch_example"
    echo "    ex) ./build.sh raspi_os switch_example"

    echo "    ex) ./build.sh emw3166 st_switch"
    echo "    ex) ./build.sh emw3080 st_switch"
    echo "    ex) ./build.sh tizenrt esp32 st_lamp"
    echo "    ex) ./build.sh cc3220sf st_switch"
    echo "    ex) ./build.sh lpc54018 st_switch"
    echo "    ex) ./build.sh cy8cproto_062_4343w switch_example"
    echo "    ex) ./build.sh rda5981c st_switch"
    echo "    ex) ./build.sh pic32mz st_switch"
    echo
}

print_path() {
    if [ "${BSP_NAME}" == "" -o ! -d apps/${BSP_NAME} ]; then
        echo "Fail to find BSP_NAME (apps/${BSP_NAME}) "
        echo "ls apps/"
        ls apps/
    elif [ "${APP_NAME}" == "" -o ! -d apps/${BSP_NAME}/${APP_NAME} ]; then
        echo "Fail to find APP_NAME (apps/${BSP_NAME}/${APP_NAME})"
        echo "ls apps/${BSP_NAME}/"
        ls apps/${BSP_NAME}
    fi
    echo

}

if [ "${BSP_NAME}" == "" ]; then
    print_usage
    print_path
    echo ${WARN_MSG}
    exit 0
fi

if [ ! -e tools/${BSP_NAME}/build_${BSP_NAME}.sh ]; then
    echo "Failed to find tools/${BSP_NAME}/build_${BSP_NAME}.sh"
    print_usage
    print_path
    echo ${WARN_MSG}
    exit 1
fi

tools/${BSP_NAME}/build_${BSP_NAME}.sh ${ARGUMENTS}
if [ "$?" -ne "0" ]; then
    print_path
fi
echo ${WARN_MSG}
