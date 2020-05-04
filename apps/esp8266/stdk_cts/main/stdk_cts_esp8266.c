/* ***************************************************************************
 *
 * Copyright (c) 2020 Samsung Electronics All Rights Reserved.
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
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "cts.h"

#define TEST_DONE_BIT	(0x01)

static void cts_task(void *arg)
{
	EventGroupHandle_t *event_group = (EventGroupHandle_t) arg;

	CTS_iot_bsp_test();
	CTS_iot_os_test();

	xEventGroupSetBits(*event_group, TEST_DONE_BIT);
	vTaskDelete(NULL);
}

void app_main(void)
{
	EventGroupHandle_t event_group;
	event_group = xEventGroupCreate();
	xTaskCreate(cts_task, "stdk_cts_task", 4086, &event_group, 10, NULL);

	xEventGroupWaitBits(event_group, TEST_DONE_BIT, pdTRUE, pdFALSE, portMAX_DELAY);

	printf("\nCTS done\n");
}