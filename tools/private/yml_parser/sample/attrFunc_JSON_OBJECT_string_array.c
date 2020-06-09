static const char **caps_$CAPS_ID$_get_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return NULL;
    }
    return (const char **)caps_data->$ATTR_NAME$_value;
}

static void caps_$CAPS_ID$_set_$ATTR_NAME$_value(caps_$CAPS_ID$_data_t *caps_data, const char **value, int arraySize)
{
    int i;
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    if (caps_data->$ATTR_NAME$_value) {
        for (i = 0; i < caps_data->$ATTR_NAME$_arraySize; i++) {
            free(caps_data->$ATTR_NAME$_value[i]);
        }
        free(caps_data->$ATTR_NAME$_value);
    }
    caps_data->$ATTR_NAME$_value = malloc(sizeof(char *) * arraySize);
    if (!caps_data->$ATTR_NAME$_value) {
        printf("fail to malloc for $ATTR_NAME$_value\n");
        caps_data->$ATTR_NAME$_arraySize = 0;
        return;
    }
    for (i = 0; i < arraySize; i++) {
        caps_data->$ATTR_NAME$_value[i] = strdup(value[i]);
    }

    caps_data->$ATTR_NAME$_arraySize = arraySize;
}

static void caps_$CAPS_ID$_attr_$ATTR_NAME$_send(caps_$CAPS_ID$_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }
    if (!caps_data->$ATTR_NAME$_value) {
        printf("value is NULL\n");
        return;
    }

    cap_evt = st_cap_attr_create_string_array((char *)caps_helper_$CAPS_ID$.attr_$ATTR_NAME$.name,
        caps_data->$ATTR_NAME$_arraySize, caps_data->$ATTR_NAME$_value, NULL);
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
