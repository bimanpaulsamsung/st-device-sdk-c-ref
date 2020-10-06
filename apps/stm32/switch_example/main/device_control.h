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
#ifndef _DEVICE_CONTROL_H_
#define _DEVICE_CONTROL_H_

#include "mbed.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_OUTPUT_MAINLED LED1

enum switch_onoff_state {
    SWITCH_OFF = 0,
    SWITCH_ON = 1,
};

enum main_led_gpio_state
{
	MAINLED_GPIO_OFF = 1,
	MAINLED_GPIO_ON = 0,
};

enum button_event_type {
    BUTTON_LONG_PRESS = 0,
    BUTTON_SHORT_PRESS = 1,
};

void gpio_set_level(PinName gpio_pin, int value);
void change_switch_state(int switch_state);

#ifdef __cplusplus
}
#endif

#endif
