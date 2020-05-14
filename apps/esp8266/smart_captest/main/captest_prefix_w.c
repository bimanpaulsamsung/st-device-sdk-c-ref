#include "st_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "caps_washerMode.h"
#include "caps_waterSensor.h"
#include "caps_washerOperatingState.h"
#include "caps_windowShade.h"
#include "caps_windowShadePreset.h"

static caps_washerMode_data_t *caps_washerMode_data;
static caps_waterSensor_data_t *caps_waterSensor_data;
static caps_washerOperatingState_data_t *caps_washerOperatingState_data;
static caps_windowShade_data_t *caps_windowShade_data;
static caps_windowShadePreset_data_t *caps_windowShadePreset_data;

#define TEST_COMPLETION_TIME	"2020-05-13 09:50:10"

// CAPTEST : make init_cb for test capabilities
static void cap_windowShadePreset_init_cb(struct caps_windowShadePreset_data *caps_data)
{
	//todo
}

static void cap_washerMode_init_cb(struct caps_washerMode_data *caps_data)
{
	caps_data->set_washerMode_value(caps_data, caps_helper_washerMode.
							attr_washerMode.values[CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_REGULAR]);
}

static void cap_waterSensor_init_cb(struct caps_waterSensor_data *caps_data)
{
	caps_data->set_water_value(caps_data, caps_helper_waterSensor.
							attr_water.values[CAP_ENUM_WATERSENSOR_WATER_VALUE_DRY]);
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

const char *support_cmd[]={"close", "pause", "open"};
static void cap_windowShade_init_cb(struct caps_windowShade_data *caps_data)
{
	caps_data->set_windowShade_value(caps_data, caps_helper_windowShade.
					attr_windowShade.values[CAP_ENUM_WINDOWSHADE_WINDOWSHADE_VALUE_CLOSED]);

	caps_data->set_supportedWindowShadeCommands_value(caps_data, (const char **)support_cmd, sizeof(support_cmd)/sizeof(char *));
}
// CAPTEST : make cmd_cb for test capabilities
static void cap_windowShadePreset_cmd_cb(struct caps_windowShadePreset_data *caps_data)
{
	//todo
}

static void cap_washerMode_cmd_cb(struct caps_washerMode_data *caps_data)
{
	const char *value = caps_data->get_washerMode_value(caps_data);
	printf("%s: washer mode value:%s\n",__func__,value);
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

static void cap_windowShade_cmd_cb(struct caps_windowShade_data *caps_data)
{
	const char *value = caps_data->get_windowShade_value(caps_data);

	printf("%s: windowShade valude :%s\n",__func__, value);
}
// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_w_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	//washerMode
	int wsm_idx = send_count % CAP_ENUM_WASHERMODE_WASHERMODE_VALUE_MAX;
	caps_washerMode_data->set_washerMode_value(caps_washerMode_data,
					caps_helper_washerMode.attr_washerMode.values[wsm_idx]);
	caps_washerMode_data->attr_washerMode_send(caps_washerMode_data);
	//waterSensor
	int ws_idx = send_count % CAP_ENUM_WATERSENSOR_WATER_VALUE_MAX;
	caps_waterSensor_data->set_water_value(caps_waterSensor_data,
					caps_helper_waterSensor.attr_water.values[ws_idx]);
	caps_waterSensor_data->attr_water_send(caps_waterSensor_data);
	//washerOperatingState
	int ms_idx = send_count % CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_MAX;
	caps_washerOperatingState_data->set_machineState_value(caps_washerOperatingState_data, caps_helper_washerOperatingState.
									attr_machineState.values[ms_idx]);
	caps_washerOperatingState_data->attr_machineState_send(caps_washerOperatingState_data);

	char completion_time[30];
	snprintf(completion_time, 30, "2020-05-13 09:02:0%d", send_count);
	caps_washerOperatingState_data->set_completionTime_value(caps_washerOperatingState_data, completion_time);
	caps_washerOperatingState_data->attr_completionTime_send(caps_washerOperatingState_data);

	int j_idx = send_count % CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_MAX;
	caps_washerOperatingState_data->set_washerJobState_value(caps_washerOperatingState_data, caps_helper_washerOperatingState.
										attr_washerJobState.values[j_idx]);
	caps_washerOperatingState_data->attr_washerJobState_send(caps_washerOperatingState_data);
	//windowShade
	int w_idx = send_count % CAP_ENUM_WINDOWSHADE_WINDOWSHADE_VALUE_MAX;
	caps_windowShade_data->set_windowShade_value(caps_windowShade_data, caps_helper_windowShade.
									attr_windowShade.values[w_idx]);
	caps_windowShade_data->attr_windowShade_send(caps_windowShade_data);
}

void captest_w_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_washerMode_data = caps_washerMode_initialize(ctx, "main", cap_washerMode_init_cb, NULL);
	caps_waterSensor_data = caps_waterSensor_initialize(ctx, "main", cap_waterSensor_init_cb, NULL);
	caps_washerOperatingState_data = caps_washerOperatingState_initialize(ctx, "main", cap_washerOperatingState_init_cb, NULL);
	caps_windowShade_data = caps_windowShade_initialize(ctx, "main", cap_windowShade_init_cb, NULL);
	caps_windowShadePreset_data = caps_windowShadePreset_initialize(ctx, "main", cap_windowShadePreset_init_cb, NULL);
	// CAPTEST : set cmd usr cb
	caps_washerMode_data->cmd_setWasherMode_usr_cb = cap_washerMode_cmd_cb;
	caps_washerOperatingState_data->cmd_setMachineState_usr_cb = cap_washerOperatingState_cmd_cb;
	caps_windowShade_data->cmd_close_usr_cb = cap_windowShade_cmd_cb;
	caps_windowShade_data->cmd_pause_usr_cb = cap_windowShade_cmd_cb;
	caps_windowShade_data->cmd_open_usr_cb = cap_windowShade_cmd_cb;
	caps_windowShadePreset_data->cmd_presetPosition_usr_cb = cap_windowShadePreset_cmd_cb;
}
