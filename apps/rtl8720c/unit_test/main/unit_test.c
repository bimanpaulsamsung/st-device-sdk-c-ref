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
#include "platform_stdlib.h"
#include "iot_debug.h"
#include "PinNames.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "iot_cli_cmd.h"
#include "iot_uart_cli.h"


void app_main(void)
{
	IOT_INFO("ENTER");
	register_iot_cli_cmd();
	uart_cli_main();
	IOT_INFO("LEAVE");
}
