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
#include "caps_dishwasherOperatingState.h"
#include "caps_dryerOperatingState.h"
#include "caps_firmwareUpdate.h"
#include "caps_objectDetection.h"
#include "caps_ovenOperatingState.h"
#include "caps_presenceSensor.h"
#include "caps_sleepSensor.h"
#include "caps_soundSensor.h"
//this command failed case:
#include "caps_tvChannel.h"


// CAPTEST :define test capablity variable.
static caps_dishwasherOperatingState_data_t *caps_dishwasherOperatingState_data;
static caps_dryerOperatingState_data_t *caps_dryerOperatingState_data;
static caps_firmwareUpdate_data_t *caps_firmwareUpdate_data;
static caps_objectDetection_data_t *cap_objectDetection_data;
static caps_ovenOperatingState_data_t *cap_ovenOperatingState_data;
static caps_presenceSensor_data_t *cap_presenceSensor_data;
static caps_sleepSensor_data_t *cap_sleepSensor_data;
static caps_soundSensor_data_t *cap_soundSensor_data;

static caps_tvChannel_data_t *caps_tvChannel_data;//command failed case.


const char *support_Ms[]={"testST1", "testST2", "testST3"};
static const char* objectDetection_support_array[] = {"active", "inactive"};
//tvChannel
#define MAX_CH  4
static char *tVCh[] = {"Ch1", "Ch2", "Ch3", "Ch4"};
static char *tVChName[] = {"tVChName1", "tVChName2", "tVChName3", "tVChName4"};

// CAPTEST : make init_cb for test capabilities
static void cap_dishwasherOperatingState_init_cb(struct caps_dishwasherOperatingState_data *caps_data)
{
		caps_data->set_completionTime_value(caps_data, "2020-05-09 13:13:02");
		caps_data->set_supportedMachineStates_value(caps_data, (const char **)support_Ms,
					sizeof(support_Ms)/sizeof(char *));
		caps_data->set_dishwasherJobState_value(caps_data, caps_helper_dishwasherOperatingState.
					attr_dishwasherJobState.values[CAP_ENUM_DISHWASHEROPERATINGSTATE_DISHWASHERJOBSTATE_VALUE_AIRWASH]);
}

static void cap_dryerOperatingState_init_cb(struct caps_dryerOperatingState_data *caps_data)
{
		caps_data->set_completionTime_value(caps_data, "2020-05-09 13:13:02");
		caps_data->set_supportedMachineStates_value(caps_data, (const char **)support_Ms,
					sizeof(support_Ms)/sizeof(char *));
		caps_data->set_dryerJobState_value(caps_data, caps_helper_dryerOperatingState.
					attr_dryerJobState.values[CAP_ENUM_DRYEROPERATINGSTATE_DRYERJOBSTATE_VALUE_MAX]);
} 
static void caps_firmwareUpdate_init_cb(struct caps_firmwareUpdate_data *caps_data)
{
		caps_data->set_lastUpdateStatus_value(caps_data, caps_helper_firmwareUpdate.attr_lastUpdateStatus.values[CAP_ENUM_FIRMWAREUPDATE_LASTUPDATESTATUS_VALUE_UPDATESUCCEEDED]);
		caps_data->set_state_value(caps_data, caps_helper_firmwareUpdate.attr_state.values[CAP_ENUM_FIRMWAREUPDATE_STATE_VALUE_NORMALOPERATION]);
		caps_data->set_currentVersion_value(caps_data, "version:1.0");

		caps_data->set_lastUpdateTime_value(caps_data, "2020-05-07");
		caps_data->set_availableVersion_value(caps_data, "version:2.0");
		caps_data->set_lastUpdateStatusReason_value(caps_data, "update test");
}
static void cap_objectDetection_init_cb(struct caps_objectDetection_data *caps_data)
{
		JSON_H *root = JSON_CREATE_OBJECT();
		JSON_ADD_NUMBER_TO_OBJECT(root, "active", 1);
		JSON_ADD_NUMBER_TO_OBJECT(root, "inactive", 0);

		caps_data->set_detected_value(caps_data, root);
		caps_data->set_supportedValues_value(caps_data, objectDetection_support_array, sizeof(objectDetection_support_array) / sizeof(char*));
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
static void cap_presenceSensor_init_cb(struct caps_presenceSensor_data *caps_data)
{
		caps_data->set_presence_value(caps_data, (const char*)caps_helper_presenceSensor.attr_presence.values[CAP_ENUM_PRESENCESENSOR_PRESENCE_VALUE_PRESENT]);
}
static void cap_sleepSensor_init_cb(struct caps_sleepSensor_data *caps_data)
{
		caps_data->set_sleeping_value(caps_data, caps_helper_sleepSensor.attr_sleeping.values[CAP_ENUM_SLEEPSENSOR_SLEEPING_VALUE_NOT_SLEEPING]);
}
static void cap_soundSensor_init_cb(struct caps_soundSensor_data *caps_data)
{
		caps_data->set_sound_value(caps_data, caps_helper_soundSensor.attr_sound.values[CAP_ENUM_SOUNDSENSOR_SOUND_VALUE_DETECTED]);
}

//cmd failed case
static void cap_tvChannel_init_cb(struct caps_tvChannel_data *caps_data)
{
		caps_data->set_tvChannel_value(caps_data, tVCh[0]);
		caps_data->set_tvChannelName_value(caps_data, tVChName[0]);
}

// CAPTEST : make cmd_cb for test capabilities
static void cap_dishwasherOperatingState_cmd_cb(struct caps_dishwasherOperatingState_data *caps_data)
{
		const char *completionTime_value = caps_data->get_completionTime_value(caps_data);
		const char *m_state = caps_data->get_machineState_value(caps_data);
		const char *job_state = caps_data->get_dishwasherJobState_value(caps_data);

		printf("%s: completionTime :%s\n",__func__, completionTime_value);
		printf("%s: machineState :%s\n",__func__, m_state);
		printf("%s: jobstate :%s\n",__func__, job_state);
}

static void cap_dryerOperatingState_cmd_cb(struct caps_dryerOperatingState_data *caps_data)
{
		const char *completionTime_value = caps_data->get_completionTime_value(caps_data);
		const char *m_state = caps_data->get_machineState_value(caps_data);
		const char *job_state = caps_data->get_dryerJobState_value(caps_data);

		printf("%s: completionTime :%s\n",__func__, completionTime_value);
		printf("%s: machineState :%s\n",__func__, m_state);
		printf("%s: jobstate :%s\n",__func__, job_state);
}
static void cap_firmwareUpdate_usr_cmd_cb(struct caps_firmwareUpdate_data *caps_data)
{
		const char *LUS = caps_data->get_lastUpdateStatus_value(caps_data);
		const char *state = caps_data->get_state_value(caps_data);
		const char *CV = caps_data->get_currentVersion_value(caps_data);
		const char *LUT = caps_data->get_lastUpdateTime_value(caps_data);
		const char *AV = caps_data->get_availableVersion_value(caps_data);
		const char *LUSR = caps_data->get_lastUpdateStatusReason_value(caps_data);

		printf("%s: lastUpdateStatus:%s\n",__func__,LUS);
		printf("%s: state:%s\n",__func__,state);
		printf("%s: currentVersion:%s\n",__func__,CV);
		printf("%s: lastUpdateTime:%s\n",__func__,LUT);
		printf("%s: availableVersion:%s\n",__func__,AV);
		printf("%s: lastUpdateStatusReason:%s\n",__func__,LUSR);
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

/*pub_queue could send max 10 attr each time, so we need to divide  capabilities
to several group, and need to test each of them one by one.*/
static int test_group = 1; //could be 1,2,3

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_failed_event_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	if (test_group == 1) {
		//dishwasherOperatingState
		const char completion_time[3][30]={"2020-05-09 13:13:02", "2020-05-10 14:14:02","2020-05-11 15:15:02"};
		int testtime_idx = send_count % 3;
		caps_dishwasherOperatingState_data->set_completionTime_value(caps_dishwasherOperatingState_data, completion_time[testtime_idx]);
		caps_dishwasherOperatingState_data->attr_completionTime_send(caps_dishwasherOperatingState_data);

		int j_idx = send_count % CAP_ENUM_DISHWASHEROPERATINGSTATE_DISHWASHERJOBSTATE_VALUE_MAX;
		caps_dishwasherOperatingState_data->set_dishwasherJobState_value(caps_dishwasherOperatingState_data, caps_helper_dishwasherOperatingState.
												attr_dishwasherJobState.values[j_idx]);
		caps_dishwasherOperatingState_data->attr_dishwasherJobState_send(caps_dishwasherOperatingState_data);
		//firmwareupdate
		int lus_idx = send_count % CAP_ENUM_FIRMWAREUPDATE_LASTUPDATESTATUS_VALUE_MAX;
		caps_firmwareUpdate_data->set_lastUpdateStatus_value(caps_firmwareUpdate_data,
																				 caps_helper_firmwareUpdate.attr_lastUpdateStatus.values[lus_idx]);
		caps_firmwareUpdate_data->attr_lastUpdateStatus_send(caps_firmwareUpdate_data);
		
		int s_idx = send_count % CAP_ENUM_FIRMWAREUPDATE_STATE_VALUE_MAX;
		caps_firmwareUpdate_data->set_state_value(caps_firmwareUpdate_data, caps_helper_firmwareUpdate.attr_state.values[s_idx]);
		caps_firmwareUpdate_data->attr_state_send(caps_firmwareUpdate_data);
		
		char ver[3][15] = {"version:1.0","version:2.0","version:3.0"};
		int v_idx = send_count % 3;
		caps_firmwareUpdate_data->set_currentVersion_value(caps_firmwareUpdate_data, ver[v_idx]);
		caps_firmwareUpdate_data->attr_currentVersion_send(caps_firmwareUpdate_data);
		
		char lastuptime[3][15] = {"2020-05-04","2020-04-04","2020-04-06"};
		int l_idx = send_count % 3;
		caps_firmwareUpdate_data->set_lastUpdateTime_value(caps_firmwareUpdate_data, lastuptime[l_idx]);
		caps_firmwareUpdate_data->attr_lastUpdateTime_send(caps_firmwareUpdate_data);
		
		char avai_version[3][15] = {"version:4.0","version:5.0","version:6.0"};
		int ava_idx = send_count % 3;
		caps_firmwareUpdate_data->set_availableVersion_value(caps_firmwareUpdate_data, avai_version[ava_idx]);
		caps_firmwareUpdate_data->attr_availableVersion_send(caps_firmwareUpdate_data);
		
		char reason[3][15] = {"reason:1","reason:2","reason:3"};
		int reason_idx = send_count % 3;
		caps_firmwareUpdate_data->set_lastUpdateStatusReason_value(caps_firmwareUpdate_data, reason[reason_idx]);
		caps_firmwareUpdate_data->attr_lastUpdateStatusReason_send(caps_firmwareUpdate_data);

	} else if (test_group == 2) {
		//objectDetection
		JSON_H *root = JSON_CREATE_OBJECT();
		JSON_ADD_NUMBER_TO_OBJECT(root, "active", 10 + send_count);
		JSON_ADD_NUMBER_TO_OBJECT(root, "inactive", 20 + send_count);
		cap_objectDetection_data->set_detected_value(cap_objectDetection_data, root);
		cap_objectDetection_data->attr_detected_send(cap_objectDetection_data);
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

		cap_presenceSensor_data->set_presence_value(cap_presenceSensor_data, (const char*)caps_helper_presenceSensor.attr_presence.values[send_count % CAP_ENUM_PRESENCESENSOR_PRESENCE_VALUE_MAX]);
		cap_presenceSensor_data->attr_presence_send(cap_presenceSensor_data);
	} else if (test_group == 3) {
		cap_sleepSensor_data->set_sleeping_value(cap_sleepSensor_data,
								(const char*)caps_helper_sleepSensor.attr_sleeping.values[send_count % CAP_ENUM_SLEEPSENSOR_SLEEPING_VALUE_MAX]);
		cap_sleepSensor_data->attr_sleeping_send(cap_sleepSensor_data);
		cap_soundSensor_data->set_sound_value(cap_soundSensor_data,
						(const char*)caps_helper_soundSensor.attr_sound.values[send_count % CAP_ENUM_SOUNDSENSOR_SOUND_VALUE_MAX]);
		cap_soundSensor_data->attr_sound_send(cap_soundSensor_data);
		//tvChannel
		int tvch_idx = send_count % MAX_CH;
		caps_tvChannel_data->set_tvChannel_value(caps_tvChannel_data, tVCh[tvch_idx]);
		caps_tvChannel_data->attr_tvChannel_send(caps_tvChannel_data);

		caps_tvChannel_data->set_tvChannelName_value(caps_tvChannel_data, tVChName[tvch_idx]);
		caps_tvChannel_data->attr_tvChannelName_send(caps_tvChannel_data);
	}

}

void captest_failed_event_initialize(IOT_CTX *ctx)
{
	if (test_group == 1) {
		// CAPTEST : call initialize function and set init_cb
		caps_dishwasherOperatingState_data = caps_dishwasherOperatingState_initialize(ctx, "main", cap_dishwasherOperatingState_init_cb, NULL);
		caps_dryerOperatingState_data = caps_dryerOperatingState_initialize(ctx, "main", cap_dryerOperatingState_init_cb, NULL);
		caps_firmwareUpdate_data = caps_firmwareUpdate_initialize(ctx, "main", caps_firmwareUpdate_init_cb, NULL);
		// CAPTEST : set cmd usr cb
		caps_dishwasherOperatingState_data->cmd_setMachineState_usr_cb = cap_dishwasherOperatingState_cmd_cb;
		caps_dryerOperatingState_data->cmd_setMachineState_usr_cb = cap_dryerOperatingState_cmd_cb;
		caps_firmwareUpdate_data->cmd_checkForFirmwareUpdate_usr_cb = cap_firmwareUpdate_usr_cmd_cb;
	} else if (test_group == 2) {
		// CAPTEST : call initialize function and set init_cb
		cap_objectDetection_data = caps_objectDetection_initialize(ctx, "main", cap_objectDetection_init_cb, NULL);
		cap_ovenOperatingState_data = caps_ovenOperatingState_initialize(ctx, "main", cap_ovenOperatingState_init_cb, NULL);
		cap_presenceSensor_data = caps_presenceSensor_initialize(ctx, "main", cap_presenceSensor_init_cb, NULL);
		// CAPTEST : set cmd usr cb
		cap_ovenOperatingState_data->cmd_start_usr_cb = cap_ovenOperatingState_cmd_cb;
		cap_ovenOperatingState_data->cmd_stop_usr_cb = cap_ovenOperatingState_start_cb;
		cap_ovenOperatingState_data->cmd_setMachineState_usr_cb = cap_ovenOperatingState_stop_cb;
		
	} else if (test_group == 3) {
		// CAPTEST : call initialize function and set init_cb
		cap_sleepSensor_data = caps_sleepSensor_initialize(ctx, "main", cap_sleepSensor_init_cb, NULL);
		cap_soundSensor_data = caps_soundSensor_initialize(ctx, "main", cap_soundSensor_init_cb, NULL);
		caps_tvChannel_data = caps_tvChannel_initialize(ctx, "main", cap_tvChannel_init_cb, NULL);
		// CAPTEST : set cmd usr cb
		caps_tvChannel_data->cmd_channelUp_usr_cb = cap_channelUp_cmd_cb;
		caps_tvChannel_data->cmd_channelDown_usr_cb = cap_channelDown_cmd_cb;
		caps_tvChannel_data->cmd_setTvChannelName_usr_cb = cap_setTvChannelName_cmd_cb;
		caps_tvChannel_data->cmd_setTvChannel_usr_cb = cap_setTvChannel_cmd_cb;
	}
}
