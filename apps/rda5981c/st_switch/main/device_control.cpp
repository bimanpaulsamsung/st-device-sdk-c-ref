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

#include "device_control.h"

#include "mbed.h"
#include "platform/mbed_thread.h"
#include "us_ticker_api.h"

#define pdFALSE false
#define pdTRUE true

static void SetStartTime(unsigned int *mstime)
{
	*mstime = (unsigned int)rtos::Kernel::get_ms_count();
}

static unsigned int GetMsCount(void)
{
	return (unsigned int)rtos::Kernel::get_ms_count();
}

static int CheckForTimeOut(unsigned int begin, unsigned int timeout)
{
	unsigned int mstime = (unsigned int)rtos::Kernel::get_ms_count();

	if (mstime < (begin + timeout))
		return false;

	return true;
}

void gpio_set_level(PinName gpio_pin, int value)
{
	DigitalOut led(gpio_pin, value);
}

int gpio_get_level(PinName gpio_pin)
{
	DigitalIn led(gpio_pin);
	return led.read();
}

int get_button_event(int *button_event_type, int *button_event_count)
{
	static uint32_t button_count = 0;
	static uint32_t button_last_state = BUTTON_GPIO_RELEASED;
	static unsigned int button_timeout;
	static unsigned int long_press_tick = BUTTON_LONG_THRESHOLD_MS;
	static unsigned int button_delay_tick = BUTTON_DELAY_MS;

	uint32_t gpio_level = 0;

	gpio_level = gpio_get_level(GPIO_INPUT_BUTTON);
	if (button_last_state != gpio_level) {
		/* wait debounce time to ignore small ripple of currunt */
		thread_sleep_for(BUTTON_DEBOUNCE_TIME_MS);
		gpio_level = gpio_get_level(GPIO_INPUT_BUTTON);
		if (button_last_state != gpio_level) {
			printf("Button event, val: %d, ms: %u\n", gpio_level,
					(uint32_t) GetMsCount());
			button_last_state = gpio_level;
			if (gpio_level == BUTTON_GPIO_PRESSED) {
				button_count++;
			}
			SetStartTime(&button_timeout);
			button_delay_tick = BUTTON_DELAY_MS;
			long_press_tick = BUTTON_LONG_THRESHOLD_MS;
		}
	} else if (button_count > 0) {
		if ((gpio_level == BUTTON_GPIO_PRESSED)
				&& (CheckForTimeOut(button_timeout, long_press_tick)
						!= pdFALSE)) {
			*button_event_type = BUTTON_LONG_PRESS;
			*button_event_count = 1;
			button_count = 0;
			return true;
		} else if ((gpio_level == BUTTON_GPIO_RELEASED)
				&& (CheckForTimeOut(button_timeout, button_delay_tick)
						!= pdFALSE)) {
			*button_event_type = BUTTON_SHORT_PRESS;
			*button_event_count = button_count;
			button_count = 0;
			return true;
		}
	}

	return false;
}

void led_blink(PinName gpio, int delay, int count)
{
	DigitalOut led(gpio);

	for (int i = 0; i < count; i++) {
		led = !led;
		thread_sleep_for(delay);
		led = !led;
		thread_sleep_for(delay);
	}
}

void change_led_state(int noti_led_mode)
{
	static unsigned int led_timeout;
	static unsigned int led_tick = -1;
	static int last_led_mode = -1;

	uint32_t gpio_level = 0;

	if (last_led_mode != noti_led_mode) {
		last_led_mode = noti_led_mode;
		SetStartTime(&led_timeout);
		led_tick = 0;
	}

	switch (noti_led_mode) {
	case LED_ANIMATION_MODE_IDLE:
		break;
	case LED_ANIMATION_MODE_SLOW:
		gpio_level = gpio_get_level(GPIO_OUTPUT_NOTIFICATION_LED);
		if ((gpio_level == NOTIFICATION_LED_GPIO_ON)
				&& (CheckForTimeOut(led_timeout, led_tick) != pdFALSE)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED,
					NOTIFICATION_LED_GPIO_OFF);
			SetStartTime(&led_timeout);
			led_tick = 1000;
		}
		if ((gpio_level == NOTIFICATION_LED_GPIO_OFF)
				&& (CheckForTimeOut(led_timeout, led_tick) != pdFALSE)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED,
					NOTIFICATION_LED_GPIO_ON);
			SetStartTime(&led_timeout);
			led_tick = 200;
		}
		break;
	case LED_ANIMATION_MODE_FAST:
		gpio_level = gpio_get_level(GPIO_OUTPUT_NOTIFICATION_LED);
		if ((gpio_level == NOTIFICATION_LED_GPIO_ON)
				&& (CheckForTimeOut(led_timeout, led_tick) != pdFALSE)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED,
					NOTIFICATION_LED_GPIO_OFF);
			SetStartTime(&led_timeout);
			led_tick = 100;
		}
		if ((gpio_level == NOTIFICATION_LED_GPIO_OFF)
				&& (CheckForTimeOut(led_timeout, led_tick) != pdFALSE)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED,
					NOTIFICATION_LED_GPIO_ON);
			SetStartTime(&led_timeout);
			led_tick = 100;
		}
		break;
	default:
		break;
	}
}