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
#include "st_dev.h"
#include "iot_debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "device_control.h"

#define NOTIFY_LED_IDX	3
// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]	__asm__("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]	__asm__("_binary_onboarding_config_json_end");

// device_info_start is null-terminated string
extern const uint8_t device_info_start[]	__asm__("_binary_device_info_json_start");
extern const uint8_t device_info_end[]		__asm__("_binary_device_info_json_end");

enum switch_onoff_state {
	SWITCH_OFF = 0,
	SWITCH_ON = 1,
};

extern bool get_button_event(int* button_event_type, int* button_event_count);

static int switch_state = SWITCH_ON;
static iot_status_t g_iot_status;

static int noti_led_mode = LED_ANIMATION_MODE_IDLE;

IOT_CTX* ctx = NULL;

void noti_led_onoff(int32_t state)
{
	if (state == 0)
		device_led_control(NOTIFY_LED_IDX, 0);
	else
		device_led_control(NOTIFY_LED_IDX, 1);
}

static void change_switch_state(int32_t state)
{
	/* change state */
	switch_state = state;
	noti_led_onoff(switch_state);
	if(state == SWITCH_ON) {
		device_led_control(NOTIFY_LED_IDX, 1);
		noti_led_onoff(1);
	} else {
		device_led_control(NOTIFY_LED_IDX, 0);
		noti_led_onoff(0);
	}
}

static void send_switch_cap_evt(IOT_CAP_HANDLE *handle, int32_t state)
{
	IOT_EVENT *switch_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	/* Setup switch onoff state */
	if(state == SWITCH_ON) {
		switch_evt = st_cap_attr_create_string("switch", "on", NULL);
	} else {
		switch_evt = st_cap_attr_create_string("switch", "off", NULL);
	}

	/* Send switch onoff event */
	sequence_no = st_cap_attr_send(handle, evt_num, &switch_evt);
	if (sequence_no < 0)
		IOT_ERROR("fail to send switch onoff data");

	IOT_INFO("Sequence number return : %d", sequence_no);
	st_cap_attr_free(switch_evt);
}

static void button_event(IOT_CAP_HANDLE *handle, uint32_t type, uint32_t count)
{
	if (type == BUTTON_SHORT_PRESS) {
		IOT_INFO("Button short press, count: %d\n", count);
		switch(count) {
			case 1:
				if (g_iot_status == IOT_STATUS_NEED_INTERACT) {
					st_conn_ownership_confirm(ctx, true);
					device_led_control(NOTIFY_LED_IDX, 0);
					noti_led_mode = LED_ANIMATION_MODE_IDLE;
				} else {
					/* change switch state and LED state */
					if (switch_state == SWITCH_ON) {
						change_switch_state(SWITCH_OFF);
						send_switch_cap_evt(handle, SWITCH_OFF);
					} else {
						change_switch_state(SWITCH_ON);
						send_switch_cap_evt(handle, SWITCH_ON);
					}
				}
				break;

			default:
				led_blink(NOTIFY_LED_IDX, 100, count);
				break;
		}
	} else if (type == BUTTON_LONG_PRESS) {
		IOT_INFO("Button long press, count: %d\n", count);
		led_blink(NOTIFY_LED_IDX, 100, 3);
		/* clean-up provisioning & registered data with reboot option*/
		st_conn_cleanup(ctx, true);
	}
}

static void iot_status_cb(iot_status_t status,
		iot_stat_lv_t stat_lv, void *usr_data)
{
	g_iot_status = status;
	IOT_INFO("iot_status: %d, lv: %d\n", status, stat_lv);

	switch(status)
	{
		case IOT_STATUS_NEED_INTERACT:
			noti_led_mode = LED_ANIMATION_MODE_FAST;
			break;
		case IOT_STATUS_IDLE:
		case IOT_STATUS_CONNECTING:
			noti_led_mode = LED_ANIMATION_MODE_IDLE;
			if (switch_state == SWITCH_ON) {
				device_led_control(NOTIFY_LED_IDX, 1);
			} else {
				device_led_control(NOTIFY_LED_IDX, 0);
			}
			break;
		default:
			break;
	}
}

void cap_switch_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
	IOT_EVENT *init_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	/* Setup switch on state */
	init_evt = st_cap_attr_create_string("switch", "on", NULL);

	/* Send switch on event */
	sequence_no = st_cap_attr_send(handle, evt_num, &init_evt);
	if (sequence_no < 0)
		IOT_ERROR("fail to send init_data");

	IOT_INFO("Sequence number return : %d", sequence_no);
	st_cap_attr_free(init_evt);
}

void cap_switch_cmd_off_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	IOT_EVENT *off_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	IOT_INFO("called [%s] func with : num_args:%u",
		__func__, cmd_data->num_args);

	change_switch_state(SWITCH_OFF);

	/* Setup switch off state */
	off_evt = st_cap_attr_create_string("switch", "off", NULL);

	/* Send switch off event */
	sequence_no = st_cap_attr_send(handle, evt_num, &off_evt);
	if (sequence_no < 0)
		IOT_ERROR("fail to send off_data");

	IOT_INFO("Sequence number return : %d", sequence_no);
	st_cap_attr_free(off_evt);
}


void cap_switch_cmd_on_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	IOT_EVENT *on_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	IOT_INFO("called [%s] func with : num_args:%u",
		__func__, cmd_data->num_args);

	change_switch_state(SWITCH_ON);

	/* Setup switch on state */
	on_evt = st_cap_attr_create_string("switch", "on", NULL);

	/* Send switch on event */
	sequence_no = st_cap_attr_send(handle, evt_num, &on_evt);
	if (sequence_no < 0)
		IOT_ERROR("fail to send on_data");

	IOT_INFO("Sequence number return : %d", sequence_no);
	st_cap_attr_free(on_evt);
}

void iot_noti_cb(iot_noti_data_t *noti_data, void *noti_usr_data)
{
	IOT_INFO("Notification message received");

	if (noti_data->type == IOT_NOTI_TYPE_DEV_DELETED) {
		IOT_INFO("[device deleted]");
	} else if (noti_data->type == IOT_NOTI_TYPE_RATE_LIMIT) {
		IOT_INFO("[rate limit] Remaining time:%d, sequence number:%d",
			noti_data->raw.rate_limit.remainingTime, noti_data->raw.rate_limit.sequenceNumber);
	}
}

static void smartswitch_task(void *arg)
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
	IOT_CAP_HANDLE* switch_handle = NULL;
	IOT_CAP_HANDLE* color_handle = NULL;
	IOT_CAP_HANDLE* level_handle = NULL;
	int iot_err;

#ifdef CONFIG_SAMSUNG_BUILD_ENG
	app_run_shell();
#endif
	// 1. create a iot context
	ctx = st_conn_init(onboarding_config, onboarding_config_len, device_info, device_info_len);
	if (ctx != NULL) {
		iot_err = st_conn_set_noti_cb(ctx, iot_noti_cb, NULL);
		if (iot_err)
			IOT_ERROR("fail to set notification callback function");

	// 2. create a handle to process capability
	//	implement init_callback function
		switch_handle = st_cap_handle_init(ctx, "main", "switch", cap_switch_init_cb, NULL);
	// 3. register a callback function to process capability command when it comes from the SmartThings Server
	//	implement callback function
		iot_err = st_cap_cmd_set_cb(switch_handle, "off", cap_switch_cmd_off_cb, NULL);
		if (iot_err)
			IOT_ERROR("fail to set cmd_cb for off");
		iot_err = st_cap_cmd_set_cb(switch_handle, "on", cap_switch_cmd_on_cb, NULL);
		if (iot_err)
			IOT_ERROR("fail to set cmd_cb for on");

	} else {
		IOT_ERROR("fail to create the iot_context");
	}

	device_led_init();
	device_button_pin_init();
	noti_led_onoff(switch_state);

	// 4. needed when it is necessary to keep monitoring the device status
	xTaskCreate(smartswitch_task, "smartswitch_task", 2048, (void *)switch_handle, 10, NULL);

	// 5. process on-boarding procedure. There is nothing more to do on the app side than call the API.
	st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, NULL);
}