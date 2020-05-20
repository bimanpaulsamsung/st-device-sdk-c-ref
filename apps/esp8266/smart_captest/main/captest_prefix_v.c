#include "st_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "caps_voltageMeasurement.h"
#include "caps_videoStream.h"
#include "caps_videoCapture.h"
#include "caps_videoCamera.h"
#include "caps_veryFineDustSensor.h"
#include "caps_valve.h"

static caps_voltageMeasurement_data_t *caps_voltageMeasurement_data;
static caps_videoStream_data_t *caps_videoStream_data;
static caps_videoCapture_data_t *caps_videoCapture_data;
static caps_videoCamera_data_t *caps_videoCamera_data;
static caps_veryFineDustSensor_data_t *caps_veryFineDustSensor_data;
static caps_valve_data_t *caps_valve_data;

#define INIT_TEST_FOR_CAMERA_STRING "test camera status message value"
static char *statusMessage[] = {"unmute", "on", "off", "filp", "mute"};

// CAPTEST : make init_cb for test capabilities
static void cap_voltageMeasurement_init_cb(struct caps_voltageMeasurement_data *caps_data)
{
	caps_data->set_voltage_value(caps_data, caps_helper_voltageMeasurement.
					attr_voltage.min);
	caps_data->set_voltage_unit(caps_data, caps_helper_voltageMeasurement.
					attr_voltage.units[CAP_ENUM_VOLTAGEMEASUREMENT_VOLTAGE_UNIT_V]);
}

static void cap_videoStream_init_cb(struct caps_videoStream_data *caps_data)
{
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "test for videoStream");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", 0);
	caps_data->set_stream_value(caps_data, root);
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

static void cap_veryFineDustSensor_init_cb(struct caps_veryFineDustSensor_data *caps_data)
{
	caps_data->set_veryFineDustLevel_value(caps_data, caps_helper_veryFineDustSensor.
					attr_veryFineDustLevel.min);
	caps_data->set_veryFineDustLevel_unit(caps_data, caps_helper_veryFineDustSensor.
					attr_veryFineDustLevel.units[CAP_ENUM_VERYFINEDUSTSENSOR_VERYFINEDUSTLEVEL_UNIT_UG_PER_M3]);
}

static void cap_valve_init_cb(struct caps_valve_data *caps_data)
{
	caps_data->set_valve_value(caps_data, caps_helper_valve.
					attr_valve.values[CAP_ENUM_VALVE_VALVE_VALUE_CLOSED]);
}

// captest : make cmd_cb for test capabilities
static void cap_videoStream_stop_cmd_cb(struct caps_videoStream_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_videoStream_start_cmd_cb(struct caps_videoStream_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_capture_cmd_cb(struct caps_videoCapture_data *caps_data)
{
	printf("[%s] callback\n", __func__);
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

static void cap_close_cmd_cb(struct caps_valve_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}

static void cap_open_cmd_cb(struct caps_valve_data *caps_data)
{
	printf("[%s] callback\n", __func__);
}
// captest : send attrbutes for test capabilities(called by button press)
void send_v_test_capabilities()
{
	static int send_count = 0;
	send_count++;
	//voltageMeasurement
	int v_idx = send_count % CAP_ENUM_VOLTAGEMEASUREMENT_VOLTAGE_UNIT_MAX;
	caps_voltageMeasurement_data->set_voltage_unit(caps_voltageMeasurement_data,
								caps_helper_voltageMeasurement.attr_voltage.
								units[v_idx]);
	caps_voltageMeasurement_data->set_voltage_value(caps_voltageMeasurement_data, (double)send_count * 10);
	caps_voltageMeasurement_data->attr_voltage_send(caps_voltageMeasurement_data);
	//videostream
	JSON_H *root = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(root, "description", "test for videostream");
	JSON_ADD_NUMBER_TO_OBJECT(root, "count", send_count);
	caps_videoStream_data->set_stream_value(caps_videoStream_data, root);
	caps_videoStream_data->attr_stream_send(caps_videoStream_data);
	JSON_FREE(root);
	//videoCapture
	JSON_H *clip_value = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(clip_value, "clip", "test for clip");
	JSON_ADD_NUMBER_TO_OBJECT(clip_value, "count", send_count);
	caps_videoCapture_data->set_clip_value(caps_videoCapture_data, clip_value);
	JSON_FREE(clip_value);

	JSON_H *stream_value = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(stream_value, "stream", "test for stream");
	JSON_ADD_NUMBER_TO_OBJECT(stream_value, "count", send_count);
	caps_videoCapture_data->set_stream_value(caps_videoCapture_data, stream_value);
	JSON_FREE(stream_value);
	//videoCamera
	int m_idx = send_count % CAP_ENUM_VIDEOCAMERA_MUTE_VALUE_MAX;
	caps_videoCamera_data->set_mute_value(caps_videoCamera_data, caps_helper_videoCamera.attr_mute.
							values[m_idx]);
	caps_videoCamera_data->attr_mute_send(caps_videoCamera_data);

	int c_idx = send_count % CAP_ENUM_VIDEOCAMERA_CAMERA_VALUE_MAX;
	caps_videoCamera_data->set_camera_value(caps_videoCamera_data, caps_helper_videoCamera.attr_camera.
							values[c_idx]);
	caps_videoCamera_data->attr_camera_send(caps_videoCamera_data);

	int s_idx = send_count % 5;
	caps_videoCamera_data->set_statusMessage_value(caps_videoCamera_data, statusMessage[s_idx]);
	caps_videoCamera_data->attr_statusMessage_send(caps_videoCamera_data);

	JSON_H *c_value = JSON_CREATE_OBJECT();
	JSON_ADD_STRING_TO_OBJECT(c_value, "camera", "test for camera");
	JSON_ADD_NUMBER_TO_OBJECT(c_value, "count", send_count);
	caps_videoCamera_data->set_settings_value(caps_videoCamera_data, c_value);
	caps_videoCamera_data->attr_settings_send(caps_videoCamera_data);
	JSON_FREE(root);
	//veryFineDustSensor
	int f_idx = send_count % CAP_ENUM_VERYFINEDUSTSENSOR_VERYFINEDUSTLEVEL_UNIT_MAX;
	caps_veryFineDustSensor_data->set_veryFineDustLevel_unit(caps_veryFineDustSensor_data,
								caps_helper_veryFineDustSensor.attr_veryFineDustLevel.
								units[f_idx]);
	caps_veryFineDustSensor_data->set_veryFineDustLevel_value(caps_veryFineDustSensor_data, send_count * 10);
	caps_veryFineDustSensor_data->attr_veryFineDustLevel_send(caps_veryFineDustSensor_data);
	//valve
	int valve_idx = send_count % CAP_ENUM_VALVE_VALVE_VALUE_MAX;
	caps_valve_data->set_valve_value(caps_valve_data, caps_helper_valve.
					attr_valve.values[valve_idx]);
	caps_valve_data->attr_valve_send(caps_valve_data);
}

void captest_v_initialize(IOT_CTX *ctx)
{
	// CAPTEST : call initialize function and set init_cb
	caps_voltageMeasurement_data = caps_voltageMeasurement_initialize(ctx, "main", cap_voltageMeasurement_init_cb, NULL);
	caps_videoStream_data = caps_videoStream_initialize(ctx, "main", cap_videoStream_init_cb, NULL);
	caps_videoCapture_data = caps_videoCapture_initialize(ctx, "main", cap_videoCapture_init_cb, NULL);
	caps_videoCamera_data = caps_videoCamera_initialize(ctx, "main", cap_videoCamera_init_cb, NULL);
	caps_veryFineDustSensor_data = caps_veryFineDustSensor_initialize(ctx, "main", cap_veryFineDustSensor_init_cb, NULL);
	caps_valve_data = caps_valve_initialize(ctx, "main", cap_valve_init_cb, NULL);
	// CAPTEST : set cmd usr cb
	caps_videoStream_data->cmd_stopStream_usr_cb = cap_videoStream_stop_cmd_cb;
	caps_videoStream_data->cmd_startStream_usr_cb = cap_videoStream_start_cmd_cb;

	caps_videoCapture_data->cmd_capture_usr_cb = cap_capture_cmd_cb;

	caps_videoCamera_data->cmd_unmute_usr_cb = cap_unmute_cmd_cb;
	caps_videoCamera_data->cmd_on_usr_cb = cap_on_cmd_cb;
	caps_videoCamera_data->cmd_off_usr_cb = cap_off_cmd_cb;
	caps_videoCamera_data->cmd_flip_usr_cb = cap_flip_cmd_cb;
	caps_videoCamera_data->cmd_mute_usr_cb = cap_mute_cmd_cb;

	caps_valve_data->cmd_close_usr_cb = cap_close_cmd_cb;
	caps_valve_data->cmd_open_usr_cb = cap_open_cmd_cb;
}
