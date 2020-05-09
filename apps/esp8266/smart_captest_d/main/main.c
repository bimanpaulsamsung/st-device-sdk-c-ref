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

// for command line interface(CLI)
#include "iot_uart_cli.h"
#include "iot_cli_cmd.h"

#include "caps_switch.h"

// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]	asm("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]	asm("_binary_onboarding_config_json_end");

// device_info_start is null-terminated string
extern const uint8_t device_info_start[]	asm("_binary_device_info_json_start");
extern const uint8_t device_info_end[]		asm("_binary_device_info_json_end");

static iot_status_t g_iot_status;
static iot_stat_lv_t g_iot_stat_lv;

IOT_CTX *ctx = NULL;

extern void send_test_capabilities();
extern void captest_prefix_d(IOT_CTX *ctx);

#ifdef CONFIG_SAMSUNG_RUN_TIME_STATS
#include "driver/hw_timer.h"
uint32_t ulHighFrequencyTimerTicks;
void hw_timer_callback(void *arg)
{
	    ulHighFrequencyTimerTicks++;
}

uint32_t get_TimerTicks(void)
{
	    return ulHighFrequencyTimerTicks;
}

void config_TimerTicks(void)
{
	    ulHighFrequencyTimerTicks = 0UL;
}
#endif

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

#ifdef SS_VER
	printf("SS version:%s(%06x)\n", SS_VER, STDK_VERSION_CODE);
#endif

#ifdef CONFIG_SAMSUNG_RUN_TIME_STATS
	hw_timer_init(hw_timer_callback, NULL);
	hw_timer_alarm_us(1000, true);
#endif

#ifdef CONFIG_SAMSUNG_BUILD_ENG
	setbuf(stdout, NULL);
	register_iot_cli_cmd();
	uart_cli_main();
#endif

	// 1. create a iot context
	ctx = st_conn_init(onboarding_config, onboarding_config_len, device_info, device_info_len);
	if (ctx != NULL) {
		iot_err = st_conn_set_noti_cb(ctx, iot_noti_cb, NULL);
		if (iot_err)
			printf("fail to set notification callback function\n");
	} else {
		printf("fail to create the iot_context\n");
	}

	captest_prefix_d(ctx);

	gpio_init();

	// 4. needed when it is necessary to keep monitoring the device status
	xTaskCreate(captest_task, "captest_task", 2048, (void *)handle, 10, NULL);

	// 5. process on-boarding procedure. There is nothing more to do on the app side than call the API.
	st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, NULL);

}
