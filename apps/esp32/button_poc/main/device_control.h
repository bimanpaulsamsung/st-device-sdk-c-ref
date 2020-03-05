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

//#define CONFIG_TARGET_WITTY_CLOUD
#if defined(CONFIG_TARGET_WITTY_CLOUD)

#define GPIO_OUTPUT_NOTIFICATION_LED 2
#define GPIO_INPUT_BUTTON 4

#define GPIO_OUTPUT_MAINLED 15
#define GPIO_OUTPUT_MAINLED_0 16 /* use as ground */

#define GPIO_OUTPUT_NOUSE1 12
#define GPIO_OUTPUT_NOUSE2 13

enum notification_led_gpio_state {
	NOTIFICATION_LED_GPIO_ON = 0,
	NOTIFICATION_LED_GPIO_OFF = 1,
};
#else //default

#define GPIO_INPUT_BUTTON_MAIN    13
#define GPIO_INPUT_BUTTON_MIC_OFF 27
#define GPIO_INPUT_BUTTON_VOL_UP  32
#define GPIO_INPUT_BUTTON_VOL_DN  33

#define GPIO_OUTPUT_LED_RED       21
#define GPIO_OUTPUT_LED_BLUE      19
#define GPIO_OUTPUT_LED_GREEN     22

enum led_gpio_state {
	LED_GPIO_OFF = 1,
	LED_GPIO_ON = 0,
};


#endif

typedef struct _button_t{
	uint32_t count;
	uint32_t last_state;
	TimeOut_t timeout;
	TickType_t long_press_tick;
	TickType_t delay_tick;
}button_t;


enum led_animation_mode_list {
	LED_ANIMATION_MODE_IDLE = 0,
	LED_ANIMATION_MODE_FAST,
	LED_ANIMATION_MODE_SLOW,
};

enum button_gpio_state {
	BUTTON_GPIO_RELEASED = 1,
	BUTTON_GPIO_PRESSED = 0,
};

#define BUTTON_DEBOUNCE_TIME_MS 20
#define BUTTON_LONG_THRESHOLD_MS 5000
#define BUTTON_DELAY_MS 300

#define BUTTON_MAX 4

enum button_event_type {
	BUTTON_LONG_PRESS = 0,
	BUTTON_SHORT_PRESS = 1,
	TYPE_MAX = 2,
};

void button_isr_handler(void *arg);
int get_button_event(int* button_event_type, int* button_event_count, int* button_event_id);
void led_blink(int gpio, int delay, int count);
void change_led_state(int noti_led_mode);
int get_adc(void);
void gpio_init(void);
