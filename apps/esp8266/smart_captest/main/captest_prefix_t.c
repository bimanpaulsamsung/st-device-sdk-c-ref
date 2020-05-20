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

#include "caps_tvocMeasurement.h"
#include "caps_tvChannel.h"
#include "caps_tV.h"
#include "caps_tone.h"
#include "caps_threeAxis.h"
#include "caps_thermostatSetpoint.h"
#include "caps_temperatureAlarm.h"
#include "caps_tamperAlert.h"

static caps_tvocMeasurement_data_t *caps_tvocMeasurement_data;
static caps_tvChannel_data_t *caps_tvChannel_data;
static caps_tV_data_t *caps_tV_data;
static caps_tone_data_t *caps_tone_data;
static caps_threeAxis_data_t *caps_threeAxis_data;
static caps_thermostatSetpoint_data_t *caps_thermostatSetpoint_data;
static caps_temperatureAlarm_data_t *caps_temperatureAlarm_data;
static caps_tamperAlert_data_t *caps_tamperAlert_data;
//tvChannel
#define MAX_CH	4
static char *tVCh[] = {"Ch1", "Ch2", "Ch3", "Ch4"};
static char *tVChName[] = {"tVChName1", "tVChName2", "tVChName3", "tVChName4"};
//tV
#define MAX_CH_VALUE	4
static char *sound_value[] = {"mono", "stereo", "left", "right"};
static char *movie_value[] = {"270p", "480p", "720p", "1080p"};
static char *pic_value[] = {"pix1", "pix2", "pix3", "pix4"};
static char *power_value[] = {"low", "medium", "high", "highest"};


// CAPTEST : make init_cb for test capabilities
static void cap_tvocMeasurement_init_cb(struct caps_tvocMeasurement_data *caps_data)
{
	caps_data->set_tvocLevel_value(caps_data, caps_helper_tvocMeasurement.
						attr_tvocLevel.min);
	caps_data->set_tvocLevel_unit(caps_data, caps_helper_tvocMeasurement.
						attr_tvocLevel.units[CAP_ENUM_TVOCMEASUREMENT_TVOCLEVEL_UNIT_PPM]);
}

static void cap_tvChannel_init_cb(struct caps_tvChannel_data *caps_data)
{
	caps_data->set_tvChannel_value(caps_data, tVCh[0]);
	caps_data->set_tvChannelName_value(caps_data, tVChName[0]);
}

static void cap_tV_init_cb(struct caps_tV_data *caps_data)
{
	caps_data->set_sound_value(caps_data, sound_value[0]);
	caps_data->set_picture_value(caps_data, pic_value[0]);
	caps_data->set_movieMode_value(caps_data, movie_value[0]);
	caps_data->set_power_value(caps_data, power_value[0]);
	caps_data->set_volume_value(caps_data, 0);
	caps_data->set_channel_value(caps_data, 0);

}

static void cap_threeAxis_init_cb(struct caps_threeAxis_data *caps_data)
{
	caps_data->set_threeAxis_value(caps_data, 0, 0, 0);
}

static void cap_tone_init_cb(struct caps_tone_data *caps_data)
{
	printf("[%s] is called\n", __func__);
}

static void cap_thermostatSetpoint_init_cb(struct caps_thermostatSetpoint_data *caps_data)
{
	caps_data->set_thermostatSetpoint_value(caps_data, caps_helper_thermostatSetpoint.
					attr_thermostatSetpoint.min);
	caps_data->set_thermostatSetpoint_unit(caps_data, caps_helper_thermostatSetpoint.
					attr_thermostatSetpoint.units[CAP_ENUM_THERMOSTATSETPOINT_THERMOSTATSETPOINT_UNIT_F]);
}

static void cap_temperatureAlarm_init_cb(struct caps_temperatureAlarm_data *caps_data)
{
	caps_data->set_temperatureAlarm_value(caps_data, caps_helper_temperatureAlarm.
						attr_temperatureAlarm.values[0]);
}

static void cap_tamperAlert_init_cb(struct caps_tamperAlert_data *caps_data)
{
	caps_data->set_tamper_value(caps_data, caps_helper_tamperAlert.
						attr_tamper.values[0]);
}
// CAPTEST : make cmd_cb for test capabilities
static void cap_channelUp_cmd_cb(struct caps_tvChannel_data *caps_data)
{
	printf("[%s] is called\n", __func__);
}

static void cap_channelDown_cmd_cb(struct caps_tvChannel_data *caps_data)
{
	printf("[%s] is called\n", __func__);
}

static void cap_setTvChannelName_cmd_cb(struct caps_tvChannel_data *caps_data)
{
	const char *chname = caps_data->get_tvChannelName_value(caps_data);
	printf("%s: TvChannelName :%s\n",__func__, chname);
}

static void cap_setTvChannel_cmd_cb(struct caps_tvChannel_data *caps_data)
{
	const char *ch = caps_data->get_tvChannel_value(caps_data);
	printf("%s: TvChannel :%s\n",__func__, ch);
}

static void cap_TVchannelUp_cmd_cb(struct caps_tV_data *caps_data)
{
	printf("[%s] is called\n", __func__);
}

static void cap_TVchannelDown_cmd_cb(struct caps_tV_data *caps_data)
{
	printf("[%s] is called\n", __func__);
}

static void cap_volume_cmd_cb(struct caps_tV_data *caps_data)
{
	int volume = caps_data->get_volume_value(caps_data);
	printf("%s: volume :%d\n",__func__, volume);
}

static void cap_beep_cmd_cb(struct caps_tone_data *caps_data)
{
	printf("[%s] is called\n", __func__);
}
// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_t_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	//tvocMeasuremen
	double value = (send_count * 10) % 1000000;
	caps_tvocMeasurement_data->set_tvocLevel_value(caps_tvocMeasurement_data, value);
	int voc_u_idx = send_count % CAP_ENUM_TVOCMEASUREMENT_TVOCLEVEL_UNIT_MAX;
	caps_tvocMeasurement_data->set_tvocLevel_unit(caps_tvocMeasurement_data, caps_helper_tvocMeasurement.
						attr_tvocLevel.units[voc_u_idx]);
	caps_tvocMeasurement_data->attr_tvocLevel_send(caps_tvocMeasurement_data);
	//tvChannel
	int tvch_idx = send_count % MAX_CH;
	caps_tvChannel_data->set_tvChannel_value(caps_tvChannel_data, tVCh[tvch_idx]);
	caps_tvChannel_data->attr_tvChannel_send(caps_tvChannel_data);

	caps_tvChannel_data->set_tvChannelName_value(caps_tvChannel_data, tVChName[tvch_idx]);
	caps_tvChannel_data->attr_tvChannelName_send(caps_tvChannel_data);
	//tV
	int tv_idx = send_count % MAX_CH_VALUE;
	caps_tV_data->set_sound_value(caps_tV_data, sound_value[tv_idx]);
	caps_tV_data->attr_sound_send(caps_tV_data);

	caps_tV_data->set_picture_value(caps_tV_data, pic_value[tv_idx]);
	caps_tV_data->attr_picture_send(caps_tV_data);

	caps_tV_data->set_movieMode_value(caps_tV_data, movie_value[tv_idx]);
	caps_tV_data->attr_movieMode_send(caps_tV_data);

	caps_tV_data->set_power_value(caps_tV_data, power_value[tv_idx]);
	caps_tV_data->attr_power_send(caps_tV_data);

	caps_tV_data->set_volume_value(caps_tV_data, send_count);
	caps_tV_data->attr_volume_send(caps_tV_data);

	caps_tV_data->set_channel_value(caps_tV_data, send_count);
	caps_tV_data->attr_channel_send(caps_tV_data);
	//threeAxis
	caps_threeAxis_data->set_threeAxis_value(caps_threeAxis_data, send_count+1, send_count, send_count-1);
	caps_threeAxis_data->attr_threeAxis_send(caps_threeAxis_data);
	//thermostatSetpoint
	double tvalue = send_count % 10000;
	caps_thermostatSetpoint_data->set_thermostatSetpoint_value(caps_thermostatSetpoint_data, tvalue);

	int tp_idx = send_count % CAP_ENUM_THERMOSTATSETPOINT_THERMOSTATSETPOINT_UNIT_MAX;
	caps_thermostatSetpoint_data->set_thermostatSetpoint_unit(caps_thermostatSetpoint_data, caps_helper_thermostatSetpoint.
						attr_thermostatSetpoint.units[tp_idx]);
	caps_thermostatSetpoint_data->attr_thermostatSetpoint_send(caps_thermostatSetpoint_data);
	//tamperAlert
	int tamper_idx = send_count % CAP_ENUM_TAMPERALERT_TAMPER_VALUE_MAX;
	caps_tamperAlert_data->set_tamper_value(caps_tamperAlert_data, caps_helper_tamperAlert.
										attr_tamper.values[tamper_idx]);
	caps_tamperAlert_data->attr_tamper_send(caps_tamperAlert_data);
	//temperatureAlarm
	int tem_idx = send_count % CAP_ENUM_TEMPERATUREALARM_TEMPERATUREALARM_VALUE_MAX;
	caps_temperatureAlarm_data->set_temperatureAlarm_value(caps_temperatureAlarm_data, caps_helper_temperatureAlarm.
									attr_temperatureAlarm.values[tem_idx]);
	caps_temperatureAlarm_data->attr_temperatureAlarm_send(caps_temperatureAlarm_data);
}

void captest_t_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_tvocMeasurement_data = caps_tvocMeasurement_initialize(ctx, "main", cap_tvocMeasurement_init_cb, NULL);
	caps_tvChannel_data = caps_tvChannel_initialize(ctx, "main", cap_tvChannel_init_cb, NULL);
	caps_tV_data = caps_tV_initialize(ctx, "main", cap_tV_init_cb, NULL);
	caps_tone_data = caps_tone_initialize(ctx, "main", cap_tone_init_cb, NULL);
	caps_threeAxis_data = caps_threeAxis_initialize(ctx, "main", cap_threeAxis_init_cb, NULL);
	caps_thermostatSetpoint_data = caps_thermostatSetpoint_initialize(ctx, "main", cap_thermostatSetpoint_init_cb, NULL);
	caps_temperatureAlarm_data = caps_temperatureAlarm_initialize(ctx, "main", cap_temperatureAlarm_init_cb, NULL);
	caps_tamperAlert_data = caps_tamperAlert_initialize(ctx, "main", cap_tamperAlert_init_cb, NULL);

	// CAPTEST : set cmd usr cb
	caps_tvChannel_data->cmd_channelUp_usr_cb = cap_channelUp_cmd_cb;
	caps_tvChannel_data->cmd_channelDown_usr_cb = cap_channelDown_cmd_cb;
	caps_tvChannel_data->cmd_setTvChannelName_usr_cb = cap_setTvChannelName_cmd_cb;
	caps_tvChannel_data->cmd_setTvChannel_usr_cb = cap_setTvChannel_cmd_cb;

	caps_tV_data->cmd_channelUp_usr_cb = cap_TVchannelUp_cmd_cb;
	caps_tV_data->cmd_volumeDown_usr_cb = cap_volume_cmd_cb;
	caps_tV_data->cmd_volumeUp_usr_cb = cap_volume_cmd_cb;
	caps_tV_data->cmd_channelDown_usr_cb = cap_TVchannelDown_cmd_cb;

	caps_tone_data->cmd_beep_usr_cb = cap_beep_cmd_cb;
}
