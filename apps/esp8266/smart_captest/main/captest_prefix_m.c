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

#include "caps_mediaInputSource.h"
#include "caps_mediaPlayback.h"
#include "caps_mediaPlaybackRepeat.h"
#include "caps_mediaPlaybackShuffle.h"
#include "caps_mediaPresets.h"
#include "JSON.h"
#include "caps_mediaTrackControl.h"
#include "caps_momentary.h"
#include "caps_motionSensor.h"


static caps_mediaInputSource_data_t *cap_mediaInputSource_data;
static caps_mediaPlayback_data_t *cap_mediaPlayback_data;
static caps_mediaPlaybackRepeat_data_t *cap_mediaPlaybackRepeat_data;
static caps_mediaPlaybackShuffle_data_t *cap_mediaPlaybackShuffle_data;
static caps_mediaPresets_data_t *cap_mediaPresets_data;
static caps_mediaTrackControl_data_t *cap_mediaTrackControl_data;
static caps_momentary_data_t *cap_momentary_data;
static caps_motionSensor_data_t *cap_motionSensor_data;


#define MEDIA_INPUTSRC_STR_TEST		"HDMI"
static const char* mediaInputSource_array[] = {"AM", "CD", "FM", "HDMI", "HDMI1", "HDMI2", "HDMI3", "HDMI4", "HDMI5", "HDMI6", "digitalTv", "USB", "YouTube", "aux", "bluetooth", "digital", "melon", "wifi"};

#define MEDIA_PLAYBACK_TEST		"stopped"
static const char* mediaPlayback_array[] = {"paused", "playing", "stopped", "fast_forwarding", "rewinding"};

#define MEDIA_PLAYBACK_REPEAT_TEST		"off"
static const char* mediaPlayback_repeat_array[] = {"all", "off", "one"};

#define MEDIA_PLAYBACK_SHUFFLE_TEST		"disabled"
static const char* mediaPlayback_shuffle_array[] = {"disabled", "enabled"};

//No clue in helper about the command supported by mediaTrackControl
static const char* mediaTrackControl_array[] = {"test1", "test2"};

#define MOTION_SENSOR_TEST		"active"
static const char* motionSensor_array[] = {"active", "inactive"};

// CAPTEST : make init_cb for test capabilities
static void cap_mediaInputSource_init_cb(struct caps_mediaInputSource_data *caps_data)
{
	caps_data->set_inputSource_value(caps_data, MEDIA_INPUTSRC_STR_TEST);
	caps_data->set_supportedInputSources_value(caps_data, mediaInputSource_array, sizeof(mediaInputSource_array) / sizeof(char*));
}

static void cap_mediaPlayback_init_cb(struct caps_mediaPlayback_data *caps_data)
{
	caps_data->set_playbackStatus_value(caps_data, MEDIA_PLAYBACK_TEST);
	caps_data->set_supportedPlaybackCommands_value(caps_data, mediaPlayback_array, sizeof(mediaPlayback_array) / sizeof(char*));
}

static void cap_mediaPlaybackRepeat_init_cb(struct caps_mediaPlaybackRepeat_data *caps_data)
{
	caps_data->set_playbackRepeatMode_value(caps_data, MEDIA_PLAYBACK_REPEAT_TEST);
}

static void cap_mediaPlaybackShuffle_init_cb(struct caps_mediaPlaybackShuffle_data *caps_data)
{
	caps_data->set_playbackShuffle_value(caps_data, MEDIA_PLAYBACK_SHUFFLE_TEST);
}

static void cap_mediaPresets_init_cb(struct caps_mediaPresets_data *caps_data)
{
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "test for mediaPresets");
	JSON_ADD_NUMBER_TO_OBJECT(root, "date", 2020);

	caps_data->set_presets_value(caps_data, root);
	JSON_FREE(root);
}

static void cap_mediaTrackControl_init_cb(struct caps_mediaTrackControl_data *caps_data)
{
	caps_data->set_supportedTrackControlCommands_value(caps_data, mediaTrackControl_array, sizeof(mediaTrackControl_array) / sizeof(char*));
}

static void cap_momentary_init_cb(struct caps_momentary_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_motionSensor_init_cb(struct caps_motionSensor_data *caps_data)
{
	caps_data->set_motion_value(caps_data, MOTION_SENSOR_TEST);
}



// CAPTEST : make cmd_cb for test capabilities
static void cap_mediaInputSource_cmd_cb(struct caps_mediaInputSource_data *caps_data)
{
	const char* source = caps_data->get_inputSource_value(caps_data);
	printf("[%s]source is : %s, index %d\n", __func__, source, caps_data->attr_inputSource_str2idx(source));
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

static void cap_mediaPlaybackRepeat_cmd_cb(struct caps_mediaPlaybackRepeat_data *caps_data)
{
	const char* repeat_mode = caps_data->get_playbackRepeatMode_value(caps_data);
	printf("[%s] repeat_mode %s\n", __func__, repeat_mode);
}

static void cap_mediaPlaybackShuffle_cmd_cb(struct caps_mediaPlaybackShuffle_data *caps_data)
{
	const char* shuffle = caps_data->get_playbackShuffle_value(caps_data);
	printf("[%s] shuffle %s, index %d\n", __func__, shuffle, caps_data->attr_playbackShuffle_str2idx(shuffle));
}

static void cap_mediaPresets_cmd_cb(struct caps_mediaPresets_data *caps_data)
{
	JSON_H* root = caps_data->get_presets_value(caps_data);
	char* json_str = JSON_PRINT(root);
	printf("[%s] json string %s\n", __func__, json_str);
	JSON_FREE(json_str);
	JSON_FREE(root);
}

static void cap_mediaTrackControl_nextTrack_cb(struct caps_mediaTrackControl_data *caps_data)
{
	printf("[%s] next track ->\n", __func__);
}

static void cap_mediaTrackControl_prevTrack_cb(struct caps_mediaTrackControl_data *caps_data)
{
	printf("[%s] previous track ->\n", __func__);
}

static void cap_momentary_cmd_cb(struct caps_momentary_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_m_test_capabilities() 
{
	static int send_count = 0;
	send_count++;

	//mediaInputSource
	cap_mediaInputSource_data->set_inputSource_value(cap_mediaInputSource_data, mediaInputSource_array[send_count % 18]);
	cap_mediaInputSource_data->attr_inputSource_send(cap_mediaInputSource_data);

	//mediaPlayback
	cap_mediaPlayback_data->set_playbackStatus_value(cap_mediaPlayback_data, mediaPlayback_array[send_count % 5]);	
	cap_mediaPlayback_data->attr_playbackStatus_send(cap_mediaPlayback_data);

	//mediaPlaybackRepeat
	cap_mediaPlaybackRepeat_data->set_playbackRepeatMode_value(cap_mediaPlaybackRepeat_data, mediaPlayback_repeat_array[send_count % 3]);
	cap_mediaPlaybackRepeat_data->attr_playbackRepeatMode_send(cap_mediaPlaybackRepeat_data);

	//mediaPlaybackShuffle
	cap_mediaPlaybackShuffle_data->set_playbackShuffle_value(cap_mediaPlaybackShuffle_data, mediaPlayback_shuffle_array[send_count % 2]);
	cap_mediaPlaybackShuffle_data->attr_playbackShuffle_send(cap_mediaPlaybackShuffle_data);

	//mediaPresets - No capability on DevWorks
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "test for mediaPresets");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", send_count);
	cap_mediaPresets_data->set_presets_value(cap_mediaPresets_data, root);
	//cap_mediaPresets_data->attr_presets_send(cap_mediaPresets_data);
	JSON_FREE(root);

	//mediaTrackControl - No capability on DevWorks
	char* control_cmd[] = {"Next", "Previous"};
	const char** cmd_arr = (send_count % 2)? control_cmd : mediaTrackControl_array;
	cap_mediaTrackControl_data->set_supportedTrackControlCommands_value(cap_mediaTrackControl_data, cmd_arr, 2);
	//cap_mediaTrackControl_data->attr_supportedTrackControlCommands_send(cap_mediaTrackControl_data);

	//No data send for momentary

	//motionSensor
	cap_motionSensor_data->set_motion_value(cap_motionSensor_data, motionSensor_array[send_count % 2]);
	cap_motionSensor_data->attr_motion_send(cap_motionSensor_data);
}

void captest_m_initialize(IOT_CTX *ctx)
{
// CAPTEST : call initialize function and set init_cb
	cap_mediaInputSource_data = caps_mediaInputSource_initialize(ctx, "main", cap_mediaInputSource_init_cb, NULL);

	cap_mediaPlayback_data = caps_mediaPlayback_initialize(ctx, "main", cap_mediaPlayback_init_cb, NULL);

	cap_mediaPlaybackRepeat_data = caps_mediaPlaybackRepeat_initialize(ctx, "main", cap_mediaPlaybackRepeat_init_cb, NULL);

	cap_mediaPlaybackShuffle_data = caps_mediaPlaybackShuffle_initialize(ctx, "main", cap_mediaPlaybackShuffle_init_cb, NULL);

	cap_mediaPresets_data = caps_mediaPresets_initialize(ctx, "main", cap_mediaPresets_init_cb, NULL);

	cap_mediaTrackControl_data = caps_mediaTrackControl_initialize(ctx, "main", cap_mediaTrackControl_init_cb, NULL);

	cap_momentary_data = caps_momentary_initialize(ctx, "main", cap_momentary_init_cb, NULL);

	cap_motionSensor_data = caps_motionSensor_initialize(ctx, "main", cap_motionSensor_init_cb, NULL);

// CAPTEST : set cmd usr cb
	cap_mediaInputSource_data->cmd_setInputSource_usr_cb = cap_mediaInputSource_cmd_cb;
	
	cap_mediaPlayback_data->cmd_setPlaybackStatus_usr_cb = cap_mediaPlayback_setStatus_cb;
	cap_mediaPlayback_data->cmd_play_usr_cb = cap_mediaPlayback_play_cb;
	cap_mediaPlayback_data->cmd_pause_usr_cb = cap_mediaPlayback_pause_cb;
	cap_mediaPlayback_data->cmd_rewind_usr_cb = cap_mediaPlayback_rewind_cb;
	cap_mediaPlayback_data->cmd_fastForward_usr_cb = cap_mediaPlayback_ff_cb;
	cap_mediaPlayback_data->cmd_stop_usr_cb = cap_mediaPlayback_stop_cb;

	cap_mediaPlaybackRepeat_data->cmd_setPlaybackRepeatMode_usr_cb = cap_mediaPlaybackRepeat_cmd_cb;

	cap_mediaPlaybackShuffle_data->cmd_setPlaybackShuffle_usr_cb = cap_mediaPlaybackShuffle_cmd_cb;

	cap_mediaPresets_data->cmd_playPreset_usr_cb = cap_mediaPresets_cmd_cb;

	cap_mediaTrackControl_data->cmd_nextTrack_usr_cb = cap_mediaTrackControl_nextTrack_cb;
	cap_mediaTrackControl_data->cmd_previousTrack_usr_cb = cap_mediaTrackControl_prevTrack_cb;

	cap_momentary_data->cmd_push_usr_cb = cap_momentary_cmd_cb;

	//No cmd usr cb for motionSensor

}
