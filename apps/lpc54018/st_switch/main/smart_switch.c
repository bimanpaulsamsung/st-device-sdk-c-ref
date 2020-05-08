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
#include <stdbool.h>
#include "st_dev.h"
#include "iot_debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "device_control.h"


enum led_animation_mode_list {
     LED_ANIMATION_MODE_IDLE = 0,
     LED_ANIMATION_MODE_FAST,
     LED_ANIMATION_MODE_SLOW,
};

enum notification_led_gpio_state {
	NOTIFICATION_LED_GPIO_OFF = 0,
	NOTIFICATION_LED_GPIO_ON = 1,
};

static int noti_led_mode = LED_ANIMATION_MODE_IDLE;

IOT_CTX *ctx = NULL;
IOT_CAP_HANDLE *handle = NULL;

static iot_status_t g_iot_status;
enum smartswitch_switch_onoff_state {
	SMARTSWITCH_SWITCH_OFF = 0,
	SMARTSWITCH_SWITCH_ON = 1,
};

enum button_gpio_state {
    BUTTON_GPIO_RELEASED = 1,
    BUTTON_GPIO_PRESSED = 0,
};

#define BUTTON_DEBOUNCE_TIME_MS 20
#define BUTTON_LONG_THRESHOLD_MS 5000
#define BUTTON_DELAY_MS 300

enum button_event_type {
     BUTTON_LONG_PRESS = 0,
     BUTTON_SHORT_PRESS = 1,
};
static int smartswitch_switch_state = SMARTSWITCH_SWITCH_ON;

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

static void led_state_switch(enum smartswitch_switch_onoff_state state)
{
	device_noti_led_control(state);
	smartswitch_switch_state = state;
}

void led_switch(enum smartswitch_switch_onoff_state state)
{
	const char * switch_str = (state == SMARTSWITCH_SWITCH_ON)? "on" : "off";
	IOT_EVENT *switch_evt;
	int32_t sequence_no;

	/* Setup switch state */
	switch_evt = st_cap_attr_create_string("switch", switch_str, NULL);

	/* Send switch off event */
	sequence_no = st_cap_attr_send(handle, 1, &switch_evt);
	st_cap_attr_free(switch_evt);
	if (sequence_no < 0) {
		printf("fail to send off_data\n");
	} else {
		printf( "Sequence number return : %d\r\n", sequence_no);
		led_state_switch(state);
	}
}

void led_blink(int delay, int count)
{
	for (int i = 0; i < count; i++) {
		device_noti_led_control(NOTIFICATION_LED_GPIO_OFF);
		vTaskDelay(delay / portTICK_PERIOD_MS);
		device_noti_led_control(NOTIFICATION_LED_GPIO_ON);
		vTaskDelay(delay / portTICK_PERIOD_MS);
	}
}

static void button_event(IOT_CAP_HANDLE *handle, uint32_t type, uint32_t count)
{
	if (type == BUTTON_SHORT_PRESS) {
		printf("Button short press, count: %d\r\n", count);
		switch(count) {
			case 1:
				if (g_iot_status == IOT_STATUS_NEED_INTERACT) {
					st_conn_ownership_confirm(ctx, true);
					device_noti_led_control(NOTIFICATION_LED_GPIO_OFF);
					noti_led_mode = LED_ANIMATION_MODE_IDLE;
				} else {
					/* change switch state and LED state */
					if (smartswitch_switch_state == SMARTSWITCH_SWITCH_ON) {
						led_state_switch(SMARTSWITCH_SWITCH_OFF);
						led_switch(SMARTSWITCH_SWITCH_OFF);
					} else {
						led_state_switch(SMARTSWITCH_SWITCH_ON);
						led_switch(SMARTSWITCH_SWITCH_ON);
					}
				}
				break;
			default:
				break;
		}
	} else if (type == BUTTON_LONG_PRESS) {
		printf("Button long press, count: %d\r\n", count);
		led_blink(100, 3);
		/* clean-up provisioning & registered data with reboot option*/
		st_conn_cleanup(ctx, true);
	}
}

static void iot_status_cb(iot_status_t status,
		iot_stat_lv_t stat_lv, void *usr_data)
{
	g_iot_status = status;
	switch(status)
	{
		case IOT_STATUS_NEED_INTERACT:
			noti_led_mode = LED_ANIMATION_MODE_FAST;
			break;
		case IOT_STATUS_IDLE:
		case IOT_STATUS_CONNECTING:
			noti_led_mode = LED_ANIMATION_MODE_IDLE;
			if (smartswitch_switch_state == SMARTSWITCH_SWITCH_ON) {
				device_noti_led_control(NOTIFICATION_LED_GPIO_ON);
			} else {
				device_noti_led_control(NOTIFICATION_LED_GPIO_OFF);
			}
			break;
		default:
			HIT();
			break;
	}
}

bool get_button_event(int* button_event_type, int* button_event_count)
{
	static uint32_t button_count = 0;
	static uint32_t button_last_state = BUTTON_GPIO_RELEASED;
	static TimeOut_t button_timeout;
	static TickType_t long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
	static TickType_t button_delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);

	uint32_t gpio_level = 0;

	gpio_level = device_button_read();
	if (button_last_state != gpio_level) {
		/* wait debounce time to ignore small ripple of currunt */
		vTaskDelay( pdMS_TO_TICKS(BUTTON_DEBOUNCE_TIME_MS) );
		gpio_level = device_button_read();
		if (button_last_state != gpio_level) {
			printf("Button event, val: %d, tick: %u\r\n", gpio_level, (uint32_t)xTaskGetTickCount());
			button_last_state = gpio_level;
			if (gpio_level == BUTTON_GPIO_PRESSED) {
				button_count++;
			}
			vTaskSetTimeOutState(&button_timeout);
			button_delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);
			long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
		}
	} else if (button_count > 0) {
		if ((gpio_level == BUTTON_GPIO_PRESSED)
				&& (xTaskCheckForTimeOut(&button_timeout, &long_press_tick ) != pdFALSE)) {
			*button_event_type = BUTTON_LONG_PRESS;
			*button_event_count = 1;
			button_count = 0;
			return true;
		} else if ((gpio_level == BUTTON_GPIO_RELEASED)
				&& (xTaskCheckForTimeOut(&button_timeout, &button_delay_tick ) != pdFALSE)) {
			*button_event_type = BUTTON_SHORT_PRESS;
			*button_event_count = button_count;
			button_count = 0;
			return true;
		}
	}
	return false;
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
		printf("fail to send init_data\n");

	printf("Sequence number return : %d\n", sequence_no);
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
	printf("Notification message received\n");

	if (noti_data->type == IOT_NOTI_TYPE_DEV_DELETED) {
		printf("[device deleted]\n");
	} else if (noti_data->type == IOT_NOTI_TYPE_RATE_LIMIT) {
		printf("[rate limit] Remaining time:%d, sequence number:%d\n",
			noti_data->raw.rate_limit.remainingTime, noti_data->raw.rate_limit.sequenceNumber);
	}
}
void change_led_state(int noti_led_mode)
{
	static uint32_t led_last_time_ms = 0;

	uint32_t now_ms = 0;
	uint32_t gpio_level = 0;

	now_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
	switch (noti_led_mode) {
		case LED_ANIMATION_MODE_IDLE:
			break;
		case LED_ANIMATION_MODE_SLOW:
			//gpio_level =  gpio_get_level(GPIO_OUTPUT_NOTIFICATION_LED);
			if ((gpio_level == NOTIFICATION_LED_GPIO_ON) && (now_ms - led_last_time_ms > 200)) {
				device_noti_led_control(NOTIFICATION_LED_GPIO_OFF);
				led_last_time_ms = now_ms;
			}
			if ((gpio_level == NOTIFICATION_LED_GPIO_OFF) && (now_ms - led_last_time_ms > 1000)) {
				device_noti_led_control(NOTIFICATION_LED_GPIO_ON);
				led_last_time_ms = now_ms;
			}
			break;
		case LED_ANIMATION_MODE_FAST:
			//gpio_level =  gpio_get_level(GPIO_OUTPUT_NOTIFICATION_LED);
			if ((gpio_level == NOTIFICATION_LED_GPIO_ON) && (now_ms - led_last_time_ms > 100)) {
				device_noti_led_control(NOTIFICATION_LED_GPIO_OFF);
				led_last_time_ms = now_ms;
			}
			if ((gpio_level == NOTIFICATION_LED_GPIO_OFF) && (now_ms - led_last_time_ms > 100)) {
				device_noti_led_control(NOTIFICATION_LED_GPIO_ON);
				led_last_time_ms = now_ms;
			}
			break;
		default:
			break;
	}
}

static void smartswitch_task(void *arg)
{
	IOT_CAP_HANDLE *handle = (IOT_CAP_HANDLE *)arg;

	int button_event_type = 0;
	int button_event_count = 0;

	for (;;) {
		if (get_button_event(&button_event_type, &button_event_count)) {
			button_event(handle, button_event_type, button_event_count);
		}
		if (noti_led_mode != LED_ANIMATION_MODE_IDLE) {
			change_led_state(noti_led_mode);
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
	int iot_err;

	printf("APP_MAIN_START >>PT<<\r\n");

	// 1. create a iot context
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
	//common api
	device_button_init();
	device_led_init();
	//default led state
	led_state_switch(SMARTSWITCH_SWITCH_ON);
	// 4. needed when it is necessary to keep monitoring the device status
	xTaskCreate(smartswitch_task, "smartswitch_task", 512, (void *)handle, 5, NULL);

	// 5. process on-boarding procedure. There is nothing more to do on the app side than call the API.
	st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, NULL);

	printf("IOT_CONNECT_DONE >>PT<<\r\n");
}
