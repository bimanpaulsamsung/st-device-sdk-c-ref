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

#include "caps_samsungTV.h"
#include "caps_securitySystem.h"
#include "caps_signalStrength.h"
#include "caps_sleepSensor.h"
#include "caps_smokeDetector.h"
#include "caps_soundPressureLevel.h"
#include "caps_soundSensor.h"
#include "caps_statelessAudioMuteButton.h"
#include "caps_statelessAudioVolumeButton.h"
#include "caps_statelessChannelButton.h"
#include "caps_statelessCustomButton.h"
#include "caps_statelessFanspeedButton.h"
#include "caps_statelessPowerButton.h"
#include "caps_statelessPowerToggleButton.h"
#include "caps_statelessTemperatureButton.h"


static caps_samsungTV_data_t *cap_samsungTV_data;
static caps_securitySystem_data_t *cap_securitySystem_data;
static caps_signalStrength_data_t *cap_signalStrength_data;
static caps_sleepSensor_data_t *cap_sleepSensor_data;
static caps_smokeDetector_data_t *cap_smokeDetector_data;
static caps_soundPressureLevel_data_t *cap_soundPressureLevel_data;
static caps_soundSensor_data_t *cap_soundSensor_data;

static caps_statelessAudioMuteButton_data_t *cap_statelessAudioMuteButton_data;
static caps_statelessAudioVolumeButton_data_t *cap_statelessAudioVolumeButton_data;
static caps_statelessChannelButton_data_t *cap_statelessChannelButton_data;
static caps_statelessCustomButton_data_t *cap_statelessCustomButton_data;
static caps_statelessFanspeedButton_data_t *cap_statelessFanspeedButton_data;
static caps_statelessPowerButton_data_t *cap_statelessPowerButton_data;
static caps_statelessPowerToggleButton_data_t *cap_statelessPowerToggleButton_data;
static caps_statelessTemperatureButton_data_t *cap_statelessTemperatureButton_data;


// CAPTEST : make init_cb for test capabilities
static void cap_samsungTV_init_cb(struct caps_samsungTV_data *caps_data)
{
	caps_data->set_volume_value(caps_data, caps_helper_samsungTV.attr_volume.min);
	JSON_H *msg = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(msg, "message", "hello world!");
	caps_data->set_messageButton_value(caps_data, (const JSON_H*)msg);
	//JSON_DELETE(msg); //do not delete temporarily
	caps_data->set_switch_value(caps_data, (const char*)caps_helper_samsungTV.attr_switch.values[CAP_ENUM_SAMSUNGTV_SWITCH_VALUE_ON]);
	caps_data->set_mute_value(caps_data, (const char*)caps_helper_samsungTV.attr_mute.values[CAP_ENUM_SAMSUNGTV_MUTE_VALUE_UNMUTED]);
	caps_data->set_pictureMode_value(caps_data, (const char*)caps_helper_samsungTV.attr_pictureMode.values[CAP_ENUM_SAMSUNGTV_PICTUREMODE_VALUE_DYNAMIC]);
	caps_data->set_soundMode_value(caps_data, (const char*)caps_helper_samsungTV.attr_soundMode.values[CAP_ENUM_SAMSUNGTV_SOUNDMODE_VALUE_CLEAR_VOICE]);
}

static void cap_securitySystem_init_cb(struct caps_securitySystem_data *caps_data)
{
	caps_data->set_alarm_value(caps_data, "Dangerous");
	caps_data->set_securitySystemStatus_value(caps_data,
		(const char*)caps_helper_securitySystem.attr_securitySystemStatus.values[CAP_ENUM_SECURITYSYSTEM_SECURITYSYSTEMSTATUS_VALUE_ARMEDAWAY]);

}

static void cap_signalStrength_init_cb(struct caps_signalStrength_data *caps_data)
{
	caps_data->set_rssi_value(caps_data, caps_helper_signalStrength.attr_rssi.min);
	caps_data->set_rssi_unit(caps_data, caps_helper_signalStrength.attr_rssi.units[0]);
	caps_data->set_lqi_value(caps_data, caps_helper_signalStrength.attr_lqi.min);
}

static void cap_sleepSensor_init_cb(struct caps_sleepSensor_data *caps_data)
{
	caps_data->set_sleeping_value(caps_data, caps_helper_sleepSensor.attr_sleeping.values[CAP_ENUM_SLEEPSENSOR_SLEEPING_VALUE_NOT_SLEEPING]);
}

static void cap_smokeDetector_init_cb(struct caps_smokeDetector_data *caps_data)
{
	caps_data->set_smoke_value(caps_data, caps_helper_smokeDetector.attr_smoke.values[CAP_ENUM_SMOKEDETECTOR_SMOKE_VALUE_CLEAR]);
}

static void cap_soundPressureLevel_init_cb(struct caps_soundPressureLevel_data *caps_data)
{
	caps_data->set_soundPressureLevel_value(caps_data, caps_helper_soundPressureLevel.attr_soundPressureLevel.min);
	caps_data->set_soundPressureLevel_unit(caps_data, caps_helper_soundPressureLevel.attr_soundPressureLevel.units[0]);
}

static void cap_soundSensor_init_cb(struct caps_soundSensor_data *caps_data)
{
	caps_data->set_sound_value(caps_data, caps_helper_soundSensor.attr_sound.values[CAP_ENUM_SOUNDSENSOR_SOUND_VALUE_DETECTED]);
}

static void cap_statelessAudioMuteButton_init_cb(struct caps_statelessAudioMuteButton_data *caps_data)
{
	caps_data->set_availableAudioMuteButtons_value(caps_data,
		(const char **)caps_helper_statelessAudioMuteButton.attr_availableAudioMuteButtons.values, CAP_ENUM_STATELESSAUDIOMUTEBUTTON_AVAILABLEAUDIOMUTEBUTTONS_VALUE_MAX);
}

static void cap_statelessAudioVolumeButton_init_cb(struct caps_statelessAudioVolumeButton_data *caps_data)
{
	caps_data->set_availableAudioVolumeButtons_value(caps_data,
		(const char **)caps_helper_statelessAudioVolumeButton.attr_availableAudioVolumeButtons.values, CAP_ENUM_STATELESSAUDIOVOLUMEBUTTON_AVAILABLEAUDIOVOLUMEBUTTONS_VALUE_MAX);
}

static void cap_statelessChannelButton_init_cb(struct caps_statelessChannelButton_data *caps_data)
{
	caps_data->set_availableChannelButtons_value(caps_data,
		(const char **)caps_helper_statelessChannelButton.attr_availableChannelButtons.values, CAP_ENUM_STATELESSCHANNELBUTTON_AVAILABLECHANNELBUTTONS_VALUE_MAX);
}

const char *custom_button[] = {"custom1", "custom2"};
static void cap_statelessCustomButton_init_cb(struct caps_statelessCustomButton_data *caps_data)
{
	caps_data->set_availableCustomButtons_value(caps_data, (const char **)custom_button, sizeof(custom_button) / sizeof(char*));
}

static void cap_statelessFanspeedButton_init_cb(struct caps_statelessFanspeedButton_data *caps_data)
{
	caps_data->set_availableFanspeedButtons_value(caps_data,
		(const char **)caps_helper_statelessFanspeedButton.attr_availableFanspeedButtons.values, CAP_ENUM_STATELESSFANSPEEDBUTTON_AVAILABLEFANSPEEDBUTTONS_VALUE_MAX);
}

static void cap_statelessPowerButton_init_cb(struct caps_statelessPowerButton_data *caps_data)
{
	caps_data->set_availablePowerButtons_value(caps_data,
		(const char **)caps_helper_statelessPowerButton.attr_availablePowerButtons.values, CAP_ENUM_STATELESSPOWERBUTTON_AVAILABLEPOWERBUTTONS_VALUE_MAX);
}

static void cap_statelessPowerToggleButton_init_cb(struct caps_statelessPowerToggleButton_data *caps_data)
{
	caps_data->set_availablePowerToggleButtons_value(caps_data,
		(const char **)caps_helper_statelessPowerToggleButton.attr_availablePowerToggleButtons.values, CAP_ENUM_STATELESSPOWERTOGGLEBUTTON_AVAILABLEPOWERTOGGLEBUTTONS_VALUE_MAX);
}

static void cap_statelessTemperatureButton_init_cb(struct caps_statelessTemperatureButton_data *caps_data)
{
	caps_data->set_availableTemperatureButtons_value(caps_data,
		(const char **)caps_helper_statelessTemperatureButton.attr_availableTemperatureButtons.values, CAP_ENUM_STATELESSTEMPERATUREBUTTON_AVAILABLETEMPERATUREBUTTONS_VALUE_MAX);
}


// CAPTEST : make cmd_cb for test capabilities
static void cap_samsungTV_on_cb(struct caps_samsungTV_data *caps_data)
{
	printf("[%s]tv ON\n", __func__);
}

static void cap_samsungTV_off_cb(struct caps_samsungTV_data *caps_data)
{
	printf("[%s]tv OFF\n", __func__);
}

static void cap_samsungTV_mute_cb(struct caps_samsungTV_data *caps_data)
{
	const char* mute = caps_data->get_mute_value(caps_data);
	printf("[%s]tv mute: %s\n", __func__, mute);
}

static void cap_samsungTV_unmute_cb(struct caps_samsungTV_data *caps_data)
{
	const char* mute = caps_data->get_mute_value(caps_data);
	printf("[%s]tv mute: %s\n", __func__, mute);
}

static void cap_samsungTV_pictureMode_cb(struct caps_samsungTV_data *caps_data)
{
	const char* mode = caps_data->get_pictureMode_value(caps_data);
	printf("[%s]tv picture mode: %s\n", __func__, mode);
}

static void cap_samsungTV_soundMode_cb(struct caps_samsungTV_data *caps_data)
{
	const char* mode = caps_data->get_soundMode_value(caps_data);
	printf("[%s]tv sound mode: %s\n", __func__, mode);
}

static void cap_samsungTV_volumeUp_cb(struct caps_samsungTV_data *caps_data)
{
	int vol = caps_data->get_volume_value(caps_data);
	printf("[%s]tv volume: %d\n", __func__, vol);
}

static void cap_samsungTV_volumeDown_cb(struct caps_samsungTV_data *caps_data)
{
	int vol = caps_data->get_volume_value(caps_data);
	printf("[%s]tv volume: %d\n", __func__, vol);
}

static void cap_samsungTV_setVolume_cb(struct caps_samsungTV_data *caps_data)
{
	int vol = caps_data->get_volume_value(caps_data);
	printf("[%s]tv volume: %d\n", __func__, vol);
}

static void cap_samsungTV_message_cb(struct caps_samsungTV_data *caps_data)
{
	const JSON_H *msg = caps_data->get_messageButton_value(caps_data);
	char *str = JSON_PRINT(msg);
	printf("[%s]tv message: %s\n", __func__, str);
	JSON_FREE(str);
}

static void cap_securitySystem_armStay_cb(struct caps_securitySystem_data *caps_data)
{
	const char *alarm = caps_data->get_alarm_value(caps_data);
	const char *status = caps_data->get_securitySystemStatus_value(caps_data);
	printf("[%s]alarm: %s, status: %s\n", __func__, alarm, status);
}

static void cap_securitySystem_disarm_cb(struct caps_securitySystem_data *caps_data)
{
	const char *alarm = caps_data->get_alarm_value(caps_data);
	const char *status = caps_data->get_securitySystemStatus_value(caps_data);
	printf("[%s]alarm: %s, status: %s\n", __func__, alarm, status);
}

static void cap_securitySystem_armAway_cb(struct caps_securitySystem_data *caps_data)
{
	const char *alarm = caps_data->get_alarm_value(caps_data);
	const char *status = caps_data->get_securitySystemStatus_value(caps_data);
	printf("[%s]alarm: %s, status: %s\n", __func__, alarm, status);
}

static void cap_statelessAudioMuteButton_cmd_cb(struct caps_statelessAudioMuteButton_data *caps_data)
{
	printf("[%s]Mute button press\n", __func__);
}

static void cap_statelessAudioVolumeButton_cmd_cb(struct caps_statelessAudioVolumeButton_data *caps_data)
{
	printf("[%s]Volume button press\n", __func__);
}

static void cap_statelessChannelButton_cmd_cb(struct caps_statelessChannelButton_data *caps_data)
{
	printf("[%s]Channel button press\n", __func__);
}

static void cap_statelessCustomButton_cmd_cb(struct caps_statelessCustomButton_data *caps_data)
{
	printf("[%s]Custom button press\n", __func__);
}

static void cap_statelessFanspeedButton_cmd_cb(struct caps_statelessFanspeedButton_data *caps_data)
{
	printf("[%s]Fan speed button press\n", __func__);
}

static void cap_statelessPowerButton_cmd_cb(struct caps_statelessPowerButton_data *caps_data)
{
	printf("[%s]Power button press\n", __func__);
}

static void cap_statelessPowerToggleButton_cmd_cb(struct caps_statelessPowerToggleButton_data *caps_data)
{
	printf("[%s]Power toggle button press\n", __func__);
}

static void cap_statelessTemperatureButton_cmd_cb(struct caps_statelessTemperatureButton_data *caps_data)
{
	printf("[%s]Temperature button press\n", __func__);
}

/*pub_queue could send max 10 attr each time, so we need to divide prefix s capabilities
to several group, and need to test each of them one by one.*/
static int test_group = 3; //could be 1,2,3

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_s_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	//since the limitation of queue depth, we can't send too much data for once.
	if (test_group == 1) {
		cap_samsungTV_data->set_volume_value(cap_samsungTV_data, send_count);
		cap_samsungTV_data->attr_volume_send(cap_samsungTV_data);
		cap_samsungTV_data->set_switch_value(cap_samsungTV_data,
			(const char*)caps_helper_samsungTV.attr_switch.values[send_count % CAP_ENUM_SAMSUNGTV_SWITCH_VALUE_MAX]);
		cap_samsungTV_data->attr_switch_send(cap_samsungTV_data);
		cap_samsungTV_data->set_mute_value(cap_samsungTV_data,
			(const char*)caps_helper_samsungTV.attr_mute.values[send_count % CAP_ENUM_SAMSUNGTV_MUTE_VALUE_MAX]);
		cap_samsungTV_data->attr_mute_send(cap_samsungTV_data);
		cap_samsungTV_data->set_pictureMode_value(cap_samsungTV_data,
			(const char*)caps_helper_samsungTV.attr_pictureMode.values[send_count % CAP_ENUM_SAMSUNGTV_PICTUREMODE_VALUE_MAX]);
		cap_samsungTV_data->attr_pictureMode_send(cap_samsungTV_data);
		cap_samsungTV_data->set_soundMode_value(cap_samsungTV_data,
			(const char*)caps_helper_samsungTV.attr_soundMode.values[send_count % CAP_ENUM_SAMSUNGTV_SOUNDMODE_VALUE_MAX]);
		cap_samsungTV_data->attr_soundMode_send(cap_samsungTV_data);
	} else if (test_group == 2) {
		cap_securitySystem_data->set_alarm_value(cap_securitySystem_data, "Warning!");
		cap_securitySystem_data->attr_alarm_send(cap_securitySystem_data);
		cap_securitySystem_data->set_securitySystemStatus_value(cap_securitySystem_data,
			(const char*)caps_helper_securitySystem.attr_securitySystemStatus.values[send_count % CAP_ENUM_SECURITYSYSTEM_SECURITYSYSTEMSTATUS_VALUE_MAX]);
		cap_securitySystem_data->attr_securitySystemStatus_send(cap_securitySystem_data);

		cap_signalStrength_data->set_rssi_value(cap_signalStrength_data, (double)(send_count * -1));
		cap_signalStrength_data->attr_rssi_send(cap_signalStrength_data);
		cap_signalStrength_data->set_lqi_value(cap_signalStrength_data, (double)send_count);
		cap_signalStrength_data->attr_lqi_send(cap_signalStrength_data);

		cap_sleepSensor_data->set_sleeping_value(cap_sleepSensor_data,
			(const char*)caps_helper_sleepSensor.attr_sleeping.values[send_count % CAP_ENUM_SLEEPSENSOR_SLEEPING_VALUE_MAX]);
		cap_sleepSensor_data->attr_sleeping_send(cap_sleepSensor_data);

		cap_smokeDetector_data->set_smoke_value(cap_smokeDetector_data,
			(const char*)caps_helper_smokeDetector.attr_smoke.values[send_count % CAP_ENUM_SMOKEDETECTOR_SMOKE_VALUE_MAX]);
		cap_smokeDetector_data->attr_smoke_send(cap_smokeDetector_data);

		cap_soundPressureLevel_data->set_soundPressureLevel_value(cap_soundPressureLevel_data, (double)send_count);
		cap_soundPressureLevel_data->attr_soundPressureLevel_send(cap_soundPressureLevel_data);

		cap_soundSensor_data->set_sound_value(cap_soundSensor_data,
			(const char*)caps_helper_soundSensor.attr_sound.values[send_count % CAP_ENUM_SOUNDSENSOR_SOUND_VALUE_MAX]);
		cap_soundSensor_data->attr_sound_send(cap_soundSensor_data);
	} else {
		printf("we don't need to send capabilities for group3\r\n");
	}

}

void captest_s_initialize(IOT_CTX *ctx)
{
	if (test_group == 1) {
		// group 1
		cap_samsungTV_data = caps_samsungTV_initialize(ctx, "main", cap_samsungTV_init_cb, NULL);
		
		cap_samsungTV_data->cmd_on_usr_cb = cap_samsungTV_on_cb;
		cap_samsungTV_data->cmd_off_usr_cb = cap_samsungTV_off_cb;
		cap_samsungTV_data->cmd_mute_usr_cb = cap_samsungTV_mute_cb;
		cap_samsungTV_data->cmd_unmute_usr_cb = cap_samsungTV_unmute_cb;
		cap_samsungTV_data->cmd_setPictureMode_usr_cb = cap_samsungTV_pictureMode_cb;
		cap_samsungTV_data->cmd_setSoundMode_usr_cb = cap_samsungTV_soundMode_cb;
		cap_samsungTV_data->cmd_volumeDown_usr_cb = cap_samsungTV_volumeDown_cb;
		cap_samsungTV_data->cmd_showMessage_usr_cb = cap_samsungTV_message_cb;
		cap_samsungTV_data->cmd_volumeUp_usr_cb = cap_samsungTV_volumeUp_cb;
		cap_samsungTV_data->cmd_setVolume_usr_cb = cap_samsungTV_setVolume_cb;
	} else if (test_group == 2) {
		// group 2
		cap_securitySystem_data = caps_securitySystem_initialize(ctx, "main", cap_securitySystem_init_cb, NULL);
		cap_signalStrength_data = caps_signalStrength_initialize(ctx, "main", cap_signalStrength_init_cb, NULL);
		cap_sleepSensor_data = caps_sleepSensor_initialize(ctx, "main", cap_sleepSensor_init_cb, NULL);
		cap_smokeDetector_data = caps_smokeDetector_initialize(ctx, "main", cap_smokeDetector_init_cb, NULL);
		cap_soundPressureLevel_data = caps_soundPressureLevel_initialize(ctx, "main", cap_soundPressureLevel_init_cb, NULL);
		cap_soundSensor_data = caps_soundSensor_initialize(ctx, "main", cap_soundSensor_init_cb, NULL);

		cap_securitySystem_data->cmd_armStay_usr_cb = cap_securitySystem_armStay_cb;
		cap_securitySystem_data->cmd_disarm_usr_cb = cap_securitySystem_disarm_cb;
		cap_securitySystem_data->cmd_armAway_usr_cb = cap_securitySystem_armAway_cb;
	} else if (test_group == 3) {
		// group 3
		cap_statelessAudioMuteButton_data = caps_statelessAudioMuteButton_initialize(ctx, "main", cap_statelessAudioMuteButton_init_cb, NULL);
		cap_statelessAudioVolumeButton_data = caps_statelessAudioVolumeButton_initialize(ctx, "main", cap_statelessAudioVolumeButton_init_cb, NULL);
		cap_statelessChannelButton_data = caps_statelessChannelButton_initialize(ctx, "main", cap_statelessChannelButton_init_cb, NULL);
		cap_statelessCustomButton_data = caps_statelessCustomButton_initialize(ctx, "main", cap_statelessCustomButton_init_cb, NULL);
		cap_statelessFanspeedButton_data = caps_statelessFanspeedButton_initialize(ctx, "main", cap_statelessFanspeedButton_init_cb, NULL);
		cap_statelessPowerButton_data = caps_statelessPowerButton_initialize(ctx, "main", cap_statelessPowerButton_init_cb, NULL);
		cap_statelessPowerToggleButton_data = caps_statelessPowerToggleButton_initialize(ctx, "main", cap_statelessPowerToggleButton_init_cb, NULL);
		cap_statelessTemperatureButton_data = caps_statelessTemperatureButton_initialize(ctx, "main", cap_statelessTemperatureButton_init_cb, NULL);

		cap_statelessAudioMuteButton_data->cmd_setButton_usr_cb = cap_statelessAudioMuteButton_cmd_cb;
		cap_statelessAudioVolumeButton_data->cmd_setButton_usr_cb = cap_statelessAudioVolumeButton_cmd_cb;
		cap_statelessChannelButton_data->cmd_setButton_usr_cb = cap_statelessChannelButton_cmd_cb;
		cap_statelessCustomButton_data->cmd_setButton_usr_cb = cap_statelessCustomButton_cmd_cb;
		cap_statelessFanspeedButton_data->cmd_setButton_usr_cb = cap_statelessFanspeedButton_cmd_cb;
		cap_statelessPowerButton_data->cmd_setButton_usr_cb = cap_statelessPowerButton_cmd_cb;
		cap_statelessPowerToggleButton_data->cmd_setButton_usr_cb = cap_statelessPowerToggleButton_cmd_cb;
		cap_statelessTemperatureButton_data->cmd_setButton_usr_cb = cap_statelessTemperatureButton_cmd_cb;
	}
}
