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

#include "caps_zwMultichannel.h"

static caps_zwMultichannel_data_t *caps_zwMultichannel_data;

#define INIT_TEST_ZWMULTICHAN_INFO_STRING	"test zwmultichan info"
#define INIT_TEST_ZWMULTICHAN_EVENT_STRING	"test zwmultichan event"
static const char *test_epinfo_array[] = {"info1", "info2", "info3", "info4"};
static const char *test_epevent_array[] = {"event1", "event2", "event3", "event4"};

// CAPTEST : make init_cb for test capabilities
static void cap_zwMultichannel_init_cb(struct caps_zwMultichannel_data *caps_data)
{
	caps_data->set_epInfo_value(caps_data, INIT_TEST_ZWMULTICHAN_INFO_STRING);
	caps_data->set_epEvent_value(caps_data, INIT_TEST_ZWMULTICHAN_EVENT_STRING);
}

// CAPTEST : make cmd_cb for test capabilities
static void cap_epCmd_cmd_cb(struct caps_zwMultichannel_data *caps_data)
{
	const char *info = caps_data->get_epInfo_value(caps_data);
	printf("[%s] epcmd info value : %s\n", __func__, info);
}

static void cap_enableEpEvents_cmd_cb(struct caps_zwMultichannel_data *caps_data)
{
	const char *event = caps_data->get_epEvent_value(caps_data);
	printf("[%s] epevent info value : %s\n", __func__, event);
}


// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_z_test_capabilities()
{
	static int send_count = 0;
	send_count++;

	//zwMultichannel
	caps_zwMultichannel_data->set_epInfo_value(caps_zwMultichannel_data, test_epinfo_array[send_count % 4]);
	caps_zwMultichannel_data->attr_epInfo_send(caps_zwMultichannel_data);

	caps_zwMultichannel_data->set_epEvent_value(caps_zwMultichannel_data, test_epevent_array[send_count % 4]);
	caps_zwMultichannel_data->attr_epEvent_send(caps_zwMultichannel_data);
}

void captest_z_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_zwMultichannel_data = caps_zwMultichannel_initialize(ctx, "main", cap_zwMultichannel_init_cb, NULL);

	// CAPTEST : set cmd usr cb
	caps_zwMultichannel_data->cmd_enableEpEvents_usr_cb = cap_enableEpEvents_cmd_cb;
	caps_zwMultichannel_data->cmd_epCmd_usr_cb = cap_epCmd_cmd_cb;
}
