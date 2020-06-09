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
#include "caps_airConditionerMode.h"
#include "caps_lock.h"

#include "caps_elevatorCall.h"
#include "caps_gasMeter.h"
#include "caps_imageCapture.h"
#include "caps_videoStream.h"
#include "caps_videoCapture.h"
#include "caps_videoCamera.h"
#include "caps_washerOperatingState.h"
#include "caps_windowShadePreset.h"

#include "caps_mediaPlayback.h"
#include "caps_ovenSetpoint.h"
#include "caps_pHMeasurement.h"
#include "caps_robotCleanerCleaningMode.h"

// CAPTEST :define test capability variable.
static caps_airConditionerMode_data_t *cap_airConditionerMode_data;
static caps_lock_data_t *cap_lock_data;

static caps_elevatorCall_data_t *cap_elevatorCall_data;
static caps_gasMeter_data_t *cap_gasMeter_data;
static caps_imageCapture_data_t *cap_imageCapture_data;
static caps_videoCamera_data_t *cap_videoCamera_data;
static caps_videoCapture_data_t *cap_videoCapture_data;
static caps_videoStream_data_t *cap_videoStream_data;
static caps_washerOperatingState_data_t *cap_washerOperatingState_data;
static caps_windowShadePreset_data_t *cap_windowShadePreset_data;

static caps_mediaPlayback_data_t *cap_mediaPlayback_data;
static caps_ovenSetpoint_data_t *cap_ovenSetpoint_data;
static caps_pHMeasurement_data_t *cap_pHMeasurement_data;
static caps_robotCleanerCleaningMode_data_t *cap_robotCleanerCleaningMode_data;

enum {
		LOCKED,
		UNKNOWN,
		UNLOCKED,
		UNLOCKED_TO,
		LOCK_MAX
};
#define INIT_GASMETER_TIME "2020-05-15 14:14:15"
#define INIT_IMAGE_CAPTURE_VALUE	"test_image_capture_value"
#define INIT_IMAGE_CAPTURE_TIME		"2020-05-15 13:13:14"
#define INIT_TEST_FOR_CAMERA_STRING "test camera status message value"
#define TEST_COMPLETION_TIME	"2020-05-13 09:50:10"
#define MEDIA_PLAYBACK_TEST		"stopped"
static char *statusMessage[] = {"unmute", "on", "off", "filp", "mute"};
static const char* lock_mode_test[] = {"locked", "unknown", "unlocked", "unlocked_with_timeout"};
static const char* mediaPlayback_array[] = {"paused", "playing", "stopped", "fast_forwarding", "rewinding"};

// CAPTEST : make init_cb for test capabilities

char *supportedAcModes_supportedMode[] = {"auto", "off", "cool", "warm", "wind"};
static void cap_airConditionerMode_init_cb(struct caps_airConditionerMode_data *caps_data)
{
	caps_data->set_airConditionerMode_value(caps_data, (const char *)supportedAcModes_supportedMode[1]);
	caps_data->set_supportedAcModes_value(caps_data,  (const char **)supportedAcModes_supportedMode, sizeof(supportedAcModes_supportedMode)/sizeof(char *));
}
static void cap_lock_init_cb(struct caps_lock_data *caps_data)
{
    caps_data->set_lock_value(caps_data, lock_mode_test[UNLOCKED]);
}

static void cap_elevatorCall_init_cb(struct caps_elevatorCall_data *caps_data)
{
	caps_data->set_callStatus_value(caps_data,
				caps_helper_elevatorCall.attr_callStatus.values[CAP_ENUM_ELEVATORCALL_CALLSTATUS_VALUE_CALLED]);
}

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

static void cap_imageCapture_init_cb(struct caps_imageCapture_data *caps_data)
{
	caps_data->set_image_value(caps_data, INIT_IMAGE_CAPTURE_VALUE);
	caps_data->set_captureTime_value(caps_data, INIT_IMAGE_CAPTURE_TIME);
}

static void cap_videoCamera_init_cb(struct caps_videoCamera_data *caps_data)
{
	caps_data->set_mute_value(caps_data, caps_helper_videoCamera.attr_mute.
							values[CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MUTED]);
	caps_data->set_camera_value(caps_data, caps_helper_videoCamera.attr_camera.
							values[CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_OFF]);
	caps_data->set_statusMessage_value(caps_data, INIT_TEST_FOR_CAMERA_STRING);
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "camera", "test for camera");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", 0);
	caps_data->set_settings_value(caps_data, root);
}

static void cap_videoCapture_init_cb(struct caps_videoCapture_data *caps_data)
{
	JSON_H *clip_value = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(clip_value, "clip", "test for clip");
	JSON_ADD_NUMBER_TO_OBJECT(clip_value, "count", 0);
	caps_data->set_clip_value(caps_data, clip_value);

	JSON_H *stream_value = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(stream_value, "stream", "test for stream");
	JSON_ADD_NUMBER_TO_OBJECT(stream_value, "count", 0);
	caps_data->set_stream_value(caps_data, stream_value);
}

static void cap_videoStream_init_cb(struct caps_videoStream_data *caps_data)
{
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "test for videoStream");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", 0);
	caps_data->set_stream_value(caps_data, root);
}

const char *support_Ms[]={"testST1", "testST2", "testST3"};
static void cap_washerOperatingState_init_cb(struct caps_washerOperatingState_data *caps_data)
{
	caps_data->set_machineState_value(caps_data, caps_helper_washerOperatingState.
									attr_machineState.values[CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_PAUSE]);
	caps_data->set_completionTime_value(caps_data, TEST_COMPLETION_TIME);
	caps_data->set_supportedMachineStates_value(caps_data, (const char **)support_Ms, sizeof(support_Ms)/sizeof(char *));
	caps_data->set_washerJobState_value(caps_data, caps_helper_washerOperatingState.
									attr_washerJobState.values[CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_AIRWASH]);
}

static void cap_windowShadePreset_init_cb(struct caps_windowShadePreset_data *caps_data)
{
	//todo
}

static void cap_mediaPlayback_init_cb(struct caps_mediaPlayback_data *caps_data)
{
	caps_data->set_playbackStatus_value(caps_data, MEDIA_PLAYBACK_TEST);
	caps_data->set_supportedPlaybackCommands_value(caps_data, mediaPlayback_array, CAP_ENUM_MEDIAPLAYBACK_PLAYBACKSTATUS_VALUE_MAX);
}

static void cap_ovenSetpoint_init_cb(struct caps_ovenSetpoint_data *caps_data)
{   
	caps_data->set_ovenSetpoint_value(caps_data, 50);
}

static void cap_pHMeasurement_init_cb(struct caps_pHMeasurement_data *caps_data)
{
	caps_data->set_pH_unit(caps_data, (const char*)caps_helper_pHMeasurement.attr_pH.units[CAP_ENUM_PHMEASUREMENT_PH_UNIT_PH]);
	caps_data->set_pH_value(caps_data, caps_helper_pHMeasurement.attr_pH.min);
}

static void cap_robotCleanerCleaningMode_init_cb(struct caps_robotCleanerCleaningMode_data *caps_data)
{
	caps_data->set_robotCleanerCleaningMode_value(caps_data,
		(const char*)caps_helper_robotCleanerCleaningMode.attr_robotCleanerCleaningMode.values[CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_AUTO]);
}


// CAPTEST : make cmd_cb for test capabilities
static void cap_airConditionerMode_cmd_cb(struct caps_airConditionerMode_data *caps_data)
{
	const char *value = caps_data->get_airConditionerMode_value(caps_data);
	printf("%s : value : %s", __func__, value);
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

static void cap_elevatorCall_cmd_cb(struct caps_elevatorCall_data *caps_data)
{
	const char *value = caps_data->get_callStatus_value(caps_data);
	printf("%s: value:%s\n",__func__,value);
}

static void cap_imageCapture_cmd_cb(struct caps_imageCapture_data *caps_data)
{
	const char *image_value = caps_data->get_image_value(caps_data);
	const char *capture_time = caps_data->get_captureTime_value(caps_data);
	printf("[%s] image value is : %s\n", __func__, image_value);
	printf("[%s] capture time is : %s\n", __func__, capture_time);
}

static void cap_unmute_cmd_cb(struct caps_videoCamera_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_on_cmd_cb(struct caps_videoCamera_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_off_cmd_cb(struct caps_videoCamera_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_flip_cmd_cb(struct caps_videoCamera_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_mute_cmd_cb(struct caps_videoCamera_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_capture_cmd_cb(struct caps_videoCapture_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_videoStream_stop_cmd_cb(struct caps_videoStream_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_videoStream_start_cmd_cb(struct caps_videoStream_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_washerOperatingState_cmd_cb(struct caps_washerOperatingState_data *caps_data)
{
	const char *completionTime_value = caps_data->get_completionTime_value(caps_data);
	const char *m_state = caps_data->get_machineState_value(caps_data);
	const char *job_state = caps_data->get_washerJobState_value(caps_data);

	printf("%s: completionTime :%s\n",__func__, completionTime_value);
	printf("%s: machineState :%s\n",__func__, m_state);
	printf("%s: jobstate :%s\n",__func__, job_state);
}

static void cap_windowShadePreset_cmd_cb(struct caps_windowShadePreset_data *caps_data)
{
	//todo
}

static void cap_mediaPlayback_setStatus_cb(struct caps_mediaPlayback_data *caps_data)
{
	const char* status = caps_data->get_playbackStatus_value(caps_data);
	printf("[%s]status is : %s, index %d\n", __func__, status, caps_data->attr_playbackStatus_str2idx(status));
}

static void cap_mediaPlayback_play_cb(struct caps_mediaPlayback_data *caps_data)
{
	printf("[%s] > \n", __func__);
}

static void cap_mediaPlayback_pause_cb(struct caps_mediaPlayback_data *caps_data)
{
	printf("[%s] || \n", __func__);
}

static void cap_mediaPlayback_rewind_cb(struct caps_mediaPlayback_data *caps_data)
{
	printf("[%s] << \n", __func__);
}

static void cap_mediaPlayback_ff_cb(struct caps_mediaPlayback_data *caps_data)
{
	printf("[%s] >> \n", __func__);
}

static void cap_mediaPlayback_stop_cb(struct caps_mediaPlayback_data *caps_data)
{
	printf("[%s] == \n", __func__);
}

static void cap_ovenSetpoint_cmd_cb(struct caps_ovenSetpoint_data *caps_data)
{
	int point = caps_data->get_ovenSetpoint_value(caps_data);
	printf("[%s] point: %d\n", __func__, point);
}

static void cap_robotCleanerCleaningMode_cmd_cb(struct caps_robotCleanerCleaningMode_data *caps_data)
{
	const char* mode = caps_data->get_robotCleanerCleaningMode_value(caps_data);
	printf("[%s]cleaning mode: %s\n", __func__, mode);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)

/*pub_queue could send max 10 attr each time, so we need to divide  capabilities
to several group, and need to test each of them one by one.*/
static int test_group = 1; //could be 1,2,3

void send_failed_ui_test_capabilities()
{
	static int send_count = 0;
	send_count++;

	if (test_group == 1) {
		const char **supportacmodes = cap_airConditionerMode_data->get_supportedAcModes_value(cap_airConditionerMode_data);
		cap_airConditionerMode_data->set_airConditionerMode_value(cap_airConditionerMode_data, supportacmodes[send_count%(sizeof(supportedAcModes_supportedMode)/sizeof(char *))]);
		cap_airConditionerMode_data->attr_airConditionerMode_send(cap_airConditionerMode_data);

		//lock
		cap_lock_data->set_lock_value(cap_lock_data, lock_mode_test[send_count % LOCK_MAX]);
		cap_lock_data->attr_lock_send(cap_lock_data);

		//elevatorCall
		int call_idx = send_count % CAP_ENUM_ELEVATORCALL_CALLSTATUS_VALUE_MAX;
		cap_elevatorCall_data->set_callStatus_value(cap_elevatorCall_data,
						caps_helper_elevatorCall.attr_callStatus.values[call_idx]);
		cap_elevatorCall_data->attr_callStatus_send(cap_elevatorCall_data);
		//gasmeter
		char test_time[50];
		snprintf(test_time, 50, "2020-05-15 13:13:1%d", send_count);
		cap_gasMeter_data->set_gasMeterTime_value(cap_gasMeter_data, test_time);
		cap_gasMeter_data->attr_gasMeterTime_send(cap_gasMeter_data);

		double test_gasMeter_value = send_count;
		cap_gasMeter_data->set_gasMeter_value(cap_gasMeter_data, test_gasMeter_value);
		int g_idx = send_count % CAP_ENUM_GASMETER_GASMETER_UNIT_MAX;
		cap_gasMeter_data->set_gasMeter_unit(cap_gasMeter_data, caps_helper_gasMeter.
						attr_gasMeter.units[g_idx]);
		cap_gasMeter_data->attr_gasMeter_send(cap_gasMeter_data);

		double test_gasMeterCalorific_value = send_count;
		cap_gasMeter_data->set_gasMeterCalorific_value(cap_gasMeter_data, test_gasMeterCalorific_value);
		cap_gasMeter_data->attr_gasMeterCalorific_send(cap_gasMeter_data);

		double test_gasMeterVolume_value = send_count;
		cap_gasMeter_data->set_gasMeterVolume_value(cap_gasMeter_data, test_gasMeterVolume_value);
		int v_idx = send_count % CAP_ENUM_GASMETER_GASMETERVOLUME_UNIT_MAX;
		cap_gasMeter_data->set_gasMeterVolume_unit(cap_gasMeter_data, caps_helper_gasMeter.
						attr_gasMeterVolume.units[v_idx]);
		cap_gasMeter_data->attr_gasMeterVolume_send(cap_gasMeter_data);

		JSON_H *root = JSON_CREATE_OBJECT();
		JSON_ADD_STRING_TO_OBJECT(root, "description", "gasMeterPrecision");
		JSON_ADD_NUMBER_TO_OBJECT(root, "count", send_count);
		cap_gasMeter_data->set_gasMeterPrecision_value(cap_gasMeter_data, root);
		cap_gasMeter_data->attr_gasMeterPrecision_send(cap_gasMeter_data);
		JSON_FREE(root);

		double test_gasMeterConversion_value = send_count;
		cap_gasMeter_data->set_gasMeterConversion_value(cap_gasMeter_data, test_gasMeterConversion_value);
		cap_gasMeter_data->attr_gasMeterConversion_send(cap_gasMeter_data);
	}if (test_group == 2) {
		//imageCapture
		char image_value[50];
		snprintf(image_value, 50, "image_value%d", send_count);
		cap_imageCapture_data->set_image_value(cap_imageCapture_data, image_value);
		cap_imageCapture_data->attr_image_send(cap_imageCapture_data);
		char test_capture_time[50];
		snprintf(test_capture_time, 50, "2020-05-15 13:13:1%d", send_count);
		cap_imageCapture_data->set_captureTime_value(cap_imageCapture_data, test_capture_time);
		cap_imageCapture_data->attr_captureTime_send(cap_imageCapture_data);

		//videoCamera
		int m_idx = send_count % CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MAX;
		cap_videoCamera_data->set_mute_value(cap_videoCamera_data, caps_helper_videoCamera.attr_mute.
								values[m_idx]);
		cap_videoCamera_data->attr_mute_send(cap_videoCamera_data);

		int c_idx = send_count % CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_MAX;
		cap_videoCamera_data->set_camera_value(cap_videoCamera_data, caps_helper_videoCamera.attr_camera.
								values[c_idx]);
		cap_videoCamera_data->attr_camera_send(cap_videoCamera_data);

		int s_idx = send_count % 5;
		cap_videoCamera_data->set_statusMessage_value(cap_videoCamera_data, statusMessage[s_idx]);
		cap_videoCamera_data->attr_statusMessage_send(cap_videoCamera_data);

		JSON_H *c_value = JSON_CREATE_OBJECT();
		JSON_ADD_STRING_TO_OBJECT(c_value, "camera", "test for camera");
		JSON_ADD_NUMBER_TO_OBJECT(c_value, "count", send_count);
		cap_videoCamera_data->set_settings_value(cap_videoCamera_data, c_value);
		cap_videoCamera_data->attr_settings_send(cap_videoCamera_data);
		JSON_FREE(c_value);
		//videoCapture
		JSON_H *clip_value = JSON_CREATE_OBJECT();
		JSON_ADD_STRING_TO_OBJECT(clip_value, "clip", "test for clip");
		JSON_ADD_NUMBER_TO_OBJECT(clip_value, "count", send_count);
		cap_videoCapture_data->set_clip_value(cap_videoCapture_data, clip_value);
		cap_videoCapture_data->attr_clip_send(cap_videoCapture_data);
		JSON_FREE(clip_value);

		JSON_H *stream_value = JSON_CREATE_OBJECT();
		JSON_ADD_STRING_TO_OBJECT(stream_value, "stream", "test for stream");
		JSON_ADD_NUMBER_TO_OBJECT(stream_value, "count", send_count);
		cap_videoCapture_data->set_stream_value(cap_videoCapture_data, stream_value);
		cap_videoCapture_data->attr_stream_send(cap_videoCapture_data);
		JSON_FREE(stream_value);
		//videostream
		JSON_H *root = JSON_CREATE_OBJECT();
		JSON_ADD_STRING_TO_OBJECT(root, "description", "test for videostream");
		JSON_ADD_NUMBER_TO_OBJECT(root, "count", send_count);
		cap_videoStream_data->set_stream_value(cap_videoStream_data, root);
		cap_videoStream_data->attr_stream_send(cap_videoStream_data);
		JSON_FREE(root);
		//washerOperatingState
		int ms_idx = send_count % CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_MAX;
		cap_washerOperatingState_data->set_machineState_value(cap_washerOperatingState_data, caps_helper_washerOperatingState.
										attr_machineState.values[ms_idx]);
		cap_washerOperatingState_data->attr_machineState_send(cap_washerOperatingState_data);
	}else if (test_group == 3) {
		char completion_time[30];
		snprintf(completion_time, 30, "2020-05-13 09:02:0%d", send_count);
		cap_washerOperatingState_data->set_completionTime_value(cap_washerOperatingState_data, completion_time);
		cap_washerOperatingState_data->attr_completionTime_send(cap_washerOperatingState_data);

		int j_idx = send_count % CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_MAX;
		cap_washerOperatingState_data->set_washerJobState_value(cap_washerOperatingState_data, caps_helper_washerOperatingState.
											attr_washerJobState.values[j_idx]);
		cap_washerOperatingState_data->attr_washerJobState_send(cap_washerOperatingState_data);

		//mediaPlayback
		cap_mediaPlayback_data->set_playbackStatus_value(cap_mediaPlayback_data, caps_helper_mediaPlayback.attr_playbackStatus.values[send_count % 5]);	
		cap_mediaPlayback_data->attr_playbackStatus_send(cap_mediaPlayback_data);

		//ovenSetpoint
		cap_ovenSetpoint_data->set_ovenSetpoint_value(cap_ovenSetpoint_data, 50 + send_count);
		cap_ovenSetpoint_data->attr_ovenSetpoint_send(cap_ovenSetpoint_data);

		//pHMeasurement
		cap_pHMeasurement_data->set_pH_value(cap_pHMeasurement_data, (double)(7 + (send_count * 0.1)));
		cap_pHMeasurement_data->attr_pH_send(cap_pHMeasurement_data);

		//robotCleanerCleaningMode
		cap_robotCleanerCleaningMode_data->set_robotCleanerCleaningMode_value(cap_robotCleanerCleaningMode_data,
			(const char*)caps_helper_robotCleanerCleaningMode.attr_robotCleanerCleaningMode.values[send_count % CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_MAX]);
		cap_robotCleanerCleaningMode_data->attr_robotCleanerCleaningMode_send(cap_robotCleanerCleaningMode_data);
	}
}

void captest_failed_ui_initialize(IOT_CTX *ctx)
{
	if (test_group == 1) {
		// CAPTEST : call initialize function and set init_cb
		cap_airConditionerMode_data = caps_airConditionerMode_initialize(ctx, "main", cap_airConditionerMode_init_cb, NULL);
		cap_lock_data = caps_lock_initialize(ctx, "main", cap_lock_init_cb, NULL);
		cap_elevatorCall_data = caps_elevatorCall_initialize(ctx, "main", cap_elevatorCall_init_cb, NULL); 
		cap_gasMeter_data = caps_gasMeter_initialize(ctx, "main", cap_gasMeter_init_cb, NULL);
		
		// CAPTEST : set cmd usr cb
		cap_airConditionerMode_data->cmd_setAirConditionerMode_usr_cb = cap_airConditionerMode_cmd_cb;
		cap_lock_data->cmd_lock_usr_cb = cap_lock_cmd_cb;
		cap_lock_data->cmd_unlock_usr_cb = cap_unlock_cmd_cb;
		cap_elevatorCall_data->cmd_call_usr_cb = cap_elevatorCall_cmd_cb;
	} else if (test_group == 2) {
		// CAPTEST : call initialize function and set init_cb
		cap_imageCapture_data = caps_imageCapture_initialize(ctx, "main", cap_imageCapture_init_cb, NULL);
		cap_videoCamera_data = caps_videoCamera_initialize(ctx, "main", cap_videoCamera_init_cb, NULL);
		cap_videoCapture_data = caps_videoCapture_initialize(ctx, "main", cap_videoCapture_init_cb, NULL);
		cap_videoStream_data = caps_videoStream_initialize(ctx, "main", cap_videoStream_init_cb, NULL);
		cap_washerOperatingState_data = caps_washerOperatingState_initialize(ctx, "main", cap_washerOperatingState_init_cb, NULL);

		// CAPTEST : set cmd usr cb
		cap_imageCapture_data->cmd_take_usr_cb = cap_imageCapture_cmd_cb;
		cap_videoCamera_data->cmd_unmute_usr_cb = cap_unmute_cmd_cb;
		cap_videoCamera_data->cmd_on_usr_cb = cap_on_cmd_cb;
		cap_videoCamera_data->cmd_off_usr_cb = cap_off_cmd_cb;
		cap_videoCamera_data->cmd_flip_usr_cb = cap_flip_cmd_cb;
		cap_videoCamera_data->cmd_mute_usr_cb = cap_mute_cmd_cb;
		cap_videoCapture_data->cmd_capture_usr_cb = cap_capture_cmd_cb;
		cap_videoStream_data->cmd_stopStream_usr_cb = cap_videoStream_stop_cmd_cb;
		cap_videoStream_data->cmd_startStream_usr_cb = cap_videoStream_start_cmd_cb;
		cap_washerOperatingState_data->cmd_setMachineState_usr_cb = cap_washerOperatingState_cmd_cb;
		cap_windowShadePreset_data->cmd_presetPosition_usr_cb = cap_windowShadePreset_cmd_cb;
	} else if (test_group == 3) {
		// CAPTEST : call initialize function and set init_cb
		cap_windowShadePreset_data = caps_windowShadePreset_initialize(ctx, "main", cap_windowShadePreset_init_cb, NULL);
		cap_mediaPlayback_data = caps_mediaPlayback_initialize(ctx, "main", cap_mediaPlayback_init_cb, NULL);
		cap_ovenSetpoint_data = caps_ovenSetpoint_initialize(ctx, "main", cap_ovenSetpoint_init_cb, NULL);
		cap_pHMeasurement_data = caps_pHMeasurement_initialize(ctx, "main", cap_pHMeasurement_init_cb, NULL);
		cap_robotCleanerCleaningMode_data = caps_robotCleanerCleaningMode_initialize(ctx, "main", cap_robotCleanerCleaningMode_init_cb, NULL);

		// CAPTEST : set cmd usr cb
		cap_mediaPlayback_data->cmd_setPlaybackStatus_usr_cb = cap_mediaPlayback_setStatus_cb;
		cap_mediaPlayback_data->cmd_play_usr_cb = cap_mediaPlayback_play_cb;
		cap_mediaPlayback_data->cmd_pause_usr_cb = cap_mediaPlayback_pause_cb;
		cap_mediaPlayback_data->cmd_rewind_usr_cb = cap_mediaPlayback_rewind_cb;
		cap_mediaPlayback_data->cmd_fastForward_usr_cb = cap_mediaPlayback_ff_cb;
		cap_mediaPlayback_data->cmd_stop_usr_cb = cap_mediaPlayback_stop_cb;
		cap_ovenSetpoint_data->cmd_setOvenSetpoint_usr_cb = cap_ovenSetpoint_cmd_cb;
		cap_robotCleanerCleaningMode_data->cmd_setRobotCleanerCleaningMode_usr_cb = cap_robotCleanerCleaningMode_cmd_cb;
	}
}
