static int caps_$CAPS_ID$_attr_$ATTR_NAME$_str2idx(const char *value)
{
    int index;

    for (index = 0; index < $ENUM_PREFIX$$CAPS_ID_UP$_$ATTR_NAME_UP$_VALUE_MAX; index++) {
        if (!strcmp(value, caps_helper_$CAPS_ID$.attr_$ATTR_NAME$.values[index])) {
            return index;
        }
    }
    return -1;
}

static const char *caps_$CAPS_ID$_get_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return caps_data->$ATTR_NAME$_value;
}

static void caps_$CAPS_ID$_set_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data, const char *value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->$ATTR_NAME$_value) {
        free(caps_data->$ATTR_NAME$_value);
    }
    caps_data->$ATTR_NAME$_value = strdup(value);
}

static void caps_$CAPS_ID$_attr_$ATTR_NAME$_send(caps_$CAPS_ID$_data_t *caps_data)
{
    int sequence_no = -1;
    IOT_EVENT *attr = NULL;
    iot_cap_val_t value;
    iot_cap_attr_option_t attr_option = { 0 };

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->$ATTR_NAME$_value) {
        printf("value is NULL\n");
        return;
    }

    value.type = IOT_CAP_VAL_TYPE_STRING;
    value.string = caps_data->$ATTR_NAME$_value;

    attr_option.state_change = true;

    attr = st_cap_create_attr_with_option(caps_data->handle, (char *)caps_helper_$CAPS_ID$.attr_$ATTR_NAME$.name, &value, NULL, NULL, &attr_option);
    if (attr != NULL){
        sequence_no = st_cap_send_attr(&attr, 1);
        st_cap_free_attr(attr);
    }

    if (sequence_no < 0)
        printf("fail to send $ATTR_NAME$ value\n");
    else
        printf("Sequence number return : %d\n", sequence_no);

}
