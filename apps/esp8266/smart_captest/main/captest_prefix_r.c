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

#include "caps_rapidCooling.h"
#include "caps_refresh.h"
#include "caps_refrigeration.h"
#include "caps_refrigerationSetpoint.h"
#include "caps_relativeHumidityMeasurement.h"
#include "caps_remoteControlStatus.h"
#include "caps_robotCleanerCleaningMode.h"
#include "caps_robotCleanerMovement.h"
#include "caps_robotCleanerTurboMode.h"

static caps_rapidCooling_data_t *cap_rapidCooling_data;
static caps_refresh_data_t *cap_refresh_data;
static caps_refrigeration_data_t *cap_refrigeration_data;
static caps_refrigerationSetpoint_data_t *cap_refrigerationSetpoint_data;
static caps_relativeHumidityMeasurement_data_t *cap_relativeHumidityMeasurement_data;
static caps_remoteControlStatus_data_t *cap_remoteControlStatus_data;
static caps_robotCleanerCleaningMode_data_t *cap_robotCleanerCleaningMode_data;
static caps_robotCleanerMovement_data_t *cap_robotCleanerMovement_data;
static caps_robotCleanerTurboMode_data_t *cap_robotCleanerTurboMode_data;


// CAPTEST : make init_cb for test capabilities
static void cap_rapidCooling_init_cb(struct caps_rapidCooling_data *caps_data)
{
	caps_data->set_rapidCooling_value(caps_data, (const char*)caps_helper_rapidCooling.attr_rapidCooling.values[CAP_ENUM_RAPIDCOOLING_RAPIDCOOLING_VALUE_OFF]);
}

static void cap_refresh_init_cb(struct caps_refresh_data *caps_data)
{
	printf("[%s]", __func__);
}

static void cap_refrigeration_init_cb(struct caps_refrigeration_data *caps_data)
{
	caps_data->set_defrost_value(caps_data, (const char*)caps_helper_refrigeration.attr_defrost.values[CAP_ENUM_REFRIGERATION_DEFROST_VALUE_OFF]);
	caps_data->set_rapidFreezing_value(caps_data, (const char*)caps_helper_refrigeration.attr_rapidFreezing.values[CAP_ENUM_REFRIGERATION_RAPIDFREEZING_VALUE_OFF]);
	caps_data->set_rapidCooling_value(caps_data, (const char*)caps_helper_refrigeration.attr_rapidCooling.values[CAP_ENUM_REFRIGERATION_RAPIDCOOLING_VALUE_ON]);
}

static void cap_refrigerationSetpoint_init_cb(struct caps_refrigerationSetpoint_data *caps_data)
{
	caps_data->set_refrigerationSetpoint_value(caps_data, caps_helper_refrigerationSetpoint.attr_refrigerationSetpoint.min);
	caps_data->set_refrigerationSetpoint_unit(caps_data,
		(const char*)caps_helper_refrigerationSetpoint.attr_refrigerationSetpoint.units[CAP_ENUM_REFRIGERATIONSETPOINT_REFRIGERATIONSETPOINT_UNIT_F]);
}

static void cap_relativeHumidityMeasurement_init_cb(struct caps_relativeHumidityMeasurement_data *caps_data)
{
	caps_data->set_humidity_value(caps_data, caps_helper_relativeHumidityMeasurement.attr_humidity.min);
	caps_data->set_humidity_unit(caps_data,
		(const char*)caps_helper_relativeHumidityMeasurement.attr_humidity.units[CAP_ENUM_RELATIVEHUMIDITYMEASUREMENT_HUMIDITY_UNIT_PERCENT]);
}

static void cap_remoteControlStatus_init_cb(struct caps_remoteControlStatus_data *caps_data)
{
	caps_data->set_remoteControlEnabled_value(caps_data,
		(const char*)caps_helper_remoteControlStatus.attr_remoteControlEnabled.values[CAP_ENUM_REMOTECONTROLSTATUS_REMOTECONTROLENABLED_VALUE_FALSE]);
}

static void cap_robotCleanerCleaningMode_init_cb(struct caps_robotCleanerCleaningMode_data *caps_data)
{
	caps_data->set_robotCleanerCleaningMode_value(caps_data,
		(const char*)caps_helper_robotCleanerCleaningMode.attr_robotCleanerCleaningMode.values[CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_AUTO]);
}

static void cap_robotCleanerMovement_init_cb(struct caps_robotCleanerMovement_data *caps_data)
{
	caps_data->set_robotCleanerMovement_value(caps_data,
		(const char*)caps_helper_robotCleanerMovement.attr_robotCleanerMovement.values[CAP_ENUM_ROBOTCLEANERMOVEMENT_ROBOTCLEANERMOVEMENT_VALUE_HOMING]);
}

static void cap_robotCleanerTurboMode_init_cb(struct caps_robotCleanerTurboMode_data *caps_data)
{
	caps_data->set_robotCleanerTurboMode_value(caps_data,
		(const char*)caps_helper_robotCleanerTurboMode.attr_robotCleanerTurboMode.values[CAP_ENUM_ROBOTCLEANERTURBOMODE_ROBOTCLEANERTURBOMODE_VALUE_ON]);
}

// CAPTEST : make cmd_cb for test capabilities
static void cap_rapidCooling_cmd_cb(struct caps_rapidCooling_data *caps_data)
{
	const char* cooling = caps_data->get_rapidCooling_value(caps_data);
	printf("[%s]rapid cooling: %s\n", __func__, cooling);
}

static void cap_refresh_cmd_cb(struct caps_refresh_data *caps_data)
{
	printf("[%s]", __func__);
}

static void cap_refrigeration_setDefrost_cb(struct caps_refrigeration_data *caps_data)
{
	const char* defrost = caps_data->get_defrost_value(caps_data);
	printf("[%s]defrost: %s\n", __func__, defrost);
}

static void cap_refrigeration_setRapidFreezing_cb(struct caps_refrigeration_data *caps_data)
{
	const char* freezing = caps_data->get_rapidFreezing_value(caps_data);
	printf("[%s]freezing: %s\n", __func__, freezing);
}

static void cap_refrigeration_setRapidCooling_cb(struct caps_refrigeration_data *caps_data)
{
	const char* rapid_cooling = caps_data->get_rapidCooling_value(caps_data);
	printf("[%s]rapid_cooling: %s\n", __func__, rapid_cooling);
}

static void cap_refrigerationSetpoint_cmd_cb(struct caps_refrigerationSetpoint_data *caps_data)
{
	double temp = caps_data->get_refrigerationSetpoint_value(caps_data);
	const char* unit = caps_data->get_refrigerationSetpoint_unit(caps_data);
	printf("[%s]temperature: %f [%s]\n", __func__, temp, unit);
}

static void cap_robotCleanerCleaningMode_cmd_cb(struct caps_robotCleanerCleaningMode_data *caps_data)
{
	const char* mode = caps_data->get_robotCleanerCleaningMode_value(caps_data);
	printf("[%s]cleaning mode: %s\n", __func__, mode);
}

static void cap_robotCleanerMovement_cmd_cb(struct caps_robotCleanerMovement_data *caps_data)
{
	const char* move = caps_data->get_robotCleanerMovement_value(caps_data);
	printf("[%s]movement : %s\n", __func__, move);
}

static void cap_robotCleanerTurboMode_cmd_cb(struct caps_robotCleanerTurboMode_data *caps_data)
{
	const char* turbo = caps_data->get_robotCleanerTurboMode_value(caps_data);
	printf("[%s]Turbo mode : %s\n", __func__, turbo);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_r_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	cap_rapidCooling_data->set_rapidCooling_value(cap_rapidCooling_data,
		(const char*)caps_helper_rapidCooling.attr_rapidCooling.values[send_count % CAP_ENUM_RAPIDCOOLING_RAPIDCOOLING_VALUE_MAX]);
	cap_rapidCooling_data->attr_rapidCooling_send(cap_rapidCooling_data);

	cap_refrigeration_data->set_defrost_value(cap_refrigeration_data,
		(const char*)caps_helper_refrigeration.attr_defrost.values[send_count % CAP_ENUM_REFRIGERATION_DEFROST_VALUE_MAX]);
	cap_refrigeration_data->attr_defrost_send(cap_refrigeration_data);
	cap_refrigeration_data->set_rapidFreezing_value(cap_refrigeration_data,
		(const char*)caps_helper_refrigeration.attr_rapidFreezing.values[send_count % CAP_ENUM_REFRIGERATION_RAPIDFREEZING_VALUE_MAX]);
	cap_refrigeration_data->attr_rapidFreezing_send(cap_refrigeration_data);
	cap_refrigeration_data->set_rapidCooling_value(cap_refrigeration_data,
		(const char*)caps_helper_refrigeration.attr_rapidCooling.values[send_count % CAP_ENUM_REFRIGERATION_RAPIDCOOLING_VALUE_MAX]);
	cap_refrigeration_data->attr_rapidCooling_send(cap_refrigeration_data);

	cap_refrigerationSetpoint_data->set_refrigerationSetpoint_value(cap_refrigerationSetpoint_data, (double)(send_count * 2));
	cap_refrigerationSetpoint_data->set_refrigerationSetpoint_unit(cap_refrigerationSetpoint_data,
		(const char*)caps_helper_refrigerationSetpoint.attr_refrigerationSetpoint.units[send_count % CAP_ENUM_REFRIGERATIONSETPOINT_REFRIGERATIONSETPOINT_UNIT_MAX]);
	cap_refrigerationSetpoint_data->attr_refrigerationSetpoint_send(cap_refrigerationSetpoint_data);

	cap_relativeHumidityMeasurement_data->set_humidity_value(cap_relativeHumidityMeasurement_data, (double)(send_count * 2));
	cap_relativeHumidityMeasurement_data->attr_humidity_send(cap_relativeHumidityMeasurement_data);

	cap_remoteControlStatus_data->set_remoteControlEnabled_value(cap_remoteControlStatus_data,
		(const char*)caps_helper_remoteControlStatus.attr_remoteControlEnabled.values[send_count % CAP_ENUM_REMOTECONTROLSTATUS_REMOTECONTROLENABLED_VALUE_MAX]);
	cap_remoteControlStatus_data->attr_remoteControlEnabled_send(cap_remoteControlStatus_data);

	cap_robotCleanerCleaningMode_data->set_robotCleanerCleaningMode_value(cap_robotCleanerCleaningMode_data,
		(const char*)caps_helper_robotCleanerCleaningMode.attr_robotCleanerCleaningMode.values[send_count % CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_MAX]);
	cap_robotCleanerCleaningMode_data->attr_robotCleanerCleaningMode_send(cap_robotCleanerCleaningMode_data);

	cap_robotCleanerMovement_data->set_robotCleanerMovement_value(cap_robotCleanerMovement_data,
		(const char*)caps_helper_robotCleanerMovement.attr_robotCleanerMovement.values[send_count % CAP_ENUM_ROBOTCLEANERMOVEMENT_ROBOTCLEANERMOVEMENT_VALUE_MAX]);
	cap_robotCleanerMovement_data->attr_robotCleanerMovement_send(cap_robotCleanerMovement_data);

	cap_robotCleanerTurboMode_data->set_robotCleanerTurboMode_value(cap_robotCleanerTurboMode_data,
		(const char*)caps_helper_robotCleanerTurboMode.attr_robotCleanerTurboMode.values[send_count % CAP_ENUM_ROBOTCLEANERTURBOMODE_ROBOTCLEANERTURBOMODE_VALUE_MAX]);
	cap_robotCleanerTurboMode_data->attr_robotCleanerTurboMode_send(cap_robotCleanerTurboMode_data);

}


void captest_r_initialize(IOT_CTX *ctx)
{
// CAPTEST : call initialize function and set init_cb
	cap_rapidCooling_data = caps_rapidCooling_initialize(ctx, "main", cap_rapidCooling_init_cb, NULL);
	cap_refresh_data = caps_refresh_initialize(ctx, "main", cap_refresh_init_cb, NULL);
	cap_refrigeration_data = caps_refrigeration_initialize(ctx, "main", cap_refrigeration_init_cb, NULL);
	cap_refrigerationSetpoint_data = caps_refrigerationSetpoint_initialize(ctx, "main", cap_refrigerationSetpoint_init_cb, NULL);
	cap_relativeHumidityMeasurement_data = caps_relativeHumidityMeasurement_initialize(ctx, "main", cap_relativeHumidityMeasurement_init_cb, NULL);
	cap_remoteControlStatus_data = caps_remoteControlStatus_initialize(ctx, "main", cap_remoteControlStatus_init_cb, NULL);
	cap_robotCleanerCleaningMode_data = caps_robotCleanerCleaningMode_initialize(ctx, "main", cap_robotCleanerCleaningMode_init_cb, NULL);
	cap_robotCleanerMovement_data = caps_robotCleanerMovement_initialize(ctx, "main", cap_robotCleanerMovement_init_cb, NULL);
	cap_robotCleanerTurboMode_data = caps_robotCleanerTurboMode_initialize(ctx, "main", cap_robotCleanerTurboMode_init_cb, NULL);

// CAPTEST : set cmd usr cb
	cap_rapidCooling_data->cmd_setRapidCooling_usr_cb = cap_rapidCooling_cmd_cb;
	cap_refresh_data->cmd_refresh_usr_cb = cap_refresh_cmd_cb;

	cap_refrigeration_data->cmd_setRapidCooling_usr_cb = cap_refrigeration_setRapidCooling_cb;
	cap_refrigeration_data->cmd_setRapidFreezing_usr_cb = cap_refrigeration_setRapidFreezing_cb;
	cap_refrigeration_data->cmd_setDefrost_usr_cb = cap_refrigeration_setDefrost_cb;

	cap_refrigerationSetpoint_data->cmd_setRefrigerationSetpoint_usr_cb = cap_refrigerationSetpoint_cmd_cb;

	cap_robotCleanerCleaningMode_data->cmd_setRobotCleanerCleaningMode_usr_cb = cap_robotCleanerCleaningMode_cmd_cb;

	cap_robotCleanerMovement_data->cmd_setRobotCleanerMovement_usr_cb = cap_robotCleanerMovement_cmd_cb;

	cap_robotCleanerTurboMode_data->cmd_setRobotCleanerTurboMode_usr_cb = cap_robotCleanerTurboMode_cmd_cb;

}
