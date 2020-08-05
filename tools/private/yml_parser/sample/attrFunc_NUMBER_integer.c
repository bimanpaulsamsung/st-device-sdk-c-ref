static int caps_$CAPS_ID$_get_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return caps_helper_$CAPS_ID$.attr_$ATTR_NAME$.min - 1;
    }
    return caps_data->$ATTR_NAME$_value;
}

static void caps_$CAPS_ID$_set_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data, int value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->$ATTR_NAME$_value = value;
}

static void caps_$CAPS_ID$_attr_$ATTR_NAME$_send(caps_$CAPS_ID$_data_t *caps_data)
{
    int sequence_no = -1;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    ST_CAP_SEND_ATTR_NUMBER(caps_data->handle,
            (char *)caps_helper_$CAPS_ID$.attr_$ATTR_NAME$.name,
            caps_data->$ATTR_NAME$_value,
            NULL,
            NULL,
            sequence_no);

    if (sequence_no < 0)
        printf("fail to send $ATTR_NAME$ value\n");
    else
        printf("Sequence number return : %d\n", sequence_no);

}
