#include "st_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "caps_fanSpeed.h"
#include "caps_fanOscillationMode.h"
#include "caps_filterState.h"
#include "caps_filterStatus.h"
#include "caps_fineDustSensor.h"
#include "caps_formaldehydeMeasurement.h"
#include "caps_firmwareUpdate.h"

static caps_fanSpeed_data_t *caps_fanSpeed_data;
static caps_fanOscillationMode_data_t *caps_fanOscillationMode_data;

static caps_filterState_data_t *caps_filterState_data;
static caps_filterStatus_data_t *caps_filterStatus_data;

static caps_fineDustSensor_data_t *caps_fineDustSensor_data;
static caps_formaldehydeMeasurement_data_t *caps_formaldehydeMeasurement_data;

static caps_firmwareUpdate_data_t *caps_firmwareUpdate_data;

// CAPTEST : make init_cb for test capabilities
static void cap_fanSpeed_init_cb(struct caps_fanSpeed_data *caps_data)
{
	caps_data->set_fanSpeed_value(caps_data, caps_helper_fanSpeed.attr_fanSpeed.min);
}

static void caps_fanOscillationMode_init_cb(struct caps_fanOscillationMode_data *caps_data)
{
	caps_data->set_fanOscillationMode_value(caps_data, caps_helper_fanOscillationMode.attr_fanOscillationMode.values[CAP_ENUM_FANOSCILLATIONMODE_FANOSCILLATIONMODE_VALUE_HORIZONTAL]);
}

static void caps_filterState_init_cb(struct caps_filterState_data *caps_data)
{
	caps_data->set_filterLifeRemaining_value(caps_data, caps_helper_filterState.attr_filterLifeRemaining.min);
}

static void caps_filterStatus_init_cb(struct caps_filterStatus_data *caps_data)
{
	caps_data->set_filterStatus_value(caps_data, caps_helper_filterStatus.attr_filterStatus.values[CAP_ENUM_FILTERSTATUS_FILTERSTATUS_VALUE_NORMAL]);
}

static void caps_fineDustSensor_init_cb(struct caps_fineDustSensor_data *caps_data)
{
	caps_data->set_fineDustLevel_value(caps_data, caps_helper_fineDustSensor.attr_fineDustLevel.min);
	caps_data->set_fineDustLevel_unit(caps_data, caps_helper_fineDustSensor.attr_fineDustLevel.units[CAP_ENUM_FINEDUSTSENSOR_FINEDUSTLEVEL_UNIT_UG_PER_M3]);
}

static void caps_formaldehydeMeasurement_init_cb(struct caps_formaldehydeMeasurement_data *caps_data)
{
	caps_data->set_formaldehydeLevel_value(caps_data, caps_helper_formaldehydeMeasurement.attr_formaldehydeLevel.min);
	caps_data->set_formaldehydeLevel_unit(caps_data, caps_helper_formaldehydeMeasurement.attr_formaldehydeLevel.units[CAP_ENUM_FORMALDEHYDEMEASUREMENT_FORMALDEHYDELEVEL_UNIT_PPM]);
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
// CAPTEST : make cmd_cb for test capabilities
static void cap_fanSpeed_cmd_cb(struct caps_fanSpeed_data *caps_data)
{
	int value = caps_data->get_fanSpeed_value(caps_data);
	printf("%s: value:%d\n",__func__,value);
}

static void cap_setFanOscillationMode_cmd_cb(struct caps_fanOscillationMode_data *caps_data)
{
	const char *value = caps_data->get_fanOscillationMode_value(caps_data);
	printf("%s: value:%s\n",__func__,value);
}

static void cap_filterState_cmd_cb(struct caps_filterState_data *caps_data)
{
	int value = caps_data->get_filterLifeRemaining_value(caps_data);
	printf("%s: value:%d\n",__func__,value);
}

static void cap_filterStatus_cmd_cb(struct caps_filterStatus_data *caps_data)
{
	const char *value = caps_data->get_filterStatus_value(caps_data);
	printf("%s: value:%s\n",__func__,value);
}

static void cap_fineDustSensor_cmd_cb(struct caps_fineDustSensor_data *caps_data)
{
	const char *unit = caps_data->get_fineDustLevel_unit(caps_data);
	int level = caps_data->get_fineDustLevel_value(caps_data);
	printf("%s: level:%d\n",__func__,level);
	printf("%s: unit:%s\n",__func__,unit);
}

static void cap_formaldehydeMeasurement_cmd_cb(struct caps_formaldehydeMeasurement_data *caps_data)
{
	const char *unit = caps_data->get_formaldehydeLevel_unit(caps_data);
	int level = caps_data->get_formaldehydeLevel_value(caps_data);
	printf("%s: level:%d\n",__func__,level);
	printf("%s: unit:%s\n",__func__,unit);
}

static void cap_firmwareUpdate_init_cmd_cb(struct caps_firmwareUpdate_data *caps_data)
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

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_f_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	//fanSpeed
	int speed_value = send_count;
	caps_fanSpeed_data->set_fanSpeed_value(caps_fanSpeed_data, speed_value);
	caps_fanSpeed_data->attr_fanSpeed_send(caps_fanSpeed_data);
	//fanOs
	int fan_mode = send_count % CAP_ENUM_FANOSCILLATIONMODE_FANOSCILLATIONMODE_VALUE_MAX;
	caps_fanOscillationMode_data->set_fanOscillationMode_value(caps_fanOscillationMode_data,
								caps_helper_fanOscillationMode.attr_fanOscillationMode.values[fan_mode]);
	caps_fanOscillationMode_data->attr_fanOscillationMode_send(caps_fanOscillationMode_data);
	//filterState
	int LifeRemaining = 100 - send_count;
	caps_filterState_data->set_filterLifeRemaining_value(caps_filterState_data, LifeRemaining);
	caps_filterState_data->attr_filterLifeRemaining_send(caps_filterState_data);
	//filterStatus
	int status_idx = send_count % CAP_ENUM_FILTERSTATUS_FILTERSTATUS_VALUE_MAX;
	caps_filterStatus_data->set_filterStatus_value(caps_filterStatus_data,
							caps_helper_filterStatus.attr_filterStatus.values[status_idx]);
	caps_filterStatus_data->attr_filterStatus_send(caps_filterStatus_data);
	//fineDust
	int dust_level = send_count % 999;
	caps_fineDustSensor_data->set_fineDustLevel_value(caps_fineDustSensor_data, dust_level);
	caps_fineDustSensor_data->attr_fineDustLevel_send(caps_fineDustSensor_data);
	//formaldehyde
	int f_value = (send_count*50) % 1000000;
	caps_formaldehydeMeasurement_data->set_formaldehydeLevel_value(caps_formaldehydeMeasurement_data, f_value);
	caps_formaldehydeMeasurement_data->attr_formaldehydeLevel_send(caps_formaldehydeMeasurement_data);

	int unit_idx = send_count % CAP_ENUM_FORMALDEHYDEMEASUREMENT_FORMALDEHYDELEVEL_UNIT_MAX;
	caps_formaldehydeMeasurement_data->set_formaldehydeLevel_unit(caps_formaldehydeMeasurement_data,
										caps_helper_formaldehydeMeasurement.attr_formaldehydeLevel.units[unit_idx]);
	caps_formaldehydeMeasurement_data->attr_formaldehydeLevel_send(caps_formaldehydeMeasurement_data);
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
}

void captest_f_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_fanSpeed_data = caps_fanSpeed_initialize(ctx, "main", cap_fanSpeed_init_cb, NULL);
	caps_fanOscillationMode_data = caps_fanOscillationMode_initialize(ctx, "main", caps_fanOscillationMode_init_cb, NULL);

	caps_filterState_data = caps_filterState_initialize(ctx, "main", caps_filterState_init_cb, NULL);
	caps_filterStatus_data = caps_filterStatus_initialize(ctx, "main", caps_filterStatus_init_cb, NULL);

	caps_fineDustSensor_data = caps_fineDustSensor_initialize(ctx, "main", caps_fineDustSensor_init_cb, NULL);

	caps_formaldehydeMeasurement_data = caps_formaldehydeMeasurement_initialize(ctx, "main", caps_formaldehydeMeasurement_init_cb, NULL);

	caps_firmwareUpdate_data = caps_firmwareUpdate_initialize(ctx, "main", caps_firmwareUpdate_init_cb, NULL);
	// CAPTEST : set cmd usr cb
	caps_fanSpeed_data->cmd_setFanSpeed_usr_cb = cap_fanSpeed_cmd_cb;
	caps_fanOscillationMode_data->cmd_setFanOscillationMode_usr_cb = cap_setFanOscillationMode_cmd_cb;
}
