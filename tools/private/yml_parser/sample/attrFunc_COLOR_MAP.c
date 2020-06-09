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
    int32_t sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    cap_evt[0] = st_cap_attr_create_number((char *) caps_helper_$CAPS_ID$.attr_hue.name, caps_data->hue_value, NULL);
    cap_evt[1] = st_cap_attr_create_number((char *) caps_helper_$CAPS_ID$.attr_saturation.name, caps_data->saturation_value, NULL);
    if (!cap_evt[0] || !cap_evt[1]) {
        printf("fail to create cap_evt\n");
        free(cap_evt[0]);
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, cap_evt);
    if (sequence_no < 0)
        printf("fail to send $ATTR_NAME$ data\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt[0]);
    st_cap_attr_free(cap_evt[1]);
}

