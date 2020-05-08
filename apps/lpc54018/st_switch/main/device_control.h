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

#define BOARD_LED_PORT 3U
#define BOARD_USER_LED_PIN 13U

#define BOARD_BUTTON_PORT   0U
#define BOARD_BUTTON_PIN    10U

void device_button_init(void);
void device_noti_led_init(void);
int device_button_read(void);
void device_noti_led_control(int op);
