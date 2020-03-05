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


#include "st_dev.h"

// for command line interface(CLI)
#include "iot_uart_cli.h"
#include "iot_cli_cmd.h"

// for CLI functions. This will depend on the features of the CLI functions you added.
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "nvs_flash.h"

#include "device_control.h"

// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]	asm("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]	asm("_binary_onboarding_config_json_end");

// device_info_start is null-terminated string
extern const uint8_t device_info_start[]	asm("_binary_device_info_json_start");
extern const uint8_t device_info_end[]		asm("_binary_device_info_json_end");

extern char* button_type[];
extern char* button_id[];

enum smartswitch_switch_onoff_state {
	SMARTSWITCH_SWITCH_OFF = 0,
	SMARTSWITCH_SWITCH_ON = 1,
};

char* my_buttons[22] =
{
	"pushed",
	"held",
	"double",
	"pushed_2x",
	"pushed_3x",

	"pushed_4x",
	"pushed_5x",
	"pushed_6x",
	"down",
	"down_2x",

	"down_3x",
	"down_4x",
	"down_5x",
	"down_6x",
	"down_hold",

	"up",
	"up_2x",
	"up_3x",
	"up_4x",
	"up_5x",

	"up_6x",
	"up_hold"
};

static IOT_CAP_HANDLE *handle_main = NULL;
static IOT_CAP_HANDLE *handle_btry = NULL;
static IOT_CAP_HANDLE *handle_mic = NULL;
static IOT_CAP_HANDLE *handle_volup = NULL;
static IOT_CAP_HANDLE *handle_voldn = NULL;
static IOT_CAP_HANDLE *handle_list[3] = {NULL};

static int32_t smartswitch_switch_state = SMARTSWITCH_SWITCH_ON;

static iot_status_t g_iot_status;

static int noti_led_mode = LED_ANIMATION_MODE_IDLE;

IOT_CTX *ctx = NULL;


static void change_switch_state(int32_t state)
{
	/* change state */
	smartswitch_switch_state = state;
	if(state == SMARTSWITCH_SWITCH_ON) {
		gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_ON);
	} else {
		gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);
	}
}

static void send_battery_cap_evt(IOT_CAP_HANDLE *handle)
{
	IOT_EVENT *battery_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	/* Setup battery state */
	battery_evt = st_cap_attr_create_int("battery", (get_adc()*100)/4095, NULL);

	/* Send battery event */
	sequence_no = st_cap_attr_send(handle, evt_num, &battery_evt);
	if (sequence_no < 0)
		printf("fail to send battery data\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(battery_evt);
}

static void send_switch_cap_evt(IOT_CAP_HANDLE *handle, int32_t state)
{
	IOT_EVENT *switch_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	/* Setup switch onoff state */
	if(state == SMARTSWITCH_SWITCH_ON) {
		switch_evt = st_cap_attr_create_string("switch", "on", NULL);
	} else {
		switch_evt = st_cap_attr_create_string("switch", "off", NULL);
	}

	/* Send switch onoff event */
	sequence_no = st_cap_attr_send(handle, evt_num, &switch_evt);
	if (sequence_no < 0)
		printf("fail to send switch onoff data\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(switch_evt);
}

void process_button(uint32_t id)
{
	static int index = 0;
	IOT_EVENT *cap_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	printf("%s is called\n", __func__);

	cap_evt = st_cap_attr_create_string("button", "pushed", NULL);

	printf("handle_button=%p, evt_num=%d, cap_evt=%p\n", handle_list[id-1], evt_num, *cap_evt);
	sequence_no = st_cap_attr_send(handle_list[id-1], evt_num, &cap_evt);

	if (sequence_no < 0)
		printf("fail to send data\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(cap_evt);

	printf("%s\n", __func__);

}


static void button_event(IOT_CAP_HANDLE *handle, uint32_t type, uint32_t count, uint32_t id)
{
	if (type == BUTTON_SHORT_PRESS) {
		printf("Button short press, count: %d\n", count);
		switch(count) {
			case 1:
				if (g_iot_status == IOT_STATUS_NEED_INTERACT) {
					st_conn_ownership_confirm(ctx, true);
					gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);
					noti_led_mode = LED_ANIMATION_MODE_IDLE;
				} else {
					if (0 != id){
						process_button(id);
					}else{
						/* change switch state and LED state */
						if (smartswitch_switch_state == SMARTSWITCH_SWITCH_ON) {
							change_switch_state(SMARTSWITCH_SWITCH_OFF);
							send_switch_cap_evt(handle, SMARTSWITCH_SWITCH_OFF);
						} else {
							change_switch_state(SMARTSWITCH_SWITCH_ON);
							send_switch_cap_evt(handle, SMARTSWITCH_SWITCH_ON);
						}
					}
				}
				break;

			default:
				led_blink(GPIO_OUTPUT_LED_RED, 100, count);
				break;
		}
	} else if (type == BUTTON_LONG_PRESS) {
		printf("Button long press, count: %d\n", count);
		led_blink(GPIO_OUTPUT_LED_RED, 100, 3);
		/* clean-up provisioning & registered data with reboot option*/
		st_conn_cleanup(ctx, true);
	}
}

static void iot_status_cb(iot_status_t status,
		iot_stat_lv_t stat_lv, void *usr_data)
{
	g_iot_status = status;
	printf("iot_status: %d, lv: %d\n", status, stat_lv);

	switch(status)
	{
		case IOT_STATUS_NEED_INTERACT:
			noti_led_mode = LED_ANIMATION_MODE_FAST;
			break;
		case IOT_STATUS_IDLE:
		case IOT_STATUS_CONNECTING:
			noti_led_mode = LED_ANIMATION_MODE_IDLE;
			if (smartswitch_switch_state == SMARTSWITCH_SWITCH_ON) {
				gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_ON);
			} else {
				gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);
			}
			break;
		default:
			break;
	}
}

void cap_battery_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
	IOT_EVENT *init_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	/* Setup battery state */
	init_evt = st_cap_attr_create_int("battery", (get_adc()*100)/4095, NULL);

	/* Send battery event */
	sequence_no = st_cap_attr_send(handle, evt_num, &init_evt);
	if (sequence_no < 0)
		printf("fail to send init_data\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(init_evt);
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
	IOT_EVENT *off_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	printf("called [%s] func with : num_args:%u\n",
		__func__, cmd_data->num_args);

	change_switch_state(SMARTSWITCH_SWITCH_OFF);

	/* Setup switch off state */
	off_evt = st_cap_attr_create_string("switch", "off", NULL);

	/* Send switch off event */
	sequence_no = st_cap_attr_send(handle, evt_num, &off_evt);
	if (sequence_no < 0)
		printf("fail to send off_data\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(off_evt);
}

void cap_switch_cmd_on_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	IOT_EVENT *on_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	printf("called [%s] func with : num_args:%u\n",
		__func__, cmd_data->num_args);

	change_switch_state(SMARTSWITCH_SWITCH_ON);

	/* Setup switch on state */
	on_evt = st_cap_attr_create_string("switch", "on", NULL);

	/* Send switch on event */
	sequence_no = st_cap_attr_send(handle, evt_num, &on_evt);
	if (sequence_no < 0)
		printf("fail to send on_data\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(on_evt);
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

static void button_task(void *arg)
{
	IOT_CAP_HANDLE *handle = (IOT_CAP_HANDLE *)arg;

	int button_event_type;
	int button_event_count;
	int button_event_id;

	for (;;) {
		if (get_button_event(&button_event_type, &button_event_count, &button_event_id)) {
			printf("The %s is %s, the count is %d\n",button_id[button_event_id], button_type[button_event_type], button_event_count);
			button_event(handle, button_event_type, button_event_count,button_event_id);
		}

		if (noti_led_mode != LED_ANIMATION_MODE_IDLE) {
			change_led_state(noti_led_mode);
		}

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

static void led_task(void *arg)
{
	for(;;){
		gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_ON);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);

		gpio_set_level(GPIO_OUTPUT_LED_BLUE, LED_GPIO_ON);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(GPIO_OUTPUT_LED_BLUE, LED_GPIO_OFF);

		gpio_set_level(GPIO_OUTPUT_LED_GREEN, LED_GPIO_ON);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(GPIO_OUTPUT_LED_GREEN, LED_GPIO_OFF);
	}
}

static void adc_task(void *arg)
{
	for(;;){
		vTaskDelay(5000 / portTICK_PERIOD_MS);
		send_battery_cap_evt(handle_btry);
	}
}

void cap_button_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
	IOT_EVENT *cap_evt[2];
	uint8_t evt_num = 2;
	int32_t sequence_no;

	printf("%s is called\n", __func__);

	cap_evt[0] = st_cap_attr_create_number("numberOfButtons", 22, NULL);
	cap_evt[1] = st_cap_attr_create_string_array("supportedButtonValues", 22, my_buttons, NULL);

	sequence_no = st_cap_attr_send(handle, evt_num, cap_evt);
	if (sequence_no < 0)
		printf("fail to send data\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(cap_evt[0]);
	st_cap_attr_free(cap_evt[1]);



	printf("%s\n", __func__);
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

#ifdef SS_VER
	printf("SS version:%s(%06x)\n", SS_VER, STDK_VERSION_CODE);
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

		handle_main = st_cap_handle_init(ctx, "main", "switch", cap_switch_init_cb, NULL);
		iot_err = st_cap_cmd_set_cb(handle_main, "off", cap_switch_cmd_off_cb, NULL);
		if (iot_err)
			printf("fail to set cmd_cb for off\n");
		iot_err = st_cap_cmd_set_cb(handle_main, "on", cap_switch_cmd_on_cb, NULL);
                if (iot_err)
                        printf("fail to set cmd_cb for on\n");

		handle_btry = st_cap_handle_init(ctx, "main", "battery", cap_battery_init_cb, NULL);
		handle_mic = st_cap_handle_init(ctx, "mic", "button",cap_button_init_cb,NULL);
		handle_volup = st_cap_handle_init(ctx, "VolUp", "button",cap_button_init_cb,NULL);
		handle_voldn = st_cap_handle_init(ctx, "VolDn", "button",cap_button_init_cb,NULL);

		handle_list[0] = handle_mic;
		handle_list[1] = handle_volup;
		handle_list[2] = handle_voldn;
	} else {
		printf("fail to create the iot_context\n");
	}

	gpio_init();

	// 4. needed when it is necessary to keep monitoring the device status
	xTaskCreate(button_task, "button_task", 4096, (void *)handle_main, 10, NULL);
	xTaskCreate(adc_task, "adc_task", 4096, (void *)handle_btry, 10, NULL);
//	xTaskCreate(led_task, "led_task", 4096, (void *)handle, 10, NULL);

	// 5. process on-boarding procedure. There is nothing more to do on the app side than call the API.
	st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, NULL);

}
