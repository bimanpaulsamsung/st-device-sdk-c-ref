static void caps_$CAPS_ID$_set_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data, double hue, double saturation)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->$ATTR_NAME$_value) {
        free(caps_data->$ATTR_NAME$_value);
    }
    caps_data->hue_value = hue;
    caps_data->saturation_value = saturation;
}

static void caps_$CAPS_ID$_attr_$ATTR_NAME$_send(caps_$CAPS_ID$_data_t *caps_data)
{
    IOT_EVENT *cap_evt[2];
    uint8_t evt_num = 2;
    iot_cap_val_t value[2];
    int32_t sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    value[0].type = IOT_CAP_VAL_TYPE_NUMBER;
    value[0].number = caps_data->hue_value;

    cap_evt[0] = st_cap_create_attr(caps_data->handle,
            (char *) caps_helper_$CAPS_ID$.attr_hue.name,
            &value[0],
            NULL,
            NULL);

    value[1].type = IOT_CAP_VAL_TYPE_NUMBER;
    value[1].number = caps_data->saturation_value;

    cap_evt[1] = st_cap_create_attr(caps_data->handle,
            (char *) caps_helper_$CAPS_ID$.attr_saturation.name,
            &value[1],
            NULL,
            NULL);

    if (!cap_evt[0] || !cap_evt[1]) {
        printf("fail to create cap_evt\n");
        free(cap_evt[0]);
        free(cap_evt[1]);
        return;
    }

    sequence_no = st_cap_send_attr(cap_evt, evt_num);
    if (sequence_no < 0)
        printf("fail to send $ATTR_NAME$ data\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_free_attr(cap_evt[0]);
    st_cap_free_attr(cap_evt[1]);
}

