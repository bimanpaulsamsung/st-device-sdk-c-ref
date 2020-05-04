static void caps_colorControl_cmd_setColor_cb(IOT_CAP_HANDLE *handle,
        iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_colorControl_data_t *caps_data = usr_data;
    JSON_H* json_object;
    JSON_H* item;
    double hue = caps_helper_colorControl.attr_hue.min - 1;
    double saturation = caps_helper_colorControl.attr_saturation.min - 1;

    printf("called [%s] func with : num_args:%u\n", __func__, cmd_data->num_args);
	caps_data->cmd_data = cmd_data;

    json_object = JSON_PARSE(cmd_data->cmd_data[0].json_object);
    if (!json_object) {
        printf("fail to parse json object\n");
        return;
    }
    item = JSON_GET_OBJECT_ITEM(json_object, "hue");
    if (JSON_IS_NUMBER(item)) {
        hue = item->valuedouble;
    }
    item = JSON_GET_OBJECT_ITEM(json_object, "saturation");
    if (JSON_IS_NUMBER(item)) {
        saturation = item->valuedouble;
    }
    JSON_DELETE(json_object);

    caps_colorControl_set_color_value(caps_data, hue, saturation);
    if (caps_data && caps_data->cmd_setColor_usr_cb)
        caps_data->cmd_setColor_usr_cb(caps_data);
    caps_colorControl_attr_color_send(caps_data);
}
