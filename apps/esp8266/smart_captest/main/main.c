/* ***************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

//for implementing main features of IoT device
#include <stdbool.h>
#include <string.h>

#include "st_dev.h"
#include "device_control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "caps_thermostatMode.h"
#include "caps_switchLevel.h"
#include "caps_threeAxis.h"
#include "caps_videoCapture.h"
#include "caps_colorControl.h"
#include "caps_switch.h"
#include "caps_formaldehydeMeasurement.h"

// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]	asm("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]	asm("_binary_onboarding_config_json_end");

// device_info_start is null-terminated string
extern const uint8_t device_info_start[]	asm("_binary_device_info_json_start");
extern const uint8_t device_info_end[]		asm("_binary_device_info_json_end");

static caps_thermostatMode_data_t *cap_thermostatMode_data;
static caps_switchLevel_data_t *cap_switchLevel_data;
static caps_threeAxis_data_t *cap_threeAxis_data;
static caps_videoCapture_data_t *cap_videoCapture_data;
static caps_colorControl_data_t *cap_colorControl_data;
static caps_switch_data_t *cap_switch_data;
static caps_formaldehydeMeasurement_data_t *cap_formaldehydeMeasurement_data;

static iot_status_t g_iot_status;
static iot_stat_lv_t g_iot_stat_lv;

IOT_CTX *ctx = NULL;

void send_test_capabilities();

static void button_event(IOT_CAP_HANDLE *handle, int type, int count)
{
	if (type == BUTTON_SHORT_PRESS) {
		printf("Button short press, count: %d\n", count);
		switch(count) {
			case 1:
				send_test_capabilities();
				break;
			case 5:
				/* clean-up provisioning & registered data with reboot option*/
				st_conn_cleanup(ctx, true);
				break;
			default:
				led_blink(GPIO_OUTPUT_NOTIFICATION_LED, 100, count);
				break;
		}
	}
}

static void iot_status_cb(iot_status_t status,
		iot_stat_lv_t stat_lv, void *usr_data)
{
	g_iot_status = status;
	g_iot_stat_lv = stat_lv;

	printf("status: %d, stat: %d\n", g_iot_status, g_iot_stat_lv);
}

void iot_noti_cb(iot_noti_data_t *noti_data, void *noti_usr_data)
{
	printf("Notification message received\n");
}

static void captest_task(void *arg)
{
	IOT_CAP_HANDLE *handle = (IOT_CAP_HANDLE *)arg;

	int button_event_type;
	int button_event_count;

	for (;;) {
		if (get_button_event(&button_event_type, &button_event_count)) {
			button_event(handle, button_event_type, button_event_count);
		}

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

// CAPTEST : make init_cb for test capabilities

char *thermostatMode_supportedMode[] = {"auto", "off"};
static void cap_thermostatMode_init_cb(struct caps_thermostatMode_data *caps_data)
{
    caps_data->set_thermostatMode_value(caps_data, caps_helper_thermostatMode.attr_thermostatMode.values[CAP_ENUM_THERMOSTATMODE_THERMOSTATMODE_VALUE_AUTO]);
    caps_data->set_supportedThermostatModes_value(caps_data, (const char **)thermostatMode_supportedMode, sizeof(thermostatMode_supportedMode)/sizeof(char *));
}
static void cap_switchLevel_init_cb(struct caps_switchLevel_data *caps_data)
{
    caps_data->set_level_value(caps_data, 50);
	caps_data->set_level_unit(caps_data, caps_helper_switchLevel.attr_level.units[CAP_ENUM_SWITCHLEVEL_LEVEL_UNIT_PERCENT]);
}
static void cap_threeAxis_init_cb(struct caps_threeAxis_data *caps_data)
{   
    caps_data->set_threeAxis_value(caps_data, 1000, 2000, 3000);
}
static void cap_videoCapture_init_cb(struct caps_videoCapture_data *caps_data)
{   
    //
}
static void cap_colorControl_init_cb(struct caps_colorControl_data *caps_data)
{   
    caps_data->set_color_value(caps_data, 0, 100);
}
void cap_switch_init_cb(struct caps_switch_data *caps_data)
{
	caps_data->set_switch_value(caps_data, caps_helper_switch.attr_switch.values[CAP_ENUM_SWITCH_SWITCH_VALUE_OFF]);
}
void cap_formaldehydeMeasurement_init_cb(struct caps_formaldehydeMeasurement_data *caps_data)
{
	caps_data->set_formaldehydeLevel_value(caps_data, 5000);
	caps_data->set_formaldehydeLevel_unit(caps_data, caps_helper_formaldehydeMeasurement.attr_formaldehydeLevel.units[CAP_ENUM_FORMALDEHYDEMEASUREMENT_FORMALDEHYDELEVEL_UNIT_MG_PER_M3]);
}


// CAPTEST : make cmd_cb for test capabilities
static void cap_thermostatMode_cmd_cb(struct caps_thermostatMode_data *caps_data)
{
    const char *value = caps_data->get_thermostatMode_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_switchLevel_cmd_cb(struct caps_switchLevel_data *caps_data)
{
    int value = caps_data->get_level_value(caps_data);
	printf("%s : value : %d", __func__, value);
}

static void cap_videoCapture_cmd_cb(struct caps_videoCapture_data *caps_data)
{
	printf("%s", __func__);
}
static void cap_colorControl_cmd_cb(struct caps_colorControl_data *caps_data)
{
	double hue = caps_data->get_hue_value(caps_data);
	double saturation = caps_data->get_saturation_value(caps_data);
	printf("%s : %d, %d", __func__, (int)hue, (int)saturation);
}
static void cap_switch_cmd_cb(struct caps_switch_data *caps_data)
{
	const char *value = caps_data->get_switch_value(caps_data);
	printf("%s : value : %s", __func__, value);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	int thermostat_state = send_count % CAP_ENUM_THERMOSTATMODE_THERMOSTATMODE_VALUE_MAX;
	cap_thermostatMode_data->set_thermostatMode_value(cap_thermostatMode_data, caps_helper_thermostatMode.attr_thermostatMode.values[thermostat_state]);
	cap_thermostatMode_data->attr_thermostatMode_send(cap_thermostatMode_data);

	int level_value = (send_count * 25) % 100;
	cap_switchLevel_data->set_level_value(cap_switchLevel_data, level_value);
	cap_switchLevel_data->attr_level_send(cap_switchLevel_data);

	int x = (send_count * 2000) % 10000;
	int y = (send_count * 2000) % 10000;
	int z = (send_count * 2000) % 10000;
	cap_threeAxis_data->set_threeAxis_value(cap_threeAxis_data, x, y, z);
	cap_threeAxis_data->attr_threeAxis_send(cap_threeAxis_data);
	
	//videoCapture

	double hue = (send_count * 25) % 100;
	double saturation = (send_count * 25) % 100;
	cap_colorControl_data->set_color_value(cap_colorControl_data, hue, saturation);
	cap_colorControl_data->attr_color_send(cap_colorControl_data);

	int switch_state = send_count % CAP_ENUM_SWITCH_SWITCH_VALUE_MAX;
	cap_switch_data->set_switch_value(cap_switch_data, caps_helper_switch.attr_switch.values[switch_state]);
	cap_switch_data->attr_switch_send(cap_switch_data);

	int formaldehyde_value = (send_count * 100000 % 1000000);
	cap_formaldehydeMeasurement_data->set_formaldehydeLevel_value(cap_formaldehydeMeasurement_data, formaldehyde_value);
	cap_formaldehydeMeasurement_data->attr_formaldehydeLevel_send(cap_formaldehydeMeasurement_data);

}

void app_main(void)
{
	/**
	  SmartThings Device SDK(STDK) aims to make it easier to develop IoT devices by providing
	  additional st_iot_core layer to the existing chip vendor SW Architecture.

	  That is, you can simply develop a basic application by just calling the APIs provided by st_iot_core layer
	  like below. st_iot_core currently offers 14 API.

	  //create a iot context
	  1. st_conn_init();

	  //create a handle to process capability
	  2. st_cap_handle_init();

	  //register a callback function to process capability command when it comes from the SmartThings Server.
	  3. st_cap_cmd_set_cb();

	  //needed when it is necessary to keep monitoring the device status
	  4. user_defined_task()

	  //process on-boarding procedure. There is nothing more to do on the app side than call the API.
	  5. st_conn_start();
	 */

	unsigned char *onboarding_config = (unsigned char *) onboarding_config_start;
	unsigned int onboarding_config_len = onboarding_config_end - onboarding_config_start;
	unsigned char *device_info = (unsigned char *) device_info_start;
	unsigned int device_info_len = device_info_end - device_info_start;
	IOT_CAP_HANDLE *handle = NULL;
	int iot_err;

	// 1. create a iot context
	ctx = st_conn_init(onboarding_config, onboarding_config_len, device_info, device_info_len);
	if (ctx != NULL) {
		iot_err = st_conn_set_noti_cb(ctx, iot_noti_cb, NULL);
		if (iot_err)
			printf("fail to set notification callback function\n");
	} else {
		printf("fail to create the iot_context\n");
	}


// CAPTEST : call initialize function and set init_cb
	cap_thermostatMode_data = caps_thermostatMode_initialize(ctx, "main", cap_thermostatMode_init_cb, NULL);
	cap_switchLevel_data = caps_switchLevel_initialize(ctx, "main", cap_switchLevel_init_cb, NULL);
	cap_threeAxis_data = caps_threeAxis_initialize(ctx, "main", cap_threeAxis_init_cb, NULL);
	cap_videoCapture_data = caps_videoCapture_initialize(ctx, "main", cap_videoCapture_init_cb, NULL);
	cap_colorControl_data = caps_colorControl_initialize(ctx, "main", cap_colorControl_init_cb, NULL);
	cap_switch_data = caps_switch_initialize(ctx, "main", cap_switch_init_cb, NULL);
	cap_formaldehydeMeasurement_data = caps_formaldehydeMeasurement_initialize(ctx, "main", cap_formaldehydeMeasurement_init_cb, NULL);

// CAPTEST : set cmd usr cb
	cap_thermostatMode_data->cmd_setThermostatMode_usr_cb = cap_thermostatMode_cmd_cb;
	cap_switchLevel_data->cmd_setLevel_usr_cb = cap_switchLevel_cmd_cb;
	cap_videoCapture_data->cmd_capture_usr_cb = cap_videoCapture_cmd_cb;
	cap_colorControl_data->cmd_setColor_usr_cb = cap_colorControl_cmd_cb;

	cap_switch_data->cmd_on_usr_cb = cap_switch_cmd_cb;
	cap_switch_data->cmd_off_usr_cb = cap_switch_cmd_cb;

	gpio_init();

	// 4. needed when it is necessary to keep monitoring the device status
	xTaskCreate(captest_task, "captest_task", 2048, (void *)handle, 10, NULL);

	// 5. process on-boarding procedure. There is nothing more to do on the app side than call the API.
	st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, NULL);

}
