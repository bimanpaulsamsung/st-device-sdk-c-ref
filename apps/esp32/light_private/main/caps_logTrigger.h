/* ***************************************************************************
 *
 * Copyright 2019-2020 Samsung Electronics All Rights Reserved.
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

#include "caps/iot_caps_helper_logTrigger.h"
#include "external/JSON.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct caps_logTrigger_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    JSON_H *logInfo_value;
    char *logState_value;
    char *logRequestState_value;

    const JSON_H *(*get_logInfo_value)(struct caps_logTrigger_data *caps_data);
    void (*set_logInfo_value)(struct caps_logTrigger_data *caps_data, const JSON_H *value);
    void (*attr_logInfo_send)(struct caps_logTrigger_data *caps_data);
    const char *(*get_logState_value)(struct caps_logTrigger_data *caps_data);
    void (*set_logState_value)(struct caps_logTrigger_data *caps_data, const char *value);
    int (*attr_logState_str2idx)(const char *value);
    void (*attr_logState_send)(struct caps_logTrigger_data *caps_data);
    const char *(*get_logRequestState_value)(struct caps_logTrigger_data *caps_data);
    void (*set_logRequestState_value)(struct caps_logTrigger_data *caps_data, const char *value);
    int (*attr_logRequestState_str2idx)(const char *value);
    void (*attr_logRequestState_send)(struct caps_logTrigger_data *caps_data);

    void (*init_usr_cb)(struct caps_logTrigger_data *caps_data);

    void (*cmd_triggerLog_usr_cb)(struct caps_logTrigger_data *caps_data);
    void (*cmd_triggerLogWithLogInfo_usr_cb)(struct caps_logTrigger_data *caps_data);
    void (*cmd_triggerLogWithUrl_usr_cb)(struct caps_logTrigger_data *caps_data);
} caps_logTrigger_data_t;

caps_logTrigger_data_t *caps_logTrigger_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);
#ifdef __cplusplus
}
#endif

