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

#include "caps_fanSpeed.h"
#include "caps_thermostatMode.h"
#include "caps_switchLevel.h"
#include "caps_threeAxis.h"
#include "caps_videoCapture.h"
#include "caps_colorControl.h"
#include "caps_switch.h"
#include "caps_formaldehydeMeasurement.h"

static caps_thermostatMode_data_t *cap_thermostatMode_data;
static caps_switchLevel_data_t *cap_switchLevel_data;
static caps_threeAxis_data_t *cap_threeAxis_data;
static caps_videoCapture_data_t *cap_videoCapture_data;
static caps_colorControl_data_t *cap_colorControl_data;
static caps_switch_data_t *cap_switch_data;
static caps_formaldehydeMeasurement_data_t *cap_formaldehydeMeasurement_data;


// CAPTEST : make init_cb for test capabilities

char *thermostatMode_supportedMode[] = {"auto", "off"};
static void cap_thermostatMode_init_cb(struct caps_thermostatMode_data *caps_data)
{
    caps_data->set_thermostatMode_value(caps_data, caps_helper_thermostatMode.attr_thermostatMode.values[CAP_ENUM_THERMOSTATMODE_THERMOSTATMODE_VALUE_AUTO]);
    caps_data->set_supportedThermostatModes_value(caps_data, (const char **)thermostatMode_supportedMode, sizeof(thermostatMode_supportedMode)/sizeof(char *));
}
static void cap_switchLevel_init_cb(struct caps_switchLevel_data *caps_data)
{
    caps_data->set_level_value(caps_data, 50);
	caps_data->set_level_unit(caps_data, caps_helper_switchLevel.attr_level.units[CAP_ENUM_SWITCHLEVEL_LEVEL_UNIT_PERCENT]);
}
static void cap_threeAxis_init_cb(struct caps_threeAxis_data *caps_data)
{   
    caps_data->set_threeAxis_value(caps_data, 1000, 2000, 3000);
}
static void cap_videoCapture_init_cb(struct caps_videoCapture_data *caps_data)
{   
    //
}
static void cap_colorControl_init_cb(struct caps_colorControl_data *caps_data)
{   
    caps_data->set_color_value(caps_data, 0, 100);
}
void cap_switch_init_cb(struct caps_switch_data *caps_data)
{
	caps_data->set_switch_value(caps_data, caps_helper_switch.attr_switch.values[CAP_ENUM_SWITCH_SWITCH_VALUE_OFF]);
}
void cap_formaldehydeMeasurement_init_cb(struct caps_formaldehydeMeasurement_data *caps_data)
{
	caps_data->set_formaldehydeLevel_value(caps_data, 5000);
	caps_data->set_formaldehydeLevel_unit(caps_data, caps_helper_formaldehydeMeasurement.attr_formaldehydeLevel.units[CAP_ENUM_FORMALDEHYDEMEASUREMENT_FORMALDEHYDELEVEL_UNIT_MG_PER_M3]);
}


// CAPTEST : make cmd_cb for test capabilities
static void cap_thermostatMode_cmd_cb(struct caps_thermostatMode_data *caps_data)
{
    const char *value = caps_data->get_thermostatMode_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_switchLevel_cmd_cb(struct caps_switchLevel_data *caps_data)
{
    int value = caps_data->get_level_value(caps_data);
	printf("%s : value : %d", __func__, value);
}

static void cap_videoCapture_cmd_cb(struct caps_videoCapture_data *caps_data)
{
	printf("%s", __func__);
}
static void cap_colorControl_cmd_cb(struct caps_colorControl_data *caps_data)
{
	double hue = caps_data->get_hue_value(caps_data);
	double saturation = caps_data->get_saturation_value(caps_data);
	printf("%s : %d, %d", __func__, (int)hue, (int)saturation);
}
static void cap_switch_cmd_cb(struct caps_switch_data *caps_data)
{
	const char *value = caps_data->get_switch_value(caps_data);
	printf("%s : value : %s", __func__, value);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	int thermostat_state = send_count % CAP_ENUM_THERMOSTATMODE_THERMOSTATMODE_VALUE_MAX;
	cap_thermostatMode_data->set_thermostatMode_value(cap_thermostatMode_data, caps_helper_thermostatMode.attr_thermostatMode.values[thermostat_state]);
	cap_thermostatMode_data->attr_thermostatMode_send(cap_thermostatMode_data);

	int level_value = (send_count * 25) % 100;
	cap_switchLevel_data->set_level_value(cap_switchLevel_data, level_value);
	cap_switchLevel_data->attr_level_send(cap_switchLevel_data);

	int x = (send_count * 2000) % 10000;
	int y = (send_count * 2000) % 10000;
	int z = (send_count * 2000) % 10000;
	cap_threeAxis_data->set_threeAxis_value(cap_threeAxis_data, x, y, z);
	cap_threeAxis_data->attr_threeAxis_send(cap_threeAxis_data);
	
	//videoCapture

	double hue = (send_count * 25) % 100;
	double saturation = (send_count * 25) % 100;
	cap_colorControl_data->set_color_value(cap_colorControl_data, hue, saturation);
	cap_colorControl_data->attr_color_send(cap_colorControl_data);

	int switch_state = send_count % CAP_ENUM_SWITCH_SWITCH_VALUE_MAX;
	cap_switch_data->set_switch_value(cap_switch_data, caps_helper_switch.attr_switch.values[switch_state]);
	cap_switch_data->attr_switch_send(cap_switch_data);

	int formaldehyde_value = (send_count * 100000 % 1000000);
	cap_formaldehydeMeasurement_data->set_formaldehydeLevel_value(cap_formaldehydeMeasurement_data, formaldehyde_value);
	cap_formaldehydeMeasurement_data->attr_formaldehydeLevel_send(cap_formaldehydeMeasurement_data);

}

void captest_initialize(IOT_CTX *ctx)
{

// CAPTEST : call initialize function and set init_cb
	cap_thermostatMode_data = caps_thermostatMode_initialize(ctx, "main", cap_thermostatMode_init_cb, NULL);
	cap_switchLevel_data = caps_switchLevel_initialize(ctx, "main", cap_switchLevel_init_cb, NULL);
	cap_threeAxis_data = caps_threeAxis_initialize(ctx, "main", cap_threeAxis_init_cb, NULL);
	cap_videoCapture_data = caps_videoCapture_initialize(ctx, "main", cap_videoCapture_init_cb, NULL);
	cap_colorControl_data = caps_colorControl_initialize(ctx, "main", cap_colorControl_init_cb, NULL);
	cap_switch_data = caps_switch_initialize(ctx, "main", cap_switch_init_cb, NULL);
	cap_formaldehydeMeasurement_data = caps_formaldehydeMeasurement_initialize(ctx, "main", cap_formaldehydeMeasurement_init_cb, NULL);

// CAPTEST : set cmd usr cb
	cap_thermostatMode_data->cmd_setThermostatMode_usr_cb = cap_thermostatMode_cmd_cb;
	cap_switchLevel_data->cmd_setLevel_usr_cb = cap_switchLevel_cmd_cb;
	cap_videoCapture_data->cmd_capture_usr_cb = cap_videoCapture_cmd_cb;
	cap_colorControl_data->cmd_setColor_usr_cb = cap_colorControl_cmd_cb;

	cap_switch_data->cmd_on_usr_cb = cap_switch_cmd_cb;
	cap_switch_data->cmd_off_usr_cb = cap_switch_cmd_cb;

}
