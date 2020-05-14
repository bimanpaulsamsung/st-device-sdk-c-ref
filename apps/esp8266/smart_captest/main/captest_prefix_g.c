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

#include "caps_gasMeter.h"
#include "caps_garageDoorControl.h"

static caps_gasMeter_data_t *caps_gasMeter_data;
static caps_garageDoorControl_data_t *caps_garageDoorControl_data;

#define INIT_GASMETER_TIME "2020-05-15 14:14:15"
// CAPTEST : make init_cb for test capabilities
static void cap_gasMeter_init_cb(struct caps_gasMeter_data *caps_data)
{
	caps_data->set_gasMeterTime_value(caps_data, INIT_GASMETER_TIME);

	caps_data->set_gasMeter_value(caps_data, caps_helper_gasMeter.
					attr_gasMeter.min);
	caps_data->set_gasMeter_unit(caps_data, caps_helper_gasMeter.
					attr_gasMeter.units[CAP_ENUM_GASMETER_GASMETER_UNIT_KWH]);

	caps_data->set_gasMeterCalorific_value(caps_data, caps_helper_gasMeter.
					attr_gasMeterCalorific.min);

	caps_data->set_gasMeterVolume_value(caps_data, caps_helper_gasMeter.
					attr_gasMeterVolume.min);
	caps_data->set_gasMeterVolume_unit(caps_data, caps_helper_gasMeter.
					attr_gasMeterVolume.units[CAP_ENUM_GASMETER_GASMETERVOLUME_UNIT_M3]);
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "init gasMeterPrecision");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", 2);
	caps_data->set_gasMeterPrecision_value(caps_data, root);
	caps_data->set_gasMeterConversion_value(caps_data, caps_helper_gasMeter.
					attr_gasMeterConversion.min);
}

static void cap_garageDoorControl_init_cb(struct caps_garageDoorControl_data *caps_data)
{
	caps_data->set_door_value(caps_data, caps_helper_garageDoorControl.
					attr_door.values[CAP_ENUM_GARAGEDOORCONTROL_DOOR_VALUE_CLOSED]);
}
// CAPTEST : make cmd_cb for test capabilities
static void cap_garageDoorControl_cmd_cb(struct caps_garageDoorControl_data *caps_data)
{
	const char *value = caps_data->get_door_value(caps_data);
	printf("[%s] garageDoorControl value: %s\n", __func__, value);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_g_test_capabilities()
{
	static int send_count = 0;
	send_count++;

	//gasMeter
	char test_time[50];
	snprintf(test_time, 50, "2020-05-15 13:13:1%d", send_count);
	caps_gasMeter_data->set_gasMeterTime_value(caps_gasMeter_data, test_time);
	caps_gasMeter_data->attr_gasMeterTime_send(caps_gasMeter_data);

	double test_gasMeter_value = send_count;
	caps_gasMeter_data->set_gasMeter_value(caps_gasMeter_data, test_gasMeter_value);
	int g_idx = send_count % CAP_ENUM_GASMETER_GASMETER_UNIT_MAX;
	caps_gasMeter_data->set_gasMeter_unit(caps_gasMeter_data, caps_helper_gasMeter.
					attr_gasMeter.units[g_idx]);
	caps_gasMeter_data->attr_gasMeter_send(caps_gasMeter_data);

	double test_gasMeterCalorific_value = send_count;
	caps_gasMeter_data->set_gasMeterCalorific_value(caps_gasMeter_data, test_gasMeterCalorific_value);
	caps_gasMeter_data->attr_gasMeterCalorific_send(caps_gasMeter_data);

	double test_gasMeterVolume_value = send_count;
	caps_gasMeter_data->set_gasMeterVolume_value(caps_gasMeter_data, test_gasMeterVolume_value);
	int v_idx = send_count % CAP_ENUM_GASMETER_GASMETERVOLUME_UNIT_MAX;
	caps_gasMeter_data->set_gasMeterVolume_unit(caps_gasMeter_data, caps_helper_gasMeter.
					attr_gasMeterVolume.units[v_idx]);
	caps_gasMeter_data->attr_gasMeterVolume_send(caps_gasMeter_data);

	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "gasMeterPrecision");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", send_count);
	caps_gasMeter_data->set_gasMeterPrecision_value(caps_gasMeter_data, root);
	caps_gasMeter_data->attr_gasMeterPrecision_send(caps_gasMeter_data);
	JSON_FREE(root);

	double test_gasMeterConversion_value = send_count;
	caps_gasMeter_data->set_gasMeterConversion_value(caps_gasMeter_data, test_gasMeterConversion_value);
	caps_gasMeter_data->attr_gasMeterConversion_send(caps_gasMeter_data);
	//garageDoorControl
	int door_idx = send_count % CAP_ENUM_GARAGEDOORCONTROL_DOOR_VALUE_MAX;
	caps_garageDoorControl_data->set_door_value(caps_garageDoorControl_data, caps_helper_garageDoorControl.
					attr_door.values[door_idx]);
	caps_garageDoorControl_data->attr_door_send(caps_garageDoorControl_data);
}

void captest_g_initialize(IOT_CTX *ctx)
{

	// CAPTEST : call initialize function and set init_cb
	caps_gasMeter_data = caps_gasMeter_initialize(ctx, "main", cap_gasMeter_init_cb, NULL);
	caps_garageDoorControl_data = caps_garageDoorControl_initialize(ctx, "main", cap_garageDoorControl_init_cb, NULL);

	// CAPTEST : set cmd usr cb
	caps_garageDoorControl_data->cmd_close_usr_cb = cap_garageDoorControl_cmd_cb;
	caps_garageDoorControl_data->cmd_open_usr_cb = cap_garageDoorControl_cmd_cb;
}
