#include "st_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "caps_contactSensor.h"
#include "caps_colorTemperature.h"
#include "caps_carbonMonoxideMeasurement.h"
#include "caps_configuration.h"

static caps_contactSensor_data_t *caps_contactSensor_data;
static caps_colorTemperature_data_t *caps_colorTemperature_data;
static caps_carbonMonoxideMeasurement_data_t *caps_carbonMonoxideMeasurement_data;
static caps_configuration_data_t *caps_configuration_data;

// CAPTEST : make init_cb for test capabilities
static void cap_contactSensor_init_cb(struct caps_contactSensor_data *caps_data)
{
	caps_data->set_contact_value(caps_data, caps_helper_contactSensor.
					attr_contact.values[CAP_ENUM_CONTACTSENSOR_CONTACT_VALUE_CLOSED]);
}

static void cap_configuration_init_cb(struct caps_configuration_data *caps_data)
{
	//todo
}

static void cap_colorTemperature_init_cb(struct caps_colorTemperature_data *caps_data)
{
	caps_data->set_colorTemperature_value(caps_data, caps_helper_colorTemperature.
									attr_colorTemperature.min);
	caps_data->set_colorTemperature_unit(caps_data, caps_helper_colorTemperature.
					attr_colorTemperature.units[CAP_ENUM_COLORTEMPERATURE_COLORTEMPERATURE_UNIT_K]);
}

static void cap_carbonMonoxideMeasurement_init_cb(struct caps_carbonMonoxideMeasurement_data *caps_data)
{
	caps_data->set_carbonMonoxideLevel_value(caps_data,
					caps_helper_carbonMonoxideMeasurement.attr_carbonMonoxideLevel.min);
	caps_data->set_carbonMonoxideLevel_unit(caps_data, caps_helper_carbonMonoxideMeasurement.
					attr_carbonMonoxideLevel.units[CAP_ENUM_CARBONMONOXIDEMEASUREMENT_CARBONMONOXIDELEVEL_UNIT_PPM]);
}
// CAPTEST : make cmd_cb for test capabilities
static void cap_colorTemperature_cmd_usr_cb(struct caps_colorTemperature_data *caps_data)
{
	int value = caps_data->get_colorTemperature_value(caps_data);
	const char *unit_value = caps_data->get_colorTemperature_unit(caps_data);

	printf("%s: value :%d\n",__func__, value);
	printf("%s: unit_value :%s\n",__func__, unit_value);
}

static void cap_configuration_cmd_usr_cb(struct caps_configuration_data *caps_data)
{
	//todo
}
// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_c_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	//contactSensor
	int idx = send_count  % CAP_ENUM_CONTACTSENSOR_CONTACT_VALUE_MAX;
	caps_contactSensor_data->set_contact_value(caps_contactSensor_data, caps_helper_contactSensor.attr_contact.values[idx]);
	caps_contactSensor_data->attr_contact_send(caps_contactSensor_data);
	//colorTemperature
	int value = (send_count * 10) % 30000;
	caps_colorTemperature_data->set_colorTemperature_value(caps_colorTemperature_data, value);
	caps_colorTemperature_data->attr_colorTemperature_send(caps_colorTemperature_data);
	//carbonMonoxideMeasurement
	int cvalue = (send_count * 100) % 1000000;
	caps_carbonMonoxideMeasurement_data->set_carbonMonoxideLevel_value(caps_carbonMonoxideMeasurement_data, cvalue);
	caps_carbonMonoxideMeasurement_data->attr_carbonMonoxideLevel_send(caps_carbonMonoxideMeasurement_data);
}

void captest_c_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_contactSensor_data = caps_contactSensor_initialize(ctx, "main", cap_contactSensor_init_cb, NULL);
	caps_colorTemperature_data = caps_colorTemperature_initialize(ctx, "main", cap_colorTemperature_init_cb, NULL);
	caps_carbonMonoxideMeasurement_data = caps_carbonMonoxideMeasurement_initialize(ctx, "main", cap_carbonMonoxideMeasurement_init_cb, NULL);
	caps_configuration_data = caps_configuration_initialize(ctx, "main", cap_configuration_init_cb, NULL);
	// CAPTEST : set cmd usr cb
	caps_colorTemperature_data->cmd_setColorTemperature_usr_cb = cap_colorTemperature_cmd_usr_cb;
	caps_configuration_data->cmd_configure_usr_cb = cap_configuration_cmd_usr_cb;
}
