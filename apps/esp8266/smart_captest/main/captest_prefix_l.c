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

#include "caps_locationMode.h"
#include "caps_lock.h"

static caps_locationMode_data_t *cap_locationMode_data;
static caps_lock_data_t *cap_lock_data;


#define LOCATION_MODE_STR_TEST		"init_lm_test"

enum {
	LOCKED,
	UNKNOWN,
	UNLOCKED,
	UNLOCKED_TO,
	LOCK_MAX
};
static const char* lock_mode_test[] = {"locked", "unknown", "unlocked", "unlocked_with_timeout"};

// CAPTEST : make init_cb for test capabilities
static void cap_locationMode_init_cb(struct caps_locationMode_data *caps_data)
{   
    caps_data->set_mode_value(caps_data, LOCATION_MODE_STR_TEST);
}

static void cap_lock_init_cb(struct caps_lock_data *caps_data)
{   
    caps_data->set_lock_value(caps_data, lock_mode_test[UNLOCKED]);
}

// CAPTEST : make cmd_cb for test capabilities
static void cap_locationMode_cmd_cb(struct caps_locationMode_data *caps_data)
{
	const char* mode = caps_data->get_mode_value(caps_data);
	printf("[%s]mode is : %s", __func__, mode);
}

static void cap_lock_cmd_cb(struct caps_lock_data *caps_data)
{
	const char* lock = caps_data->get_lock_value(caps_data);
	printf("[%s]lock status is : %s", __func__, lock);
	int index = caps_data->attr_lock_str2idx(lock);
	printf("[%s]get index : %d", __func__, index);
}

static void cap_unlock_cmd_cb(struct caps_lock_data *caps_data)
{
	const char* lock = caps_data->get_lock_value(caps_data);
	printf("[%s]lock status is : %s", __func__, lock);
	int index = caps_data->attr_lock_str2idx(lock);
	printf("[%s]get index : %d", __func__, index);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_l_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	//locationMode
	char lmode[32];
	snprintf(lmode, 32, "locMode-%d", send_count);
	cap_locationMode_data->set_mode_value(cap_locationMode_data, lmode);
	cap_locationMode_data->attr_mode_send(cap_locationMode_data);

	//lock
	cap_lock_data->set_lock_value(cap_lock_data, lock_mode_test[send_count % LOCK_MAX]);
	cap_lock_data->attr_lock_send(cap_lock_data);
}

void captest_l_initialize(IOT_CTX *ctx)
{

// CAPTEST : call initialize function and set init_cb
	cap_locationMode_data = caps_locationMode_initialize(ctx, "main", cap_locationMode_init_cb, NULL);
	cap_lock_data = caps_lock_initialize(ctx, "main", cap_lock_init_cb, NULL);


// CAPTEST : set cmd usr cb
	cap_locationMode_data->cmd_setMode_usr_cb = cap_locationMode_cmd_cb;
	cap_lock_data->cmd_lock_usr_cb = cap_lock_cmd_cb;
	cap_lock_data->cmd_unlock_usr_cb = cap_unlock_cmd_cb;

}
