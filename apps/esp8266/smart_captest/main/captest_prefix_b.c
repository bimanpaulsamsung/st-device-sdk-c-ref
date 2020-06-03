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

// CAPTEST :add test capability head files.
#include "caps_battery.h"
#include "caps_bodyMassIndexMeasurement.h"
#include "caps_bodyWeightMeasurement.h"
#include "caps_button.h"
#include "caps_bypassable.h"

// CAPTEST :define test capability variable.
static caps_battery_data_t *cap_battery_data;
static caps_bodyMassIndexMeasurement_data_t *cap_bodyMassIndexMeasurement_data;
static caps_bodyWeightMeasurement_data_t *cap_bodyWeightMeasurement_data;
static caps_button_data_t *cap_button_data;
static caps_bypassable_data_t *cap_bypassable_data;

// CAPTEST : make init_cb for test capabilities

static void cap_battery_init_cb(struct caps_battery_data *caps_data)
{
	caps_data->set_battery_value(caps_data, caps_helper_battery.attr_battery.min);
}
static void cap_bodyMassIndexMeasurement_init_cb(struct caps_bodyMassIndexMeasurement_data *caps_data)
{
	caps_data->set_bmiMeasurement_value(caps_data, caps_helper_bodyMassIndexMeasurement.attr_bmiMeasurement.min);
	caps_data->set_bmiMeasurement_unit(caps_data, caps_helper_bodyMassIndexMeasurement.attr_bmiMeasurement.units[CAP_ENUM_BODYMASSINDEXMEASUREMENT_BMIMEASUREMENT_UNIT_KG_PER_M2]);
}
static void cap_bodyWeightMeasurement_init_cb(struct caps_bodyWeightMeasurement_data *caps_data)
{
	caps_data->set_bodyWeightMeasurement_value(caps_data, caps_helper_bodyWeightMeasurement.attr_bodyWeightMeasurement.min);
	caps_data->set_bodyWeightMeasurement_unit(caps_data, caps_helper_bodyWeightMeasurement.attr_bodyWeightMeasurement.units[CAP_ENUM_BODYWEIGHTMEASUREMENT_BODYWEIGHTMEASUREMENT_UNIT_KG]);
}
static void cap_button_init_cb(struct caps_button_data *caps_data)
{
	caps_data->set_button_value(caps_data, caps_helper_button.attr_button.values[CAP_ENUM_BUTTON_BUTTON_VALUE_PUSHED]);
	caps_data->set_supportedButtonValues_value(caps_data, (const char **)caps_helper_button.attr_button.values,
				sizeof(caps_helper_button.attr_button.values)/sizeof(char *));
	caps_data->set_numberOfButtons_value(caps_data, 3);
}
static void cap_bypassable_init_cb(struct caps_bypassable_data *caps_data)
{
	caps_data->set_bypassStatus_value(caps_data, caps_helper_bypassable.attr_bypassStatus.values[CAP_ENUM_BYPASSABLE_BYPASSSTATUS_VALUE_READY]);
}

// CAPTEST : make cmd_cb for test capabilities
//all capabilities have not command.

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_b_test_capabilities()
{
	static int send_count = 0;
	send_count++;

	int battery_value = send_count % 101;
	cap_battery_data->set_battery_value(cap_battery_data, battery_value);
	cap_battery_data->set_battery_unit(cap_battery_data, caps_helper_battery.attr_battery.units[CAP_ENUM_BATTERY_BATTERY_UNIT_PERCENT]);
	cap_battery_data->attr_battery_send(cap_battery_data);
	
	double bmiM_value = send_count* 100/3;
	int bmiM_unit = send_count % CAP_ENUM_BODYMASSINDEXMEASUREMENT_BMIMEASUREMENT_UNIT_MAX;
	cap_bodyMassIndexMeasurement_data->set_bmiMeasurement_value(cap_bodyMassIndexMeasurement_data, bmiM_value);
	cap_bodyMassIndexMeasurement_data->set_bmiMeasurement_unit(cap_bodyMassIndexMeasurement_data,
									caps_helper_bodyMassIndexMeasurement.attr_bmiMeasurement.units[bmiM_unit]);
	cap_bodyMassIndexMeasurement_data->attr_bmiMeasurement_send(cap_bodyMassIndexMeasurement_data);

	double bodyWM_value = send_count* 100/3;
	int bodyWM_unit = send_count % CAP_ENUM_BODYWEIGHTMEASUREMENT_BODYWEIGHTMEASUREMENT_UNIT_MAX;
	cap_bodyWeightMeasurement_data->set_bodyWeightMeasurement_value(cap_bodyWeightMeasurement_data, bodyWM_value);
	cap_bodyWeightMeasurement_data->set_bodyWeightMeasurement_unit(cap_bodyWeightMeasurement_data, caps_helper_bodyWeightMeasurement.attr_bodyWeightMeasurement.units[bodyWM_unit]);
	cap_bodyWeightMeasurement_data->attr_bodyWeightMeasurement_send(cap_bodyWeightMeasurement_data);

	int button_value = send_count % CAP_ENUM_BUTTON_BUTTON_VALUE_MAX;
	cap_button_data->set_button_value(cap_button_data, caps_helper_button.attr_button.values[button_value]);
	int numOfButtons_value = send_count % 10;
	cap_button_data->set_numberOfButtons_value(cap_button_data, numOfButtons_value);
	cap_button_data->attr_button_send(cap_button_data);
	cap_button_data->attr_numberOfButtons_send(cap_button_data);

	int bypass_value = send_count % CAP_ENUM_BYPASSABLE_BYPASSSTATUS_VALUE_MAX;
	cap_bypassable_data->set_bypassStatus_value(cap_bypassable_data, caps_helper_bypassable.attr_bypassStatus.values[bypass_value]);
	cap_bypassable_data->attr_bypassStatus_send(cap_bypassable_data);
}

void captest_b_initialize(IOT_CTX *ctx)
{
// CAPTEST : call initialize function and set init_cb
	cap_battery_data = caps_battery_initialize(ctx, "main", cap_battery_init_cb, NULL);
	cap_bodyMassIndexMeasurement_data = caps_bodyMassIndexMeasurement_initialize(ctx, "main", cap_bodyMassIndexMeasurement_init_cb, NULL);
	cap_bodyWeightMeasurement_data = caps_bodyWeightMeasurement_initialize(ctx, "main", cap_bodyWeightMeasurement_init_cb, NULL);
	cap_button_data = caps_button_initialize(ctx, "main", cap_button_init_cb, NULL);
	cap_bypassable_data = caps_bypassable_initialize(ctx, "main", cap_bypassable_init_cb, NULL);

// CAPTEST : set cmd usr cb
	//all capabilities have not command.
}
