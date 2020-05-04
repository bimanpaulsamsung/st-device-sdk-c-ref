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

#include "caps/iot_caps_helper_refrigeration.h"

typedef struct caps_refrigeration_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    char *defrost_value;
    char *rapidFreezing_value;
    char *rapidCooling_value;

    const char *(*get_defrost_value)(struct caps_refrigeration_data *caps_data);
    void (*set_defrost_value)(struct caps_refrigeration_data *caps_data, const char *value);
    int (*attr_defrost_str2idx)(const char *value);
    void (*attr_defrost_send)(struct caps_refrigeration_data *caps_data);
    const char *(*get_rapidFreezing_value)(struct caps_refrigeration_data *caps_data);
    void (*set_rapidFreezing_value)(struct caps_refrigeration_data *caps_data, const char *value);
    int (*attr_rapidFreezing_str2idx)(const char *value);
    void (*attr_rapidFreezing_send)(struct caps_refrigeration_data *caps_data);
    const char *(*get_rapidCooling_value)(struct caps_refrigeration_data *caps_data);
    void (*set_rapidCooling_value)(struct caps_refrigeration_data *caps_data, const char *value);
    int (*attr_rapidCooling_str2idx)(const char *value);
    void (*attr_rapidCooling_send)(struct caps_refrigeration_data *caps_data);

    void (*init_usr_cb)(struct caps_refrigeration_data *caps_data);

    void (*cmd_setRapidCooling_usr_cb)(struct caps_refrigeration_data *caps_data);
    void (*cmd_setRapidFreezing_usr_cb)(struct caps_refrigeration_data *caps_data);
    void (*cmd_setDefrost_usr_cb)(struct caps_refrigeration_data *caps_data);
} caps_refrigeration_data_t;

caps_refrigeration_data_t *caps_refrigeration_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);
