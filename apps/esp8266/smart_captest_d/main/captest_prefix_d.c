#include "st_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "caps_dishwasherOperatingState.h"
#include "caps_doorControl.h"
#include "caps_dryerMode.h"
#include "caps_dustSensor.h"
#include "caps_dryerOperatingState.h"

static caps_dishwasherOperatingState_data_t *caps_dishwasherOperatingState_data;
static caps_dryerOperatingState_data_t *caps_dryerOperatingState_data;
static caps_doorControl_data_t *caps_doorControl_data;
static caps_dryerMode_data_t *caps_dryerMode_data;
static caps_dustSensor_data_t *caps_dustSensor_data;

const char *support_Ms[]={"testST1", "testST2", "testST3"};
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

static void cap_doorControl_init_cb(struct caps_doorControl_data *caps_data)
{
    caps_data->set_door_value(caps_data, caps_helper_doorControl.attr_door.values[CAP_ENUM_DOORCONTROL_DOOR_VALUE_CLOSED]);
}

static void cap_dryerMode_init_cb(struct caps_dryerMode_data *caps_data)
{
    caps_data->set_dryerMode_value(caps_data, caps_helper_dryerMode.attr_dryerMode.values[CAP_ENUM_DRYERMODE_DRYERMODE_VALUE_REGULAR]);
}

static void cap_dustSensor_init_cb(struct caps_dustSensor_data *caps_data)
{
    caps_data->set_fineDustLevel_value(caps_data, caps_helper_dustSensor.attr_fineDustLevel.min);
    caps_data->set_fineDustLevel_unit(caps_data, caps_helper_dustSensor.attr_fineDustLevel.
										units[CAP_ENUM_DUSTSENSOR_FINEDUSTLEVEL_UNIT_UG_PER_M3]);

    caps_data->set_dustLevel_value(caps_data, caps_helper_dustSensor.attr_dustLevel.min);
    caps_data->set_dustLevel_unit(caps_data, caps_helper_dustSensor.attr_dustLevel.
										units[CAP_ENUM_DUSTSENSOR_DUSTLEVEL_UNIT_UG_PER_M3]);
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

static void cap_doorControl_cmd_cb(struct caps_doorControl_data *caps_data)
{
    const char *door_value = caps_data->get_door_value(caps_data);

	printf("%s: door value :%s\n",__func__, door_value);
}

static void cap_dryerMode_cmd_cb(struct caps_dryerMode_data *caps_data)
{
    const char *dryerMode_value = caps_data->get_dryerMode_value(caps_data);

	printf("%s: dryerMode value :%s\n",__func__, dryerMode_value);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	//dishwasherOperatingState
	const char completion_time[3][30]={"2020-05-09 13:13:02", "2020-05-10 14:14:02","2020-05-11 15:15:02"};
	int testtime_idx = send_count % 3;
	caps_dishwasherOperatingState_data->set_completionTime_value(caps_dishwasherOperatingState_data, completion_time[testtime_idx]);
	caps_dishwasherOperatingState_data->attr_completionTime_send(caps_dishwasherOperatingState_data);
	
	caps_dishwasherOperatingState_data->set_supportedMachineStates_value(caps_dishwasherOperatingState_data, (const char **)support_Ms, 
								sizeof(support_Ms)/sizeof(char *));
	caps_dishwasherOperatingState_data->attr_supportedMachineStates_send(caps_dishwasherOperatingState_data);
	
	int j_idx = send_count % CAP_ENUM_DISHWASHEROPERATINGSTATE_DISHWASHERJOBSTATE_VALUE_MAX;
	caps_dishwasherOperatingState_data->set_dishwasherJobState_value(caps_dishwasherOperatingState_data, caps_helper_dishwasherOperatingState.
						attr_dishwasherJobState.values[j_idx]);
	caps_dishwasherOperatingState_data->attr_dishwasherJobState_send(caps_dishwasherOperatingState_data);
	//doorControl
	int door_idx = send_count % CAP_ENUM_DOORCONTROL_DOOR_VALUE_MAX;
	caps_doorControl_data->set_door_value(caps_doorControl_data, caps_helper_doorControl.attr_door.values[door_idx]);
	caps_doorControl_data->attr_door_send(caps_doorControl_data);
	//dryerMode
	int dryerMode_idx = send_count % CAP_ENUM_DRYERMODE_DRYERMODE_VALUE_MAX;
	caps_dryerMode_data->set_dryerMode_value(caps_dryerMode_data, caps_helper_dryerMode.
						attr_dryerMode.values[dryerMode_idx]);
	caps_dryerMode_data->attr_dryerMode_send(caps_dryerMode_data);
	//dustSensor
	int FDL_value = send_count * 10;
	caps_dustSensor_data->set_fineDustLevel_value(caps_dustSensor_data, FDL_value);
	caps_dustSensor_data->attr_fineDustLevel_send(caps_dustSensor_data);

	caps_dustSensor_data->set_dustLevel_value(caps_dustSensor_data, FDL_value);
	caps_dustSensor_data->attr_dustLevel_send(caps_dustSensor_data);
	//dryerOperatingState
	const char dcompletion_time[3][30]={"2020-05-09 13:13:02", "2020-05-10 14:14:02","2020-05-11 15:15:02"};
	int dtesttime_idx = send_count % 3;
	caps_dryerOperatingState_data->set_completionTime_value(caps_dryerOperatingState_data, dcompletion_time[dtesttime_idx]);
	caps_dryerOperatingState_data->attr_completionTime_send(caps_dryerOperatingState_data);

	caps_dryerOperatingState_data->set_supportedMachineStates_value(caps_dryerOperatingState_data, (const char **)support_Ms,
								sizeof(support_Ms)/sizeof(char *));
	caps_dryerOperatingState_data->attr_supportedMachineStates_send(caps_dryerOperatingState_data);

	int d_idx = send_count % CAP_ENUM_DRYEROPERATINGSTATE_DRYERJOBSTATE_VALUE_MAX;
	caps_dryerOperatingState_data->set_dryerJobState_value(caps_dryerOperatingState_data, caps_helper_dryerOperatingState.
						attr_dryerJobState.values[d_idx]);
	caps_dryerOperatingState_data->attr_dryerJobState_send(caps_dryerOperatingState_data);

}

void captest_prefix_d(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_dishwasherOperatingState_data = caps_dishwasherOperatingState_initialize(ctx, "main", cap_dishwasherOperatingState_init_cb, NULL);
	caps_doorControl_data = caps_doorControl_initialize(ctx, "main", cap_doorControl_init_cb, NULL);
	caps_dryerMode_data = caps_dryerMode_initialize(ctx, "main", cap_dryerMode_init_cb, NULL);
	caps_dustSensor_data = caps_dustSensor_initialize(ctx, "main", cap_dustSensor_init_cb, NULL);
	caps_dryerOperatingState_data = caps_dryerOperatingState_initialize(ctx, "main", cap_dryerOperatingState_init_cb, NULL);
	// CAPTEST : set cmd usr cb
	caps_dishwasherOperatingState_data->init_usr_cb = cap_dishwasherOperatingState_init_cb;
	caps_dishwasherOperatingState_data->cmd_setMachineState_usr_cb = cap_dishwasherOperatingState_cmd_cb;

	caps_doorControl_data->init_usr_cb = cap_doorControl_init_cb;
	caps_doorControl_data->cmd_close_usr_cb = cap_doorControl_cmd_cb;
	caps_doorControl_data->cmd_open_usr_cb = cap_doorControl_cmd_cb;
	
	caps_dryerMode_data->init_usr_cb = cap_dryerMode_init_cb;
	caps_dryerMode_data->cmd_setDryerMode_usr_cb = cap_dryerMode_cmd_cb;

	caps_dustSensor_data->init_usr_cb = cap_dustSensor_init_cb;

	caps_dryerOperatingState_data->init_usr_cb = cap_dryerOperatingState_init_cb;
	caps_dryerOperatingState_data->cmd_setMachineState_usr_cb = cap_dryerOperatingState_cmd_cb;
}
