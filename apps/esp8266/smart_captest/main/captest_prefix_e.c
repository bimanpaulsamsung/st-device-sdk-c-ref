#include "st_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "caps_elevatorCall.h"
#include "caps_energyMeter.h"
#include "caps_equivalentCarbonDioxideMeasurement.h"
#include "caps_execute.h"

static caps_elevatorCall_data_t *caps_elevatorCall_data;
static caps_energyMeter_data_t *caps_energyMeter_data;
static caps_equivalentCarbonDioxideMeasurement_data_t *caps_equivalentCarbonDioxideMeasurement_data;
static caps_execute_data_t *caps_execute_data;

// CAPTEST : make init_cb for test capabilities
static void cap_elevatorCall_init_cb(struct caps_elevatorCall_data *caps_data)
{
	caps_data->set_callStatus_value(caps_data,
				caps_helper_elevatorCall.attr_callStatus.values[CAP_ENUM_ELEVATORCALL_CALLSTATUS_VALUE_CALLED]);
}

static void cap_energyMeter_init_cb(struct caps_energyMeter_data *caps_data)
{
	caps_data->set_energy_value(caps_data,caps_helper_energyMeter.attr_energy.min);
	caps_data->set_energy_unit(caps_data, caps_helper_energyMeter.attr_energy.units[CAP_ENUM_ENERGYMETER_ENERGY_UNIT_WH]);
}

static void cap_equivalentCarbonDioxideMeasurement_init_cb(struct caps_equivalentCarbonDioxideMeasurement_data *caps_data)
{
	caps_data->set_equivalentCarbonDioxideMeasurement_value(caps_data,
					caps_helper_equivalentCarbonDioxideMeasurement.
					attr_equivalentCarbonDioxideMeasurement.min);
	caps_data->set_equivalentCarbonDioxideMeasurement_unit(caps_data,
					caps_helper_equivalentCarbonDioxideMeasurement.
					attr_equivalentCarbonDioxideMeasurement.
					units[CAP_ENUM_EQUIVALENTCARBONDIOXIDEMEASUREMENT_EQUIVALENTCARBONDIOXIDEMEASUREMENT_UNIT_PPM]);
}

static void cap_execute_init_cb(struct caps_execute_data *caps_data)
{
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "init test for execute");

	caps_data->set_data_value(caps_data, root);
	JSON_FREE(root);
}
// CAPTEST : make cmd_cb for test capabilities
static void cap_elevatorCall_cmd_cb(struct caps_elevatorCall_data *caps_data)
{
	const char *value = caps_data->get_callStatus_value(caps_data);
	printf("%s: value:%s\n",__func__,value);
}

static void cap_execute_cmd_cb(struct caps_execute_data *caps_data)
{
	char *json_data = NULL;
	const JSON_H *data_value = caps_data->get_data_value(caps_data);

	printf("data:%s\n",json_data = JSON_PRINT(data_value));
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_e_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	//elevatorCall
	int call_idx = send_count % CAP_ENUM_ELEVATORCALL_CALLSTATUS_VALUE_MAX;
	caps_elevatorCall_data->set_callStatus_value(caps_elevatorCall_data,
					caps_helper_elevatorCall.attr_callStatus.values[call_idx]);
	caps_elevatorCall_data->attr_callStatus_send(caps_elevatorCall_data);
	//energyMeter
	int em_idx = send_count % CAP_ENUM_ENERGYMETER_ENERGY_UNIT_MAX;
	caps_energyMeter_data->set_energy_unit(caps_energyMeter_data,
					caps_helper_energyMeter.attr_energy.units[em_idx]);
	caps_energyMeter_data->set_energy_value(caps_energyMeter_data, send_count);
	caps_energyMeter_data->attr_energy_send(caps_energyMeter_data);
	//equivalentCarbonDioxideMeasurement
	int ecdm_idx = send_count % CAP_ENUM_EQUIVALENTCARBONDIOXIDEMEASUREMENT_EQUIVALENTCARBONDIOXIDEMEASUREMENT_UNIT_MAX;
	caps_equivalentCarbonDioxideMeasurement_data->set_equivalentCarbonDioxideMeasurement_unit(caps_equivalentCarbonDioxideMeasurement_data,
					caps_helper_equivalentCarbonDioxideMeasurement.
					attr_equivalentCarbonDioxideMeasurement.units[ecdm_idx]);
	caps_equivalentCarbonDioxideMeasurement_data->set_equivalentCarbonDioxideMeasurement_value(caps_equivalentCarbonDioxideMeasurement_data, send_count);
	caps_equivalentCarbonDioxideMeasurement_data->attr_equivalentCarbonDioxideMeasurement_send(caps_equivalentCarbonDioxideMeasurement_data);

	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "test for execute");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", send_count);

	caps_execute_data->set_data_value(caps_execute_data, root);
	caps_execute_data->attr_data_send(caps_execute_data);
	JSON_FREE(root);
}

void captest_e_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_elevatorCall_data = caps_elevatorCall_initialize(ctx, "main", cap_elevatorCall_init_cb, NULL);
	caps_energyMeter_data = caps_energyMeter_initialize(ctx, "main", cap_energyMeter_init_cb, NULL);
	caps_equivalentCarbonDioxideMeasurement_data = caps_equivalentCarbonDioxideMeasurement_initialize(ctx, "main",
					cap_equivalentCarbonDioxideMeasurement_init_cb, NULL);
	caps_execute_data = caps_execute_initialize(ctx, "main", cap_execute_init_cb, NULL);
	// CAPTEST : set cmd usr cb
	caps_elevatorCall_data->cmd_call_usr_cb = cap_elevatorCall_cmd_cb;
	caps_execute_data->cmd_execute_usr_cb = cap_execute_cmd_cb;
}
