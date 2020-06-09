static void caps_$CAPS_ID$_cmd_$CMDS_NAME$_cb(IOT_CAP_HANDLE *handle,
        iot_cap_cmd_data_t *cmd_data, void *usr_data)
{
    caps_$CAPS_ID$_data_t *caps_data = usr_data;

    printf("called [%s] func with : num_args:%u\n", __func__, cmd_data->num_args);
    caps_data->cmd_data = cmd_data;

    if (caps_data && caps_data->cmd_$CMDS_NAME$_usr_cb)
        caps_data->cmd_$CMDS_NAME$_usr_cb(caps_data);
}
