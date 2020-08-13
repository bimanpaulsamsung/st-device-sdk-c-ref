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
//for implementing main features of IoT device
#include "st_dev.h"
#include "iot_debug.h"
#include "mico.h"

#define PWM_LEVEL 10

#define LED_BLINK_TIME 50
#define BUTTON_DEBOUNCE_TIME_MS 20
#define BUTTON_LONG_THRESHOLD_MS 5000
#define BUTTON_DELAY_MS 300

#define GPIO_INPUT_BUTTON MICO_GPIO_1     //SWD CLD

#define GPIO_OUTPUT_NOTIFICATION_LED MICO_GPIO_12 //actually not used on WIFI-EXT, no more available gpio
#define GPIO_OUTPUT_RED_LED    MICO_GPIO_2    //SWD IO
#define GPIO_OUTPUT_GREEN_LED  MICO_GPIO_7	  //RTS
#define GPIO_OUTPUT_BLUE_LED   MICO_GPIO_8    //CTS

// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]	asm("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]	asm("_binary_onboarding_config_json_end");

// device_info_start is null-terminated string
extern const uint8_t device_info_start[]	asm("_binary_device_info_json_start");
extern const uint8_t device_info_end[]		asm("_binary_device_info_json_end");

enum notification_led_gpio_state {
	NOTIFICATION_LED_GPIO_ON = 0,
	NOTIFICATION_LED_GPIO_OFF = 1,
};

enum smartlamp_switch_onoff_state {
	SMARTLAMP_SWITCH_OFF = 0,
	SMARTLAMP_SWITCH_ON = 1,
};

enum button_gpio_state {
	BUTTON_GPIO_RELEASED = 1,
	BUTTON_GPIO_PRESSED = 0,
};

enum led_animation_mode_list {
	LED_ANIMATION_MODE_IDLE = 0,
	LED_ANIMATION_MODE_FAST,
	LED_ANIMATION_MODE_SLOW,
};

enum button_event_type {
	BUTTON_LONG_PRESS = 0,
	BUTTON_SHORT_PRESS = 1,
};

static int smartlamp_switch_state = SMARTLAMP_SWITCH_ON;
static double smartlamp_color_hue = 0;
static double smartlamp_color_saturation = 100;
static int smartlamp_color_level = 50;
static int smartlamp_color_red = 255;
static int smartlamp_color_green = 255;
static int smartlamp_color_blue = 255;
static int smartlamp_brightness_level = 100;

static iot_status_t g_iot_status;
static int noti_led_mode = LED_ANIMATION_MODE_IDLE;
IOT_CTX* ctx = NULL;

static mico_timer_t led_pwm_timer;
#define PwmRestoreDefaultTimeOut 1

static float calculate_rgb(float v1, float v2, float vh)
{
	if (vh < 0) vh += 1;
	if (vh > 1) vh -= 1;

	if ((6 * vh) < 1)
		return (v1 + (v2 - v1) * 6 * vh);

	if ((2 * vh) < 1)
		return v2;

	if ((3 * vh) < 2)
		return (v1 + (v2 - v1) * ((2.0f / 3) - vh) * 6);

	return v1;
}

/* SmartThings manage color by using Hue-Saturation format,
   If you use LED by using RGB color format, you need to change color format */
static void update_rgb_from_hsl(double hue, double saturation, int level,
		int *red, int *green, int *blue)
{
	if (saturation == 0)
	{
		*red = *green = *blue = 255;
		return;
	}

	float v1, v2;
	float h = ((float) hue) / 100;
	float s = ((float) saturation) / 100;
	float l = ((float) level) / 100;

	if (l < 0.5) {
		v2 = l * (1 + s);
	} else {
		v2 = l + s - l * s;
	}

	v1 = 2 * l - v2;

	*red   = (int)(255 * calculate_rgb(v1, v2, h + (1.0f / 3)));
	*green = (int)(255 * calculate_rgb(v1, v2, h));
	*blue  = (int)(255 * calculate_rgb(v1, v2, h - (1.0f / 3)));
}

/* Send integer type capability to SmartThings Sever */
static void send_capability_integer(IOT_CAP_HANDLE *handle, char* attribute_name, int value)
{
	IOT_EVENT *cap_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	cap_evt = st_cap_attr_create_int(attribute_name, value, NULL);

	sequence_no = st_cap_attr_send(handle, evt_num, &cap_evt);
	if (sequence_no < 0)
		printf("fail to send data\r\n");

	printf("Sequence number return : %d\r\n", sequence_no);
	st_cap_attr_free(cap_evt);
}

static void send_color_capability(IOT_CAP_HANDLE *handle, double hue, double saturation)
{
	IOT_EVENT *cap_evt[2];
	uint8_t evt_num = 2;
	int32_t sequence_no;

	cap_evt[0] = st_cap_attr_create_number("hue", hue, NULL);
	cap_evt[1] = st_cap_attr_create_number("saturation", saturation, NULL);

	sequence_no = st_cap_attr_send(handle, evt_num, cap_evt);
	if (sequence_no < 0)
		printf("fail to send data\r\n");

	printf("Sequence number return : %d\r\n", sequence_no);
	st_cap_attr_free(cap_evt[0]);
	st_cap_attr_free(cap_evt[1]);

}

static void color_led_onoff(int32_t state)
{
	if (state == 0) {
		MicoGpioOutputHigh(GPIO_OUTPUT_RED_LED);
		MicoGpioOutputHigh(GPIO_OUTPUT_GREEN_LED);
		MicoGpioOutputHigh(GPIO_OUTPUT_BLUE_LED);
	} else {
		if (smartlamp_color_red > 127)
			MicoGpioOutputLow(GPIO_OUTPUT_RED_LED);
		else
			MicoGpioOutputHigh(GPIO_OUTPUT_RED_LED);

		if (smartlamp_color_green > 127)
			MicoGpioOutputLow(GPIO_OUTPUT_GREEN_LED);
		else
			MicoGpioOutputHigh(GPIO_OUTPUT_GREEN_LED);

		if (smartlamp_color_blue > 127)
			MicoGpioOutputLow(GPIO_OUTPUT_BLUE_LED);
		else
			MicoGpioOutputHigh(GPIO_OUTPUT_BLUE_LED);
	}
}

static void change_switch_state(int32_t state)
{
	/* change state */
	smartlamp_switch_state = state;
	color_led_onoff(smartlamp_switch_state);
	if(state == SMARTLAMP_SWITCH_ON) {
		MicoGpioOutputHigh(GPIO_OUTPUT_NOTIFICATION_LED);
		color_led_onoff(1);
	} else {
		MicoGpioOutputLow(GPIO_OUTPUT_NOTIFICATION_LED);
		color_led_onoff(0);
	}
}

static void send_switch_cap_evt(IOT_CAP_HANDLE *handle, int32_t state)
{
	IOT_EVENT *switch_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	/* Setup switch onoff state */
	if(state == SMARTLAMP_SWITCH_ON) {
		switch_evt = st_cap_attr_create_string("switch", "on", NULL);
	} else {
		switch_evt = st_cap_attr_create_string("switch", "off", NULL);
	}

	/* Send switch onoff event */
	sequence_no = st_cap_attr_send(handle, evt_num, &switch_evt);
	if (sequence_no < 0)
		printf("fail to send switch onoff data\r\n");

	printf("Sequence number return : %d\r\n", sequence_no);
	st_cap_attr_free(switch_evt);
}

static void led_blink(int gpio, int delay, int count)
{
	for (int i = 0; i < count; i++) {
		change_switch_state(SMARTLAMP_SWITCH_ON);
		iot_os_delay(delay);
		change_switch_state(SMARTLAMP_SWITCH_OFF);
		iot_os_delay(delay);
	}
}

static void change_led_state(int noti_led_mode)
{
	static uint32_t led_last_time_ms = 0;

	uint32_t now_ms = 0;
	uint32_t gpio_level = 0;

	now_ms = mico_rtos_get_time();
	switch (noti_led_mode)
	{
		case LED_ANIMATION_MODE_IDLE:
			break;
		case LED_ANIMATION_MODE_SLOW:
			if ((gpio_level == NOTIFICATION_LED_GPIO_ON) && (now_ms - led_last_time_ms > 200)) {
				MicoGpioOutputLow(GPIO_OUTPUT_NOTIFICATION_LED);
				led_last_time_ms = now_ms;
			}
			if ((gpio_level == NOTIFICATION_LED_GPIO_OFF) && (now_ms - led_last_time_ms > 1000)) {
				MicoGpioOutputHigh(GPIO_OUTPUT_NOTIFICATION_LED);
				led_last_time_ms = now_ms;
			}
			break;
		case LED_ANIMATION_MODE_FAST:
			if ((gpio_level == NOTIFICATION_LED_GPIO_ON) && (now_ms - led_last_time_ms > 100)) {
				MicoGpioOutputLow(GPIO_OUTPUT_NOTIFICATION_LED);
				led_last_time_ms = now_ms;
			}
			if ((gpio_level == NOTIFICATION_LED_GPIO_OFF) && (now_ms - led_last_time_ms > 100)) {
				MicoGpioOutputHigh(GPIO_OUTPUT_NOTIFICATION_LED);
				led_last_time_ms = now_ms;
			}
			break;
		default:
			break;
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
					MicoGpioOutputLow(GPIO_OUTPUT_NOTIFICATION_LED);
					noti_led_mode = LED_ANIMATION_MODE_IDLE;
				} else {
					/* change switch state and LED state */
					if (smartlamp_switch_state == SMARTLAMP_SWITCH_ON) {
						change_switch_state(SMARTLAMP_SWITCH_OFF);
						send_switch_cap_evt(handle, SMARTLAMP_SWITCH_OFF);
					} else {
						change_switch_state(SMARTLAMP_SWITCH_ON);
						send_switch_cap_evt(handle, SMARTLAMP_SWITCH_ON);
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
		/* clean-up provisioning & registered data with reboot option*/
		st_conn_cleanup(ctx, true);
	}
}

static void iot_status_cb(iot_status_t status,
		iot_stat_lv_t stat_lv, void *usr_data)
{
	g_iot_status = status;
	printf("iot_status: %d, lv: %d\r\n", status, stat_lv);

	switch(status)
	{
		case IOT_STATUS_NEED_INTERACT:
			noti_led_mode = LED_ANIMATION_MODE_FAST;
			break;
		case IOT_STATUS_IDLE:
		case IOT_STATUS_CONNECTING:
			noti_led_mode = LED_ANIMATION_MODE_IDLE;
			if (smartlamp_switch_state == SMARTLAMP_SWITCH_ON) {
				MicoGpioOutputHigh(GPIO_OUTPUT_NOTIFICATION_LED);
			} else {
				MicoGpioOutputLow(GPIO_OUTPUT_NOTIFICATION_LED);
			}
			break;
		default:
			break;
	}
}

uint32_t ulHighFrequencyTimerTicks;
void level_timer_callback(void *arg)
{
	ulHighFrequencyTimerTicks++;
	color_led_onoff((ulHighFrequencyTimerTicks% (PWM_LEVEL + 1))
					< (smartlamp_switch_state * PWM_LEVEL * smartlamp_brightness_level/100));
}

static void cap_color_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
	send_color_capability(handle, smartlamp_color_hue, smartlamp_color_saturation);
}

static void cap_color_cmd_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	int tmp_state;
	JSON_H* json_object;
	JSON_H* item;

	printf("called [%s] func with : num_args:%u\r\n",
		__func__, cmd_data->num_args);

	json_object = JSON_PARSE(cmd_data->cmd_data[0].json_object);
	if (!json_object) {
		printf("fail to parse json object\n");
		return;
	}
	item = JSON_GET_OBJECT_ITEM(json_object, "hue");
	if (JSON_IS_NUMBER(item)) {
		smartlamp_color_hue = item->valuedouble;
	}
	item = JSON_GET_OBJECT_ITEM(json_object, "saturation");
	if (JSON_IS_NUMBER(item)) {
		smartlamp_color_saturation = item->valuedouble;
	}
	JSON_DELETE(json_object);

	update_rgb_from_hsl(smartlamp_color_hue, smartlamp_color_saturation, smartlamp_color_level,
					&smartlamp_color_red, &smartlamp_color_green, &smartlamp_color_blue);

	printf("HSL (%lf, %lf, %d), RGB (%d, %d, %d)\r\n",
			smartlamp_color_hue, smartlamp_color_saturation, smartlamp_color_level,
			smartlamp_color_red, smartlamp_color_green, smartlamp_color_blue);

	tmp_state = smartlamp_switch_state;
	smartlamp_switch_state = SMARTLAMP_SWITCH_ON;

	color_led_onoff(smartlamp_switch_state);

	send_color_capability(handle, smartlamp_color_hue, smartlamp_color_saturation);

	iot_os_delay(LED_BLINK_TIME);
	smartlamp_switch_state = tmp_state;
}

static void cap_level_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
	send_capability_integer(handle, "level", smartlamp_brightness_level);
}

static void level_cmd_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	int tmp_state;

	printf("called [%s] func with : num_args:%u\n",
		__func__, cmd_data->num_args);
	smartlamp_brightness_level = cmd_data->cmd_data[0].integer;

	tmp_state = smartlamp_switch_state;
	smartlamp_switch_state = SMARTLAMP_SWITCH_ON;

	send_capability_integer(handle, "level", smartlamp_brightness_level);

	iot_os_delay(LED_BLINK_TIME);
	smartlamp_switch_state = tmp_state;
}

static void cap_switch_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
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

static void cap_switch_cmd_off_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	IOT_EVENT *off_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	printf("called [%s] func with : num_args:%u\r\n",
		__func__, cmd_data->num_args);

	change_switch_state(SMARTLAMP_SWITCH_OFF);

	/* Setup switch off state */
	off_evt = st_cap_attr_create_string("switch", "off", NULL);

	/* Send switch off event */
	sequence_no = st_cap_attr_send(handle, evt_num, &off_evt);
	if (sequence_no < 0)
		printf("fail to send off_data\r\n");

	printf("Sequence number return : %d\r\n", sequence_no);
	st_cap_attr_free(off_evt);
}

static void cap_switch_cmd_on_cb(IOT_CAP_HANDLE *handle,
			iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
	IOT_EVENT *on_evt;
	uint8_t evt_num = 1;
	int32_t sequence_no;

	printf("called [%s] func with : num_args:%u\r\n",
		__func__, cmd_data->num_args);

	change_switch_state(SMARTLAMP_SWITCH_ON);

	/* Setup switch on state */
	on_evt = st_cap_attr_create_string("switch", "on", NULL);

	/* Send switch on event */
	sequence_no = st_cap_attr_send(handle, evt_num, &on_evt);
	if (sequence_no < 0)
		printf("fail to send on_data\r\n");

	printf("Sequence number return : %d\r\n", sequence_no);
	st_cap_attr_free(on_evt);
}

static void iot_noti_cb(iot_noti_data_t *noti_data, void *noti_usr_data)
{
	printf("Notification message received\r\n");

	if (noti_data->type == IOT_NOTI_TYPE_DEV_DELETED) {
		printf("[device deleted]\r\n");
	} else if (noti_data->type == IOT_NOTI_TYPE_RATE_LIMIT) {
		printf("[rate limit] Remaining time:%d, sequence number:%d\r\n",
			noti_data->raw.rate_limit.remainingTime, noti_data->raw.rate_limit.sequenceNumber);
	}
}

static void button_led_init(void)
{
	//notify led init
	MicoGpioInitialize(GPIO_OUTPUT_NOTIFICATION_LED, OUTPUT_PUSH_PULL);
	MicoGpioOutputHigh(GPIO_OUTPUT_NOTIFICATION_LED);

	//rgb led init
	MicoGpioInitialize(GPIO_OUTPUT_RED_LED, OUTPUT_PUSH_PULL);
	MicoGpioOutputLow(GPIO_OUTPUT_RED_LED);

	MicoGpioInitialize(GPIO_OUTPUT_GREEN_LED, OUTPUT_PUSH_PULL);
	MicoGpioOutputLow(GPIO_OUTPUT_GREEN_LED);

	MicoGpioInitialize(GPIO_OUTPUT_BLUE_LED, OUTPUT_PUSH_PULL);
	MicoGpioOutputLow(GPIO_OUTPUT_BLUE_LED);

	//button init
	MicoGpioInitialize(GPIO_INPUT_BUTTON, INPUT_PULL_UP);
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

static bool get_button_event(int* button_event_type, int* button_event_count)
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
			printf("Button event, val: %d, tick: %u\r\n", gpio_level, button_timeout);
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

static void smartlamp_task(void *arg)
{
	IOT_CAP_HANDLE *handle = (IOT_CAP_HANDLE *)arg;

	int button_event_type;
	int button_event_count;

	for (;;) {
		if (get_button_event(&button_event_type, &button_event_count)) {
			button_event(handle, button_event_type, button_event_count);
		}

		if (noti_led_mode != LED_ANIMATION_MODE_IDLE) {
			change_led_state(noti_led_mode);
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
	IOT_CAP_HANDLE* switch_handle = NULL;
	IOT_CAP_HANDLE* color_handle = NULL;
	IOT_CAP_HANDLE* level_handle = NULL;
	int iot_err;

	mico_rtos_init_timer(&led_pwm_timer, PwmRestoreDefaultTimeOut, (void*)level_timer_callback, NULL);
	mico_rtos_start_timer(&led_pwm_timer);

	// 1. create a iot context
	ctx = st_conn_init(onboarding_config, onboarding_config_len, device_info, device_info_len);
	if (ctx != NULL) {
		iot_err = st_conn_set_noti_cb(ctx, iot_noti_cb, NULL);
		if (iot_err)
			printf("fail to set notification callback function\r\n");

	// 2. create a handle to process capability
	//	implement init_callback function
		switch_handle = st_cap_handle_init(ctx, "main", "switch", cap_switch_init_cb, NULL);
		color_handle = st_cap_handle_init(ctx, "main", "colorControl", cap_color_init_cb, NULL);
		level_handle = st_cap_handle_init(ctx, "main", "switchLevel", cap_level_init_cb, NULL);
	// 3. register a callback function to process capability command when it comes from the SmartThings Server
	//	implement callback function
		iot_err = st_cap_cmd_set_cb(switch_handle, "off", cap_switch_cmd_off_cb, NULL);
		if (iot_err)
			printf("fail to set cmd_cb for off\r\n");
		iot_err = st_cap_cmd_set_cb(switch_handle, "on", cap_switch_cmd_on_cb, NULL);
		if (iot_err)
			printf("fail to set cmd_cb for on\r\n");
		iot_err = st_cap_cmd_set_cb(color_handle, "setColor", cap_color_cmd_cb, NULL);
		if (iot_err)
			printf("fail to set cmd_cb for setcolor\r\n");
		iot_err = st_cap_cmd_set_cb(level_handle, "setLevel", level_cmd_cb, NULL);
		if (iot_err)
			printf("fail to set cmd_cb for setcolor\r\n");

	} else {
		printf("fail to create the iot_context\r\n");
	}
	
	button_led_init();

	update_rgb_from_hsl(smartlamp_color_hue, smartlamp_color_saturation, smartlamp_color_level,
			&smartlamp_color_red, &smartlamp_color_green, &smartlamp_color_blue);

	printf("HSL (%lf, %lf, %d), RGB (%d, %d, %d)\r\n",
			smartlamp_color_hue, smartlamp_color_saturation, smartlamp_color_level,
			smartlamp_color_red, smartlamp_color_green, smartlamp_color_blue);
	color_led_onoff(smartlamp_switch_state);
	// 4. needed when it is necessary to keep monitoring the device status
	iot_os_thread_create(smartlamp_task, "smartlamp_task", 2048, (void *)switch_handle, 10, NULL);

	// 5. process on-boarding procedure. There is nothing more to do on the app side than call the API.
	st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, NULL);
	printf("IOT_CONNECT_DONE >>PT<<\r\n");
}