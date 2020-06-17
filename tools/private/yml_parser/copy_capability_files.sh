#!/bin/bash

CHIP_LIST=("esp8266" "esp32" "rtl8195" "rtl8720c" "rtl8721c" "emw3166" "emw3080" "cc3220sf" "lpc54018")

SRC_PATH="apps/esp8266/capability_example/main"
BASE_PATH=""
if [ ! -d ${SRC_PATH} ]; then
    SRC_PATH="../../../apps/esp8266/capability_example/main"
    BASE_PATH="../../../"
    if [ ! -d ${SRC_PATH} ]; then
	   echo "fail to find SRC_PATH"
	   exit 0
	fi
fi
#ST_SWITCH
for CHIP_NAME in "${CHIP_LIST[@]}"; do
	APP_PATH=${BASE_PATH}/apps/${CHIP_NAME}/st_switch
	if [ -d ${APP_PATH} ]; then
		cp ${SRC_PATH}/caps_switch.* ${APP_PATH}/main/
	fi

	APP_PATH=${BASE_PATH}/apps/${CHIP_NAME}/st_lamp
	if [ -d ${APP_PATH} ]; then
		cp ${SRC_PATH}/caps_switch.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_switchLevel.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_colorControl.* ${APP_PATH}/main/
	fi

	APP_PATH=${BASE_PATH}/apps/${CHIP_NAME}/st_lamp_conn_retry
	if [ -d ${APP_PATH} ]; then
		cp ${SRC_PATH}/caps_switch.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_switchLevel.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_colorControl.* ${APP_PATH}/main/
	fi

	APP_PATH=${BASE_PATH}/apps/${CHIP_NAME}/st_thermostat
	if [ -d ${APP_PATH} ]; then
		cp ${SRC_PATH}/caps_thermostatMode.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_thermostatOperatingState.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_thermostatHeatingSetpoint.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_temperatureMeasurement.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_thermostatFanMode.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_thermostatCoolingSetpoint.* ${APP_PATH}/main/
	fi

	APP_PATH=${BASE_PATH}/apps/${CHIP_NAME}/st_air_monitor
	if [ -d ${APP_PATH} ]; then
		cp ${SRC_PATH}/caps_alarm.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_formaldehydeMeasurement.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_airQualitySensor.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_carbonDioxideMeasurement.* ${APP_PATH}/main/
		cp ${SRC_PATH}/caps_carbonMonoxideDetector.* ${APP_PATH}/main/
	fi
done

