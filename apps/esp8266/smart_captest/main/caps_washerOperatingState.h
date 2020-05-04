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

#include "caps/iot_caps_helper_washerOperatingState.h"
#include "JSON.h"

typedef struct caps_washerOperatingState_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    char *machineState_value;
    char *completionTime_value;
    char **supportedMachineStates_value;
    int supportedMachineStates_arraySize;
    char *washerJobState_value;

    const char *(*get_machineState_value)(struct caps_washerOperatingState_data *caps_data);
    void (*set_machineState_value)(struct caps_washerOperatingState_data *caps_data, const char *value);
    int (*attr_machineState_str2idx)(const char *value);
    void (*attr_machineState_send)(struct caps_washerOperatingState_data *caps_data);
    const char *(*get_completionTime_value)(struct caps_washerOperatingState_data *caps_data);
    void (*set_completionTime_value)(struct caps_washerOperatingState_data *caps_data, const char *value);
    void (*attr_completionTime_send)(struct caps_washerOperatingState_data *caps_data);
    const char **(*get_supportedMachineStates_value)(struct caps_washerOperatingState_data *caps_data);
    void (*set_supportedMachineStates_value)(struct caps_washerOperatingState_data *caps_data, const char **value, int arraySize);
    void (*attr_supportedMachineStates_send)(struct caps_washerOperatingState_data *caps_data);
    const char *(*get_washerJobState_value)(struct caps_washerOperatingState_data *caps_data);
    void (*set_washerJobState_value)(struct caps_washerOperatingState_data *caps_data, const char *value);
    int (*attr_washerJobState_str2idx)(const char *value);
    void (*attr_washerJobState_send)(struct caps_washerOperatingState_data *caps_data);

    void (*init_usr_cb)(struct caps_washerOperatingState_data *caps_data);

    void (*cmd_setMachineState_usr_cb)(struct caps_washerOperatingState_data *caps_data);
} caps_washerOperatingState_data_t;

caps_washerOperatingState_data_t *caps_washerOperatingState_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);
