static const JSON_H *caps_$CAPS_ID$_get_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data)
{
	if (!caps_data) {
		printf("caps_data is NULL\n");
		return NULL;
	}
	return (const JSON_H *)caps_data->$ATTR_NAME$_value;
}

static void caps_$CAPS_ID$_set_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data, const JSON_H *value)
{
	if (!caps_data) {
		printf("caps_data is NULL\n");
		return;
	}
	if (caps_data->$ATTR_NAME$_value) {
		JSON_DELETE(caps_data->$ATTR_NAME$_value);
	}
	caps_data->$ATTR_NAME$_value = JSON_DUPLICATE(value, true);
}

static void caps_$CAPS_ID$_attr_$ATTR_NAME$_send(caps_$CAPS_ID$_data_t *caps_data)
{
	IOT_EVENT *cap_evt;
	uint8_t evt_num = 1;
	int sequence_no;
	iot_cap_val_t value;

	if (!caps_data || !caps_data->handle) {
		printf("fail to get handle\n");
		return;
	}
	if (!caps_data->$ATTR_NAME$_value) {
		printf("value is NULL\n");
		return;
	}

	value.type = IOT_CAP_VAL_TYPE_JSON_OBJECT;
	value.json_object = JSON_PRINT(caps_data->$ATTR_NAME$_value);

	cap_evt = st_cap_attr_create((char *)caps_helper_$CAPS_ID$.attr_$ATTR_NAME$.name,
		&value, NULL, NULL);
	if (!cap_evt) {
		printf("fail to create cap_evt\n");
		return;
	}

	sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
	if (sequence_no < 0)
		printf("fail to send $ATTR_NAME$ value\n");

	printf("Sequence number return : %d\n", sequence_no);
	st_cap_attr_free(cap_evt);
}
