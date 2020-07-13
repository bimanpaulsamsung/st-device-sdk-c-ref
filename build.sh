#!/bin/bash

CHIP_NAME=${1}
APP_NAME=${2}
ARGUMENTS=$@

if [[ "${1}" == "apps/"*"/"?* ]]; then
    CHIP_NAME=`echo ${1} | awk -F'/' '{print $2}'`
    APP_NAME=`echo ${1} | awk -F'/' '{print $3}'`
    shift 1
    ARGUMENTS="${CHIP_NAME} ${APP_NAME} $@"
fi

print_usage () {
    echo "    Usage: ./build.sh CHIP_NAME APP_NAME"
    echo "                     or "
    echo "           ./build.sh apps/CHIP_NAME/APP_NAME"
    echo "- - - - - - - - - - - - - - - - - - -"
    echo "    ex) ./build.sh apps/esp8266/st_switch"
    echo "    ex) ./build.sh apps/esp32/st_switch"
    echo "    ex) ./build.sh rtl8195 st_switch"
    echo "    ex) ./build.sh rtl8720c st_switch"
    echo "    ex) ./build.sh rtl8721c st_switch"
    echo "    ex) ./build.sh emw3166 st_switch"
    echo "    ex) ./build.sh emw3080 st_switch"
    echo "    ex) ./build.sh tizenrt esp32 st_lamp"
    echo "    ex) ./build.sh cc3220sf st_switch"
    echo "    ex) ./build.sh lpc54018 st_switch"
    echo "    ex) ./build.sh cy8cproto_062_4343w st_switch"
    echo "    ex) ./build.sh rda5981c st_switch"
    echo "    ex) ./build.sh pic32mz st_switch"
    echo
}

print_path() {
    if [ "${CHIP_NAME}" == "" -o ! -d apps/${CHIP_NAME} ]; then
        echo "Fail to find CHIP_NAME (apps/${CHIP_NAME}) "
        echo "ls apps/"
        ls apps/
    elif [ "${APP_NAME}" == "" -o ! -d apps/${CHIP_NAME}/${APP_NAME} ]; then
        echo "Fail to find APP_NAME (apps/${CHIP_NAME}/${APP_NAME})"
        echo "ls apps/${CHIP_NAME}/"
        ls apps/${CHIP_NAME}
    fi
    echo

}

if [ "${CHIP_NAME}" == "" ]; then
    print_usage
    print_path
    exit 0
fi

if [ ! -e tools/${CHIP_NAME}/build_${CHIP_NAME}.sh ]; then
    echo "Failed to find tools/${CHIP_NAME}/build_${CHIP_NAME}.sh"
    print_usage
    print_path
    exit 1
fi

tools/${CHIP_NAME}/build_${CHIP_NAME}.sh ${ARGUMENTS}
if [ "$?" -ne "0" ]; then
    print_path
fi
