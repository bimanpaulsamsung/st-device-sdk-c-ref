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

#include "st_dev.h"
#include "fsl_shell.h"
#include "fsl_gpio.h"
#include "device_control.h"

void device_button_init(void)
{
	GPIO_PortInit(GPIO, BOARD_BUTTON_PORT);
	gpio_pin_config_t button_config = {
		.pinDirection = kGPIO_DigitalInput,
	};
	GPIO_PinInit(GPIO, BOARD_BUTTON_PORT, BOARD_BUTTON_PIN, &button_config);
}

void device_led_init(void)
{
	//notify led init
	GPIO_PortInit(GPIO, BOARD_LED_PORT);
	gpio_pin_config_t USER_LED_config = {
		.pinDirection = kGPIO_DigitalOutput,
		.outputLogic = 0U
	};
	GPIO_PinInit(GPIO, BOARD_LED_PORT, BOARD_USER_LED_PIN, &USER_LED_config);
}

int device_button_read()
{
	int value = 0;

	value = GPIO_PinRead(GPIO, BOARD_BUTTON_PORT, BOARD_BUTTON_PIN);

	return value;
}

void device_noti_led_control(int state)
{
	if (state == 0)
		GPIO_PortSet(GPIO, BOARD_LED_PORT, 1U << BOARD_USER_LED_PIN);
	else
		GPIO_PortClear(GPIO, BOARD_LED_PORT, 1U << BOARD_USER_LED_PIN);
}

