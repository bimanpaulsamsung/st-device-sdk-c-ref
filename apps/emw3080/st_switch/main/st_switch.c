/* ***************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdbool.h>
#include "st_dev.h"
#include "iot_common.h"
#include "mico.h"
#include "iot_debug.h"

static iot_status_t g_iot_status;
IOT_CTX *ctx = NULL;
IOT_CAP_HANDLE *handle = NULL;

enum smartswitch_switch_onoff_state {
	SMARTSWITCH_SWITCH_OFF = 0,
	SMARTSWITCH_SWITCH_ON = 1,
};

enum button_gpio_state {
    BUTTON_GPIO_RELEASED = 1,
    BUTTON_GPIO_PRESSED = 0,
};

enum button_event_type {
     BUTTON_LONG_PRESS = 0,
     BUTTON_SHORT_PRESS = 1,
};

static int smartswitch_switch_state = SMARTSWITCH_SWITCH_ON;
#define BUTTON_DEBOUNCE_TIME_MS 20
#define BUTTON_LONG_THRESHOLD_MS 5000
#define BUTTON_DELAY_MS 300

#define GPIO_OUTPUT_COLORLED_0 MICO_GPIO_2
#define GPIO_OUTPUT_NOTIFICATION_LED MICO_GPIO_7
#define GPIO_INPUT_BUTTON MICO_GPIO_1

// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]	asm("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]	asm("_binary_onboarding_config_json_end");
extern unsigned int _binary_onboarding_config_json_size;
// device_info_start is null-terminated string
extern const uint8_t device_info_start[]	asm("_binary_device_info_json_start");
extern const uint8_t device_info_end[]		asm("_binary_device_info_json_end");
extern unsigned int _binary_device_info_json_size;

int led_status(void)
{
	return smartswitch_switch_state;
}

static void notification_switch(enum smartswitch_switch_onoff_state state)
{
	if (state == SMARTSWITCH_SWITCH_ON)
		MicoGpioOutputHigh(GPIO_OUTPUT_NOTIFICATION_LED);
	else
		MicoGpioOutputLow(GPIO_OUTPUT_NOTIFICATION_LED);
}

static void led_state_switch(enum smartswitch_switch_onoff_state state)
{
	if (state == SMARTSWITCH_SWITCH_ON)
		MicoGpioOutputLow(GPIO_OUTPUT_COLORLED_0);
	else
		MicoGpioOutputHigh(GPIO_OUTPUT_COLORLED_0);
	smartswitch_switch_state = state;
}

void led_switch(enum smartswitch_switch_onoff_state state)
{
	const char * switch_str = (state == SMARTSWITCH_SWITCH_ON) ? "on" : "off";
	IOT_EVENT *switch_evt;
	int32_t sequence_no;

	led_state_switch(state);

	/* Setup switch state */
	switch_evt = st_cap_attr_create_string("switch", switch_str, NULL);

	/* Send switch off event */
	sequence_no = st_cap_attr_send(handle, 1, &switch_evt);
	st_cap_attr_free(switch_evt);
	if (sequence_no < 0) {
		printf("fail to send off_data\r\n");
	} else {
		printf( "Sequence number return : %d\r\n", sequence_no);
	}
}

static void led_blink(int gpio, int delay, int count)
{
	for (int i = 0; i < count; i++) {
		notification_switch(SMARTSWITCH_SWITCH_ON);
		iot_os_delay(delay);
		notification_switch(SMARTSWITCH_SWITCH_OFF);
		iot_os_delay(delay);
	}
}

static void button_event(IOT_CAP_HANDLE *handle, uint32_t type, uint32_t count)
{
	if (type == BUTTON_SHORT_PRESS) {
		printf("Button short press, count: %d\r\n", count);
		switch(count) {
			case 1:
				if (g_iot_status == IOT_STATUS_NEED_INTERACT){
					st_conn_ownership_confirm(ctx,true);
					MicoGpioOutputLow(GPIO_OUTPUT_NOTIFICATION_LED);
				} else{
					/* change switch state and LED state */
					if (smartswitch_switch_state == SMARTSWITCH_SWITCH_ON) {
						led_switch(SMARTSWITCH_SWITCH_OFF);
					} else {
						led_switch(SMARTSWITCH_SWITCH_ON);
					}
				}
				break;
			default:
				led_blink(GPIO_OUTPUT_NOTIFICATION_LED, 100, count);
				break;
		}
	} else if (type == BUTTON_LONG_PRESS) {
		printf("Button long press, count: %d\r\n", count);
		led_blink(GPIO_OUTPUT_NOTIFICATION_LED, 100, 3);
		/*clean-up provisioning & registered data with reboot option*/
		st_conn_cleanup(ctx, true);
	}
}

static void iot_status_cb(iot_status_t status,
		iot_stat_lv_t stat_lv, void *usr_data)
{
	g_iot_status = status;
	printf("iot_status: %d, lv: %d\r\n",status, stat_lv);
	switch(status)
	{
		case IOT_STATUS_NEED_INTERACT:
			break;
		case IOT_STATUS_IDLE:
		case IOT_STATUS_CONNECTING:
			if (smartswitch_switch_state == SMARTSWITCH_SWITCH_ON) {
				led_state_switch(SMARTSWITCH_SWITCH_ON);
			} else {
				led_state_switch(SMARTSWITCH_SWITCH_OFF);
			}
			break;
		default:
			break;
	}
}

static int _check_timeout(mico_time_t *const ptime_out, mico_time_t *const pms_to_wait)
{
	int ret;

	/* Minor optimisation.  The tick count cannot change in this block. */
	const mico_time_t const_tick_count = mico_rtos_get_time();
	const mico_time_t elapsed_time = const_tick_count - *ptime_out;

	if (elapsed_time < *pms_to_wait ) {
		/* Not a genuine timeout. Adjust parameters for time remaining. */
		*pms_to_wait -= elapsed_time;
		*ptime_out = mico_rtos_get_time();
		ret = 0;
	} else {
		*pms_to_wait = 0;
		ret = 1;
	}

	return ret;
}

bool get_button_event(int* button_event_type, int* button_event_count)
{
	static uint32_t button_count = 0;
	static uint32_t button_last_state = BUTTON_GPIO_RELEASED;
	static unsigned int button_timeout;
	static unsigned int long_press_tick = BUTTON_LONG_THRESHOLD_MS;
	static unsigned int button_delay_tick = BUTTON_DELAY_MS;

	uint32_t gpio_level = 0;

	gpio_level = MicoGpioInputGet(GPIO_INPUT_BUTTON);
	if (button_last_state != gpio_level) {
		/* wait debounce time to ignore small ripple of currunt */
		iot_os_delay(BUTTON_DEBOUNCE_TIME_MS);
		gpio_level = MicoGpioInputGet(GPIO_INPUT_BUTTON);
		if (button_last_state != gpio_level) {
			button_last_state = gpio_level;
			if (gpio_level == BUTTON_GPIO_PRESSED) {
				button_count++;
			}
			button_timeout = mico_rtos_get_time();
			button_delay_tick = BUTTON_DELAY_MS;
			long_press_tick = BUTTON_LONG_THRESHOLD_MS;
			printf("Button event, val: %d, time: %u\r\n", gpio_level, button_timeout);
		}
	} else if (button_count > 0) {
		if ((gpio_level == BUTTON_GPIO_PRESSED)
				&& (_check_timeout(&button_timeout, &long_press_tick ) == 1)) {
			*button_event_type = BUTTON_LONG_PRESS;
			*button_event_count = 1;
			button_count = 0;
			return true;
		} else if ((gpio_level == BUTTON_GPIO_RELEASED)
				&& (_check_timeout(&button_timeout, &button_delay_tick ) == 1)) {
			*button_event_type = BUTTON_SHORT_PRESS;
			*button_event_count = button_count;
			button_count = 0;
			return true;
		}
	}
	return false;
}

void led_button_init(void)
{
	//COLORLED 0 init
	MicoGpioInitialize(GPIO_OUTPUT_COLORLED_0, OUTPUT_PUSH_PULL);
	MicoGpioOutputHigh(GPIO_OUTPUT_COLORLED_0);

	//notify led init
	MicoGpioInitialize(GPIO_OUTPUT_NOTIFICATION_LED, OUTPUT_PUSH_PULL);
	MicoGpioOutputHigh(GPIO_OUTPUT_NOTIFICATION_LED);
	notification_switch(SMARTSWITCH_SWITCH_ON);

	//button init
	MicoGpioInitialize(GPIO_INPUT_BUTTON, INPUT_PULL_UP);
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
		printf("fail to send init_data\r\n");

	printf("Sequence number return : %d\r\n", sequence_no);
	st_cap_attr_free(init_evt);
}

void cap_switch_cmd_off_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	led_switch(SMARTSWITCH_SWITCH_OFF);
}

void cap_switch_cmd_on_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	led_switch(SMARTSWITCH_SWITCH_ON);
}

void iot_noti_cb(iot_noti_data_t *noti_data, void *noti_usr_data)
{
	printf("Notification message received\r\n");

	if (noti_data->type == IOT_NOTI_TYPE_DEV_DELETED) {
		printf("[device deleted]\r\n");
	} else if (noti_data->type == IOT_NOTI_TYPE_RATE_LIMIT) {
		printf("[rate limit] Remaining time:%d, sequence number:%d\r\n",
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

		iot_os_delay(10);
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
	int iot_err;

	printf("APP_MAIN_START >>PT<<\r\n");
	// 1. create an iot context
	ctx = st_conn_init(onboarding_config, onboarding_config_len, device_info, device_info_len);
	if (ctx != NULL) {
		iot_err = st_conn_set_noti_cb(ctx, iot_noti_cb, NULL);
		if (iot_err)
			printf("fail to set notification callback function\r\n");

	// 2. create a handle to process capability
	//	implement init_callback function (cap_switch_init_cb)
		handle = st_cap_handle_init(ctx, "main", "switch", cap_switch_init_cb, NULL);

	// 3. register a callback function to process capability command when it comes from the SmartThings Server
	//	implement callback function (cap_switch_cmd_off_cb)
		iot_err = st_cap_cmd_set_cb(handle, "off", cap_switch_cmd_off_cb, NULL);
		if (iot_err)
			printf("fail to set cmd_cb for off\r\n");

	//	implement callback function (cap_switch_cmd_on_cb)
		iot_err = st_cap_cmd_set_cb(handle, "on", cap_switch_cmd_on_cb, NULL);
		if (iot_err)
			printf("fail to set cmd_cb for on\r\n");

	} else {
		printf("fail to create the iot_context\r\n");
	}

	printf("IOT_INIT_DONE >>PT<<\r\n");

	led_button_init();
	// 4. needed when it is necessary to keep monitoring the device status
	iot_os_thread_create(smartswitch_task, "smartswitch_task", 2048, (void *)handle, 10, NULL);
	// 5. process on-boarding procedure. There is nothing more to do on the app side than call the API.
	st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, NULL);
	printf("IOT_CONNECT_DONE >>PT<<\r\n");
}