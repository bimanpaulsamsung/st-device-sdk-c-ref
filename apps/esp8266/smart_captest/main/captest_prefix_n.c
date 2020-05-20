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

#include "caps_networkMeter.h"
#include "caps_notification.h"

static caps_networkMeter_data_t *cap_networkMeter_data;
static caps_notification_data_t *cap_notification_data;


// CAPTEST : make init_cb for test capabilities
static void cap_networkMeter_init_cb(struct caps_networkMeter_data *caps_data)
{   
	caps_data->set_downlinkSpeed_value(caps_data, 1);
	caps_data->set_uplinkSpeed_value(caps_data, 10);
}

static void cap_notification_init_cb(struct caps_notification_data *caps_data)
{   
    printf("[%s] init cb do nothing.\n", __func__);
}

// CAPTEST : make cmd_cb for test capabilities
static void cap_notification_cmd_cb(struct caps_notification_data *caps_data)
{
	printf("[%s] is called\n", __func__);
}


// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_n_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	//networkMeter
	int ds = send_count * 10;
	int us = send_count + 10;
	cap_networkMeter_data->set_downlinkSpeed_value(cap_networkMeter_data, ds);
	cap_networkMeter_data->set_uplinkSpeed_value(cap_networkMeter_data, us);
	
	cap_networkMeter_data->attr_downlinkSpeed_send(cap_networkMeter_data);
	cap_networkMeter_data->attr_uplinkSpeed_send(cap_networkMeter_data);

}

void captest_n_initialize(IOT_CTX *ctx)
{

// CAPTEST : call initialize function and set init_cb
	cap_networkMeter_data = caps_networkMeter_initialize(ctx, "main", cap_networkMeter_init_cb, NULL);
	cap_notification_data = caps_notification_initialize(ctx, "main", cap_notification_init_cb, NULL);


// CAPTEST : set cmd usr cb
	//networkMeter has no cmd cb
	cap_notification_data->cmd_deviceNotification_usr_cb = cap_notification_cmd_cb;

}
