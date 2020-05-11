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
#include "caps_accelerationSensor.h"
#include "caps_activityLightingMode.h"
#include "caps_airConditionerMode.h"
#include "caps_airQualitySensor.h"
#include "caps_alarm.h"
#include "caps_audioMute.h"
#include "caps_audioNotification.h"
#include "caps_audioStream.h"
#include "caps_audioTrackData.h"
#include "caps_audioVolume.h"

// CAPTEST :define test capablity variable.
static caps_accelerationSensor_data_t *cap_accelerationSensor_data;
static caps_activityLightingMode_data_t *cap_activityLightingMode_data;
static caps_airConditionerMode_data_t *cap_airConditionerMode_data;
static caps_airQualitySensor_data_t *cap_airQualitySensor_data;
static caps_alarm_data_t *cap_alarm_data;
static caps_audioMute_data_t *cap_audioMute_data;
static caps_audioNotification_data_t *cap_audioNotification_data;
static caps_audioStream_data_t *cap_audioStream_data;
static caps_audioTrackData_data_t *cap_audioTrackData_data;
static caps_audioVolume_data_t *cap_audioVolume_data;

// CAPTEST : make init_cb for test capabilities

static void cap_accelerationSensor_init_cb(struct caps_accelerationSensor_data *caps_data)
{
	caps_data->set_acceleration_value(caps_data, caps_helper_accelerationSensor.attr_acceleration.values[CAP_ENUM_ACCELERATIONSENSOR_ACCELERATION_VALUE_ACTIVE]);
}
static void cap_activityLightingMode_init_cb(struct caps_activityLightingMode_data *caps_data)
{
	caps_data->set_lightingMode_value(caps_data, caps_helper_activityLightingMode.attr_lightingMode.values[CAP_ENUM_ACTIVITYLIGHTINGMODE_LIGHTINGMODE_VALUE_READING]);
}
char *supportedAcModes_supportedMode[] = {"auto", "off", "cool", "warm", "wind"};
static void cap_airConditionerMode_init_cb(struct caps_airConditionerMode_data *caps_data)
{
	caps_data->set_airConditionerMode_value(caps_data, (const char *)supportedAcModes_supportedMode[1]);
	caps_data->set_supportedAcModes_value(caps_data,  (const char **)supportedAcModes_supportedMode, sizeof(supportedAcModes_supportedMode)/sizeof(char *));
}
static void cap_airQualitySensor_init_cb(struct caps_airQualitySensor_data *caps_data)
{
	caps_data->set_airQuality_value(caps_data, caps_helper_airQualitySensor.attr_airQuality.min);
	caps_data->set_airQuality_unit(caps_data, caps_helper_airQualitySensor.attr_airQuality.units[CAP_ENUM_AIRQUALITYSENSOR_AIRQUALITY_UNIT_CAQI]);
}
static void cap_alarm_init_cb(struct caps_alarm_data *caps_data)
{
	caps_data->set_alarm_value(caps_data, caps_helper_alarm.attr_alarm.values[CAP_ENUM_ALARM_ALARM_VALUE_BOTH]);
}
void cap_audioMute_init_cb(struct caps_audioMute_data *caps_data)
{
	caps_data->set_mute_value(caps_data, caps_helper_audioMute.attr_mute.values[CAP_ENUM_AUDIOMUTE_MUTE_VALUE_MUTED]);
}
void cap_audioNotification_init_cb(struct caps_audioNotification_data *caps_data)
{
// no event
}
void cap_audioStream_init_cb(struct caps_audioStream_data *caps_data)
{
	char audio_uri[] = "";
	caps_data->set_uri_value(caps_data, (const char *)audio_uri);
}
void cap_audioTrackData_init_cb(struct caps_audioTrackData_data *caps_data)
{
	char *data = "{\"test\":[\"audio\",\"Track\"]}";
	JSON_H *data_value = cJSON_Parse(data);
	caps_data->set_totalTime_value(caps_data, caps_helper_audioTrackData.attr_totalTime.min);
	caps_data->set_elapsedTime_value(caps_data, caps_helper_audioTrackData.attr_elapsedTime.min);
	caps_data->set_audioTrackData_value(caps_data, data_value);
}
void cap_audioVolume_init_cb(struct caps_audioVolume_data *caps_data)
{
	caps_data->set_volume_value(caps_data, caps_helper_audioVolume.attr_volume.min);
	caps_data->set_volume_unit(caps_data, caps_helper_audioVolume.attr_volume.units[CAP_ENUM_AUDIOVOLUME_VOLUME_UNIT_PERCENT]);
}



// CAPTEST : make cmd_cb for test capabilities
static void cap_accelerationSensor_cmd_cb(struct caps_accelerationSensor_data *caps_data)
{
    const char *value = caps_data->get_acceleration_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_activityLightingMode_cmd_cb(struct caps_activityLightingMode_data *caps_data)
{
	const char *value = caps_data->get_lightingMode_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_airConditionerMode_cmd_cb(struct caps_airConditionerMode_data *caps_data)
{
	const char *value = caps_data->get_airConditionerMode_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_airQualitySensor_cmd_cb(struct caps_airQualitySensor_data *caps_data)
{
	int value = caps_data->get_airQuality_value(caps_data);
	const char *unit = caps_data->get_airQuality_unit(caps_data);
	printf("%s : value : %d (%s)", __func__, value, unit);
}
static void cap_alarm_cmd_cb(struct caps_alarm_data *caps_data)
{
	const char *value = caps_data->get_alarm_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_audioMute_cmd_cb(struct caps_audioMute_data *caps_data)
{
	const char *value = caps_data->get_mute_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_audioNotification_cmd_cb(struct caps_audioNotification_data *caps_data)
{
// no event
}
static void cap_audioStream_cmd_cb(struct caps_audioStream_data *caps_data)
{
	const char *value = caps_data->get_uri_value(caps_data);
	printf("%s : value : %s", __func__, value);
}
static void cap_audioTrackData_cmd_cb(struct caps_audioTrackData_data *caps_data)
{
	int totalTime = caps_data->get_totalTime_value(caps_data);
	int elapsedTime = caps_data->get_elapsedTime_value(caps_data);
	const JSON_H *trackData = caps_data->get_audioTrackData_value(caps_data);
	printf("%s : totalTime elapsedTime: %d %d", __func__, totalTime, elapsedTime);
	//TODO: printf track data
}
static void cap_audioVolume_cmd_cb(struct caps_audioVolume_data *caps_data)
{
	int value = caps_data->get_volume_value(caps_data);
	const char *unit = caps_data->get_volume_unit(caps_data);
	printf("%s : value : %d (%s)", __func__, value, unit);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_a_test_capabilities()
{
	static int send_count = 0;
	send_count++;

	int acceleration_value = send_count % CAP_ENUM_ACCELERATIONSENSOR_ACCELERATION_VALUE_MAX;
	cap_accelerationSensor_data->set_acceleration_value(cap_accelerationSensor_data, caps_helper_accelerationSensor.attr_acceleration.values[acceleration_value]);
	cap_accelerationSensor_data->attr_acceleration_send(cap_accelerationSensor_data);

	int lightingMode = send_count  % CAP_ENUM_ACTIVITYLIGHTINGMODE_LIGHTINGMODE_VALUE_MAX;
	cap_activityLightingMode_data->set_lightingMode_value(cap_activityLightingMode_data, caps_helper_activityLightingMode.attr_lightingMode.values[lightingMode]);
	cap_activityLightingMode_data->attr_lightingMode_send(cap_activityLightingMode_data);

	const char **supportacmodes = cap_airConditionerMode_data->get_supportedAcModes_value(cap_airConditionerMode_data);
	cap_airConditionerMode_data->set_airConditionerMode_value(cap_airConditionerMode_data, supportacmodes[send_count%(sizeof(supportedAcModes_supportedMode)/sizeof(char *))]);
	cap_airConditionerMode_data->attr_airConditionerMode_send(cap_airConditionerMode_data);

	int quality_value = send_count % 100;
	int quality_unit = send_count % CAP_ENUM_AIRQUALITYSENSOR_AIRQUALITY_UNIT_MAX;

	cap_airQualitySensor_data->set_airQuality_value(cap_airQualitySensor_data, quality_value);
	cap_airQualitySensor_data->set_airQuality_unit(cap_airQualitySensor_data, caps_helper_airQualitySensor.attr_airQuality.units[quality_unit]);
	cap_airQualitySensor_data->attr_airQuality_send(cap_airQualitySensor_data);

	int alarm_value = send_count % CAP_ENUM_ALARM_ALARM_VALUE_MAX;
	cap_alarm_data->set_alarm_value(cap_alarm_data, caps_helper_alarm.attr_alarm.values[alarm_value]);
	cap_alarm_data->attr_alarm_send(cap_alarm_data);

	int mute_value = send_count % CAP_ENUM_AUDIOMUTE_MUTE_VALUE_MAX;
	cap_audioMute_data->set_mute_value(cap_audioMute_data, caps_helper_audioMute.attr_mute.values[mute_value]);
	cap_audioMute_data->attr_mute_send(cap_audioMute_data);

	const char send_uri[] = "content://media/external/audio/media/10901";
	cap_audioStream_data->set_uri_value(cap_audioStream_data, send_uri);
	cap_audioStream_data->attr_uri_send(cap_audioStream_data);

	int totalTime = send_count * 100 % 10000000;
	int elapsedTime = send_count * 10 % 10000000;
	char *data = "{\"test\":[\"audio\",\"Track\"]}";
	JSON_H *data_value = cJSON_Parse(data);
	cap_audioTrackData_data->set_totalTime_value(cap_audioTrackData_data, totalTime);
	cap_audioTrackData_data->set_elapsedTime_value(cap_audioTrackData_data, elapsedTime);
	cap_audioTrackData_data->set_audioTrackData_value(cap_audioTrackData_data, data_value);
	cap_audioTrackData_data->attr_audioTrackData_send(cap_audioTrackData_data);

	int volume_value = send_count % 100;
	int volume_unit = send_count % CAP_ENUM_AUDIOVOLUME_VOLUME_UNIT_MAX;

	cap_audioVolume_data->set_volume_value(cap_audioVolume_data, volume_value);
	cap_audioVolume_data->set_volume_unit(cap_audioVolume_data, caps_helper_audioVolume.attr_volume.units[volume_unit]);
	cap_audioVolume_data->attr_volume_send(cap_audioVolume_data);

}

void captest_a_initialize(IOT_CTX *ctx)
{
// CAPTEST : call initialize function and set init_cb
	cap_accelerationSensor_data = caps_accelerationSensor_initialize(ctx, "main", cap_accelerationSensor_init_cb, NULL);
	cap_activityLightingMode_data = caps_activityLightingMode_initialize(ctx, "main", cap_activityLightingMode_init_cb, NULL);
	cap_airConditionerMode_data = caps_airConditionerMode_initialize(ctx, "main", cap_airConditionerMode_init_cb, NULL);
	cap_airQualitySensor_data = caps_airQualitySensor_initialize(ctx, "main", cap_airQualitySensor_init_cb, NULL);
	cap_alarm_data = caps_alarm_initialize(ctx, "main", cap_alarm_init_cb, NULL);
	cap_audioMute_data = caps_audioMute_initialize(ctx, "main", cap_audioMute_init_cb, NULL);
	cap_audioNotification_data = caps_audioNotification_initialize(ctx, "main", cap_audioNotification_init_cb, NULL);
	cap_audioStream_data = caps_audioStream_initialize(ctx, "main", cap_audioStream_init_cb, NULL);
	cap_audioTrackData_data = caps_audioTrackData_initialize(ctx, "main", cap_audioTrackData_init_cb, NULL);
	cap_audioVolume_data = caps_audioVolume_initialize(ctx, "main", cap_audioVolume_init_cb, NULL);


// CAPTEST : set cmd usr cb
	//cap_accelerationSensor_data->cmd_setAccelerationSensor_usr_cb = cap_thermostatMode_cmd_cb;
	cap_activityLightingMode_data->cmd_setLightingMode_usr_cb = cap_activityLightingMode_cmd_cb;
	cap_airConditionerMode_data->cmd_setAirConditionerMode_usr_cb = cap_airConditionerMode_cmd_cb;
	//cap_airQualitySensor_data->cmd_setColor_usr_cb = cap_colorControl_cmd_cb;
	cap_alarm_data->cmd_both_usr_cb = cap_alarm_cmd_cb;
	cap_alarm_data->cmd_siren_usr_cb = cap_alarm_cmd_cb;
	cap_alarm_data->cmd_off_usr_cb = cap_alarm_cmd_cb;
	cap_alarm_data->cmd_strobe_usr_cb = cap_alarm_cmd_cb;

	cap_audioMute_data->cmd_unmute_usr_cb = cap_audioMute_cmd_cb;
	cap_audioMute_data->cmd_setMute_usr_cb = cap_audioMute_cmd_cb;
	cap_audioMute_data->cmd_mute_usr_cb = cap_audioMute_cmd_cb;
	cap_audioNotification_data->cmd_playTrack_usr_cb = cap_audioNotification_cmd_cb;
	cap_audioNotification_data->cmd_playTrackAndRestore_usr_cb = cap_audioNotification_cmd_cb;
	cap_audioNotification_data->cmd_playTrackAndResume_usr_cb = cap_audioNotification_cmd_cb;

	cap_audioNotification_data->cmd_playTrackAndRestore_usr_cb = cap_audioNotification_cmd_cb;
	cap_audioNotification_data->cmd_playTrackAndRestore_usr_cb = cap_audioNotification_cmd_cb;
	cap_audioNotification_data->cmd_playTrackAndRestore_usr_cb = cap_audioNotification_cmd_cb;

	cap_audioStream_data->cmd_stopAudio_usr_cb = cap_audioStream_cmd_cb;
	cap_audioStream_data->cmd_stopAudio_usr_cb = cap_audioStream_cmd_cb;

	cap_audioVolume_data->cmd_volumeDown_usr_cb = cap_audioVolume_cmd_cb;
	cap_audioVolume_data->cmd_volumeUp_usr_cb = cap_audioVolume_cmd_cb;
	cap_audioVolume_data->cmd_setVolume_usr_cb = cap_audioVolume_cmd_cb;

}
