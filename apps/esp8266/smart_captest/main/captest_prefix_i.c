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

#include "caps_infraredLevel.h"
#include "caps_imageCapture.h"
#include "caps_illuminanceMeasurement.h"

static caps_infraredLevel_data_t *caps_infraredLevel_data;
static caps_imageCapture_data_t *caps_imageCapture_data;
static caps_illuminanceMeasurement_data_t *caps_illuminanceMeasurement_data;

#define INIT_IMAGE_CAPTURE_VALUE	"test_image_capture_value"
#define INIT_IMAGE_CAPTURE_TIME		"2020-05-15 13:13:14"

// CAPTEST : make init_cb for test capabilities
static void cap_infraredLevel_init_cb(struct caps_infraredLevel_data *caps_data)
{
	caps_data->set_infraredLevel_value(caps_data, caps_helper_infraredLevel.attr_infraredLevel.min);
	caps_data->set_infraredLevel_unit(caps_data, caps_helper_infraredLevel.
					attr_infraredLevel.units[CAP_ENUM_INFRAREDLEVEL_INFRAREDLEVEL_UNIT_PERCENT]);
}

static void cap_imageCapture_init_cb(struct caps_imageCapture_data *caps_data)
{
	caps_data->set_image_value(caps_data, INIT_IMAGE_CAPTURE_VALUE);
	caps_data->set_captureTime_value(caps_data, INIT_IMAGE_CAPTURE_TIME);
}

static void cap_illuminanceMeasurement_init_cb(struct caps_illuminanceMeasurement_data *caps_data)
{
	caps_data->set_illuminance_value(caps_data, caps_helper_illuminanceMeasurement.
								attr_illuminance.min);
	caps_data->set_illuminance_unit(caps_data, caps_helper_illuminanceMeasurement.
					attr_illuminance.units[CAP_ENUM_ILLUMINANCEMEASUREMENT_ILLUMINANCE_UNIT_LUX]);
}
// CAPTEST : make cmd_cb for test capabilities
static void cap_infraredLevel_cmd_cb(struct caps_infraredLevel_data *caps_data)
{
	double Level_value = caps_data->get_infraredLevel_value(caps_data);
	const char *Level_unit = caps_data->get_infraredLevel_unit(caps_data);
	printf("[%s] Level value is : %f\n", __func__, Level_value);
	printf("[%s] Level unit is : %s\n", __func__, Level_unit);
}

static void cap_imageCapture_cmd_cb(struct caps_imageCapture_data *caps_data)
{
	const char *image_value = caps_data->get_image_value(caps_data);
	const char *capture_time = caps_data->get_captureTime_value(caps_data);
	printf("[%s] image value is : %s\n", __func__, image_value);
	printf("[%s] capture time is : %s\n", __func__, capture_time);
}

// CAPTEST : send attrbutes for test capabilities(called by button press)
void send_i_test_capabilities()
{
	static int send_count = 0;
	send_count++;

	//infraredLevel
	int u_idx = send_count % CAP_ENUM_INFRAREDLEVEL_INFRAREDLEVEL_UNIT_MAX;
	caps_infraredLevel_data->set_infraredLevel_value(caps_infraredLevel_data, send_count);
	caps_infraredLevel_data->set_infraredLevel_unit(caps_infraredLevel_data,
					caps_helper_infraredLevel.attr_infraredLevel.units[u_idx]);
	caps_infraredLevel_data->attr_infraredLevel_send(caps_infraredLevel_data);
	//imageCapture
	char image_value[50];
	snprintf(image_value, 50, "image_value%d", send_count);
	caps_imageCapture_data->set_image_value(caps_imageCapture_data, image_value);
	caps_imageCapture_data->attr_image_send(caps_imageCapture_data);
	char test_capture_time[50];
	snprintf(test_capture_time, 50, "2020-05-15 13:13:1%d", send_count);
	caps_imageCapture_data->set_captureTime_value(caps_imageCapture_data, test_capture_time);
	caps_imageCapture_data->attr_captureTime_send(caps_imageCapture_data);
	//illuminanceMeasurement
	int ill_idx = send_count % CAP_ENUM_ILLUMINANCEMEASUREMENT_ILLUMINANCE_UNIT_MAX;
	caps_illuminanceMeasurement_data->set_illuminance_value(caps_illuminanceMeasurement_data, send_count);
	caps_illuminanceMeasurement_data->set_illuminance_unit(caps_illuminanceMeasurement_data,
					caps_helper_illuminanceMeasurement.attr_illuminance.units[ill_idx]);
	caps_illuminanceMeasurement_data->attr_illuminance_send(caps_illuminanceMeasurement_data);

}

void captest_i_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_infraredLevel_data = caps_infraredLevel_initialize(ctx, "main", cap_infraredLevel_init_cb, NULL);
	caps_imageCapture_data = caps_imageCapture_initialize(ctx, "main", cap_imageCapture_init_cb, NULL);
	caps_illuminanceMeasurement_data = caps_illuminanceMeasurement_initialize(ctx, "main", cap_illuminanceMeasurement_init_cb, NULL);

	// CAPTEST : set cmd usr cb
	caps_infraredLevel_data->cmd_setInfraredLevel_usr_cb = cap_infraredLevel_cmd_cb;
	caps_imageCapture_data->cmd_take_usr_cb = cap_imageCapture_cmd_cb;
}
