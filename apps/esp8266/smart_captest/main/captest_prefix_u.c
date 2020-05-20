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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "caps_ultravioletIndex.h"

static caps_ultravioletIndex_data_t *caps_ultravioletIndex_data;

// CAPTEST : make init_cb for test capabilities
static void cap_ultravioletIndex_init_cb(struct caps_ultravioletIndex_data *caps_data)
{
	caps_data->set_ultravioletIndex_value(caps_data, 0);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_u_test_capabilities()
{
	static int send_count = 0;
	send_count++;

	//ultravioletIndex
	double value = send_count * 10;
	caps_ultravioletIndex_data->set_ultravioletIndex_value(caps_ultravioletIndex_data, value);
	caps_ultravioletIndex_data->attr_ultravioletIndex_send(caps_ultravioletIndex_data);
}

void captest_u_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_ultravioletIndex_data = caps_ultravioletIndex_initialize(ctx, "main", cap_ultravioletIndex_init_cb, NULL);

	// CAPTEST : set cmd usr cb
}
