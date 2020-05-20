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

#include "caps_objectDetection.h"
#include "caps_odorSensor.h"
#include "caps_ovenMode.h"
#include "caps_ovenOperatingState.h"
#include "caps_ovenSetpoint.h"



static caps_objectDetection_data_t *cap_objectDetection_data;
static caps_odorSensor_data_t *cap_odorSensor_data;
static caps_ovenMode_data_t *cap_ovenMode_data;
static caps_ovenOperatingState_data_t *cap_ovenOperatingState_data;
static caps_ovenSetpoint_data_t *cap_ovenSetpoint_data;


static const char* objectDetection_support_array[] = {"active", "inactive"};

#define ODORSENSOR_INIT_LVL		66


// CAPTEST : make init_cb for test capabilities
static void cap_objectDetection_init_cb(struct caps_objectDetection_data *caps_data)
{
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_NUMBER_TO_OBJECT(root, "active", 1);
	JSON_ADD_NUMBER_TO_OBJECT(root, "inactive", 0);

	caps_data->set_detected_value(caps_data, root);
	caps_data->set_supportedValues_value(caps_data, objectDetection_support_array, sizeof(objectDetection_support_array) / sizeof(char*));
}

static void cap_odorSensor_init_cb(struct caps_odorSensor_data *caps_data)
{   
	caps_data->set_odorLevel_value(caps_data, ODORSENSOR_INIT_LVL);
}

static void cap_ovenMode_init_cb(struct caps_ovenMode_data *caps_data)
{   
	caps_data->set_ovenMode_value(caps_data, "heating");
	caps_data->set_supportedOvenModes_value(caps_data, (const char**)caps_helper_ovenMode.attr_ovenMode.values, sizeof(caps_helper_ovenMode.attr_ovenMode.values) / sizeof(char*));
}

static void cap_ovenOperatingState_init_cb(struct caps_ovenOperatingState_data *caps_data)
{   
	caps_data->set_ovenJobState_value(caps_data, "ready");
	char *ctime = "2020-05-14 9:00";
	caps_data->set_completionTime_value(caps_data, ctime);
	caps_data->set_supportedMachineStates_value(caps_data, (const char**)caps_helper_ovenOperatingState.attr_machineState.values,
												sizeof(caps_helper_ovenOperatingState.attr_machineState.values) / sizeof(char*));

	caps_data->set_progress_value(caps_data, caps_helper_ovenOperatingState.attr_progress.min); //10% progress
	caps_data->set_progress_unit(caps_data, caps_helper_ovenOperatingState.attr_progress.units[0]);
	caps_data->set_operationTime_value(caps_data, 30); //30 min?
	caps_data->set_machineState_value(caps_data, "ready");
}

static void cap_ovenSetpoint_init_cb(struct caps_ovenSetpoint_data *caps_data)
{   
	caps_data->set_ovenSetpoint_value(caps_data, 50);
}

// CAPTEST : make cmd_cb for test capabilities
//No cmd_usr_cb for objectDetection
//No cmd_usr_cb for odorSensor
static void cap_ovenMode_cmd_cb(struct caps_ovenMode_data *caps_data)
{
	const char *mode = caps_data->get_ovenMode_value(caps_data);
	printf("[%s] oven mode: %s, index %d\n", __func__, mode, caps_data->attr_ovenMode_str2idx(mode));
}

static void cap_ovenOperatingState_cmd_cb(struct caps_ovenOperatingState_data *caps_data)
{
	const char *state = caps_data->get_machineState_value(caps_data);
	printf("[%s] machine state: %s, index %d\n", __func__, state, caps_data->attr_machineState_str2idx(state));
}

static void cap_ovenOperatingState_start_cb(struct caps_ovenOperatingState_data *caps_data)
{
	printf("[%s] machine start\n", __func__);
}

static void cap_ovenOperatingState_stop_cb(struct caps_ovenOperatingState_data *caps_data)
{
	printf("[%s] machine stop\n", __func__);
}

static void cap_ovenSetpoint_cmd_cb(struct caps_ovenSetpoint_data *caps_data)
{
	int point = caps_data->get_ovenSetpoint_value(caps_data);
	printf("[%s] point: %d\n", __func__, point);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_o_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	//objectDetection
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_NUMBER_TO_OBJECT(root, "active", 10 + send_count);
	JSON_ADD_NUMBER_TO_OBJECT(root, "inactive", 20 + send_count);
	cap_objectDetection_data->set_detected_value(cap_objectDetection_data, root);
	cap_objectDetection_data->attr_detected_send(cap_objectDetection_data);

	//odorSensor
	cap_odorSensor_data->set_odorLevel_value(cap_odorSensor_data, ODORSENSOR_INIT_LVL + send_count);
	cap_odorSensor_data->attr_odorLevel_send(cap_odorSensor_data);

	//ovenMode
	int oven_idx = send_count % (sizeof(caps_helper_ovenMode.attr_ovenMode.values) / sizeof(char*));
	cap_ovenMode_data->set_ovenMode_value(cap_ovenMode_data, caps_helper_ovenMode.attr_ovenMode.values[oven_idx]);
	//cap_ovenMode_data->attr_ovenMode_send(cap_ovenMode_data);

	//ovenOperatingState
	cap_ovenOperatingState_data->set_ovenJobState_value(cap_ovenOperatingState_data, caps_helper_ovenOperatingState.attr_ovenJobState.values[send_count % 7]);
	cap_ovenOperatingState_data->attr_ovenJobState_send(cap_ovenOperatingState_data);
	char *ctime = "2020-05-14 15";
	cap_ovenOperatingState_data->set_completionTime_value(cap_ovenOperatingState_data, ctime);
	cap_ovenOperatingState_data->attr_completionTime_send(cap_ovenOperatingState_data);
	cap_ovenOperatingState_data->set_progress_value(cap_ovenOperatingState_data, 10 * (send_count % 10)); //% progress
	cap_ovenOperatingState_data->attr_progress_send(cap_ovenOperatingState_data);
	cap_ovenOperatingState_data->set_operationTime_value(cap_ovenOperatingState_data, send_count);
	cap_ovenOperatingState_data->attr_operationTime_send(cap_ovenOperatingState_data);
	cap_ovenOperatingState_data->set_machineState_value(cap_ovenOperatingState_data, caps_helper_ovenOperatingState.attr_machineState.values[send_count % 3]);
	cap_ovenOperatingState_data->attr_machineState_send(cap_ovenOperatingState_data);

	cap_ovenSetpoint_data->set_ovenSetpoint_value(cap_ovenSetpoint_data, 50 + send_count);
	cap_ovenSetpoint_data->attr_ovenSetpoint_send(cap_ovenSetpoint_data);
}

void captest_o_initialize(IOT_CTX *ctx)
{
// CAPTEST : call initialize function and set init_cb
	cap_objectDetection_data = caps_objectDetection_initialize(ctx, "main", cap_objectDetection_init_cb, NULL);
	cap_odorSensor_data = caps_odorSensor_initialize(ctx, "main", cap_odorSensor_init_cb, NULL);
	cap_ovenMode_data = caps_ovenMode_initialize(ctx, "main", cap_ovenMode_init_cb, NULL);
	cap_ovenOperatingState_data = caps_ovenOperatingState_initialize(ctx, "main", cap_ovenOperatingState_init_cb, NULL);
	cap_ovenSetpoint_data = caps_ovenSetpoint_initialize(ctx, "main", cap_ovenSetpoint_init_cb, NULL);

// CAPTEST : set cmd usr cb
	//No cmd_usr_cb for objectDetection
	//No cmd_usr_cb for odorSensor

	cap_ovenMode_data->cmd_setOvenMode_usr_cb = cap_ovenMode_cmd_cb;

	cap_ovenOperatingState_data->cmd_start_usr_cb = cap_ovenOperatingState_cmd_cb;
	cap_ovenOperatingState_data->cmd_stop_usr_cb = cap_ovenOperatingState_start_cb;
	cap_ovenOperatingState_data->cmd_setMachineState_usr_cb = cap_ovenOperatingState_stop_cb;

	cap_ovenSetpoint_data->cmd_setOvenSetpoint_usr_cb = cap_ovenSetpoint_cmd_cb;
}
