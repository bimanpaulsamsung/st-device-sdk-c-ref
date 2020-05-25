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

#include "caps_panicAlarm.h"
#include "caps_pHMeasurement.h"
#include "caps_powerMeter.h"
#include "caps_powerSource.h"
#include "caps_presenceSensor.h"
#include "caps_rapidCooling.h"


static caps_panicAlarm_data_t *cap_panicAlarm_data;
static caps_pHMeasurement_data_t *cap_pHMeasurement_data;
static caps_powerMeter_data_t *cap_powerMeter_data;
static caps_powerSource_data_t *cap_powerSource_data;
static caps_presenceSensor_data_t *cap_presenceSensor_data;


// CAPTEST : make init_cb for test capabilities
static void cap_panicAlarm_init_cb(struct caps_panicAlarm_data *caps_data)
{   
	caps_data->set_panicAlarm_value(caps_data, (const char*)caps_helper_panicAlarm.attr_panicAlarm.values[CAP_ENUM_PANICALARM_PANICALARM_VALUE_CLEAR]);
}

static void cap_pHMeasurement_init_cb(struct caps_pHMeasurement_data *caps_data)
{
	caps_data->set_pH_unit(caps_data, (const char*)caps_helper_pHMeasurement.attr_pH.units[CAP_ENUM_PHMEASUREMENT_PH_UNIT_PH]);
	caps_data->set_pH_value(caps_data, caps_helper_pHMeasurement.attr_pH.min);
}

static void cap_powerMeter_init_cb(struct caps_powerMeter_data *caps_data)
{
	caps_data->set_power_unit(caps_data, (const char*)caps_helper_powerMeter.attr_power.units[CAP_ENUM_POWERMETER_POWER_UNIT_W]);
	caps_data->set_power_value(caps_data, caps_helper_powerMeter.attr_power.min);
}

static void cap_powerSource_init_cb(struct caps_powerSource_data *caps_data)
{
	caps_data->set_powerSource_value(caps_data, (const char*)caps_helper_powerSource.attr_powerSource.values[CAP_ENUM_POWERSOURCE_POWERSOURCE_VALUE_BATTERY]);
}

static void cap_presenceSensor_init_cb(struct caps_presenceSensor_data *caps_data)
{
	caps_data->set_presence_value(caps_data, (const char*)caps_helper_presenceSensor.attr_presence.values[CAP_ENUM_PRESENCESENSOR_PRESENCE_VALUE_PRESENT]);
}

// CAPTEST : make cmd_cb for test capabilities

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_p_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	cap_panicAlarm_data->set_panicAlarm_value(cap_panicAlarm_data, (const char*)caps_helper_panicAlarm.attr_panicAlarm.values[send_count % CAP_ENUM_PANICALARM_PANICALARM_VALUE_MAX]);
	cap_panicAlarm_data->attr_panicAlarm_send(cap_panicAlarm_data);

	cap_pHMeasurement_data->set_pH_value(cap_pHMeasurement_data, (double)(7 + (send_count * 0.1)));
	cap_pHMeasurement_data->attr_pH_send(cap_pHMeasurement_data);

	cap_powerMeter_data->set_power_value(cap_powerMeter_data, send_count);
	cap_powerMeter_data->attr_power_send(cap_powerMeter_data);

	cap_powerSource_data->set_powerSource_value(cap_powerSource_data, (const char*)caps_helper_powerSource.attr_powerSource.values[send_count % CAP_ENUM_POWERSOURCE_POWERSOURCE_VALUE_MAX]);
	cap_powerSource_data->attr_powerSource_send(cap_powerSource_data);

	cap_presenceSensor_data->set_presence_value(cap_presenceSensor_data, (const char*)caps_helper_presenceSensor.attr_presence.values[send_count % CAP_ENUM_PRESENCESENSOR_PRESENCE_VALUE_MAX]);
	cap_presenceSensor_data->attr_presence_send(cap_presenceSensor_data);
}

void captest_p_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	cap_panicAlarm_data = caps_panicAlarm_initialize(ctx, "main", cap_panicAlarm_init_cb, NULL);
	cap_pHMeasurement_data = caps_pHMeasurement_initialize(ctx, "main", cap_pHMeasurement_init_cb, NULL);
	cap_powerMeter_data = caps_powerMeter_initialize(ctx, "main", cap_powerMeter_init_cb, NULL);
	cap_powerSource_data = caps_powerSource_initialize(ctx, "main", cap_powerSource_init_cb, NULL);
	cap_presenceSensor_data = caps_presenceSensor_initialize(ctx, "main", cap_presenceSensor_init_cb, NULL);

	// CAPTEST : set cmd usr cb
	//panicAlarm has no cmd cb
	//pHMeasurement has no cmd cb
	//powerMeter has no cmd cb
	//powerSource has no cmd cb
	//presenceSensor has no cmd cb

}
