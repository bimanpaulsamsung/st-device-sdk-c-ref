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

#include <stdbool.h>
#include "st_dev.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "device_control.h"

char *button_type[TYPE_MAX] = {	"Long pressed",
				"Short pressed"};

char *button_id[BUTTON_MAX] = {	"Main button",
				"Mic OFF button",
				"Volume + button",
 				"Volume - button"};

int button_list[BUTTON_MAX] = {	GPIO_INPUT_BUTTON_MAIN,
				GPIO_INPUT_BUTTON_MIC_OFF,
				GPIO_INPUT_BUTTON_VOL_UP,
				GPIO_INPUT_BUTTON_VOL_DN};

static button_t buttons[BUTTON_MAX] = {0};

static xQueueHandle button_event_queue = NULL;

static void button_init(void)
{
	for(int idx=0; idx < BUTTON_MAX; idx++){
		buttons[idx].count = 0;
		buttons[idx].last_state = BUTTON_GPIO_RELEASED;
		buttons[idx].long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
		buttons[idx].delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);
	}
	return;
}

int get_button_event(int* button_event_type, int* button_event_count, int* button_event_id)
{
	button_t *button = NULL;
	uint32_t gpio_level = 0;

	for(int idx = 0; idx < BUTTON_MAX; idx++){
		button = &buttons[idx];
		gpio_level = gpio_get_level(button_list[idx]);
		if (button->last_state != gpio_level) {
			/* wait debounce time to ignore small ripple of currunt */
			vTaskDelay( pdMS_TO_TICKS(BUTTON_DEBOUNCE_TIME_MS) );
			gpio_level = gpio_get_level(button_list[idx]);
			if (button->last_state != gpio_level) {
				button->last_state = gpio_level;
				if (gpio_level == BUTTON_GPIO_PRESSED) {
					button->count++;
				}
				vTaskSetTimeOutState(&button->timeout);
				button->delay_tick = pdMS_TO_TICKS(BUTTON_DELAY_MS);
				button->long_press_tick = pdMS_TO_TICKS(BUTTON_LONG_THRESHOLD_MS);
			}
		} else if (button->count > 0) {
			if ((gpio_level == BUTTON_GPIO_PRESSED)
					&& (xTaskCheckForTimeOut(&button->timeout, &button->long_press_tick ) != pdFALSE)) {
				*button_event_type = BUTTON_LONG_PRESS;
				*button_event_count = 1;
 				*button_event_id = idx;
				button->count = 0;
				return true;
			} else if ((gpio_level == BUTTON_GPIO_RELEASED)
					&& (xTaskCheckForTimeOut(&button->timeout, &button->delay_tick ) != pdFALSE)) {
				*button_event_type = BUTTON_SHORT_PRESS;
				*button_event_count = button->count;
				*button_event_id = idx;
				button->count = 0;
				return true;
			}
		}
	}
	return false;
}

void led_blink(int gpio, int delay, int count)
{
	uint32_t gpio_level;

	gpio_level =  gpio_get_level(gpio);
	for (int i = 0; i < count; i++) {
		gpio_set_level(GPIO_OUTPUT_LED_RED, 1 - gpio_level);
		vTaskDelay(delay / portTICK_PERIOD_MS);
		gpio_set_level(GPIO_OUTPUT_LED_RED, gpio_level);
		vTaskDelay(delay / portTICK_PERIOD_MS);
	}
}

void change_led_state(int noti_led_mode)
{
	static TimeOut_t led_timeout;
	static TickType_t led_tick = -1;
	static int last_led_mode = -1;

	uint32_t gpio_level = 0;

	if (last_led_mode != noti_led_mode) {
		last_led_mode = noti_led_mode;
		vTaskSetTimeOutState(&led_timeout);
		led_tick = 0;
	}

	switch (noti_led_mode)
	{
		case LED_ANIMATION_MODE_IDLE:
			break;
		case LED_ANIMATION_MODE_SLOW:
			gpio_level =  gpio_get_level(GPIO_OUTPUT_LED_RED);
			if ((gpio_level == LED_GPIO_ON) &&
					(xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE)) {
				gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);
				vTaskSetTimeOutState(&led_timeout);
				led_tick = pdMS_TO_TICKS(1000);
			}
			if ((gpio_level == LED_GPIO_OFF) &&
					(xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE)) {
				gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_ON);
				vTaskSetTimeOutState(&led_timeout);
				led_tick = pdMS_TO_TICKS(200);
			}
			break;
		case LED_ANIMATION_MODE_FAST:
			gpio_level =  gpio_get_level(GPIO_OUTPUT_LED_RED);
			if ((gpio_level == LED_GPIO_ON) &&
					(xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE)) {
				gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);
				vTaskSetTimeOutState(&led_timeout);
				led_tick = pdMS_TO_TICKS(100);
			}
			if ((gpio_level == LED_GPIO_OFF) &&
					(xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE)) {
				gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_ON);
				vTaskSetTimeOutState(&led_timeout);
				led_tick = pdMS_TO_TICKS(100);
			}
			break;
		default:
			break;
	}
}

int get_adc(void)
{
	return adc1_get_raw(ADC1_CHANNEL_6);
}

void gpio_init(void)
{
	gpio_config_t io_conf;

	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_down_en = 1;
	io_conf.pull_up_en = 0;
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_LED_RED;
	gpio_config(&io_conf);
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_LED_BLUE;
	gpio_config(&io_conf);
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_LED_GREEN;
	gpio_config(&io_conf);

	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_down_en = (BUTTON_GPIO_RELEASED == 0);
	io_conf.pull_up_en = (BUTTON_GPIO_RELEASED == 1);
	for(int idx = 0; idx < BUTTON_MAX; idx++){
		io_conf.pin_bit_mask = (uint64_t)1 << button_list[idx];
		gpio_config(&io_conf);
	}

	button_event_queue = xQueueCreate(10, sizeof(uint32_t));

	gpio_install_isr_service(0);

	gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);
	gpio_set_level(GPIO_OUTPUT_LED_BLUE, LED_GPIO_OFF);
	gpio_set_level(GPIO_OUTPUT_LED_GREEN, LED_GPIO_OFF);

	button_init();

	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0);
}


