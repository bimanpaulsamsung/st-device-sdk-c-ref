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

#include <string.h>

#include "st_dev.h"
#include "caps_testCapability.h"

#include "freertos/FreeRTOS.h"

static void caps_testCapability_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_testCapability_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
}

caps_testCapability_data_t *caps_testCapability_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_testCapability_data_t *caps_data = NULL;

    caps_data = malloc(sizeof(caps_testCapability_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_testCapability_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_testCapability_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_testCapability.id, caps_testCapability_init_cb, caps_data);
    }
    if (!caps_data->handle) {
        printf("fail to init testCapability handle\n");
    }

    return caps_data;
}
