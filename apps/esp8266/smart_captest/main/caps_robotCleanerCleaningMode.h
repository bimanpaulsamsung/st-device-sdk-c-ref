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

#include "caps/iot_caps_helper_robotCleanerCleaningMode.h"

typedef struct caps_robotCleanerCleaningMode_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    char *robotCleanerCleaningMode_value;

    const char *(*get_robotCleanerCleaningMode_value)(struct caps_robotCleanerCleaningMode_data *caps_data);
    void (*set_robotCleanerCleaningMode_value)(struct caps_robotCleanerCleaningMode_data *caps_data, const char *value);
    int (*attr_robotCleanerCleaningMode_str2idx)(const char *value);
    void (*attr_robotCleanerCleaningMode_send)(struct caps_robotCleanerCleaningMode_data *caps_data);

    void (*init_usr_cb)(struct caps_robotCleanerCleaningMode_data *caps_data);

    void (*cmd_setRobotCleanerCleaningMode_usr_cb)(struct caps_robotCleanerCleaningMode_data *caps_data);
} caps_robotCleanerCleaningMode_data_t;

caps_robotCleanerCleaningMode_data_t *caps_robotCleanerCleaningMode_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);
