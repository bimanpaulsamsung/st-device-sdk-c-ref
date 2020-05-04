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
#include "caps_filterState.h"

#include "freertos/FreeRTOS.h"

static int caps_filterState_get_filterLifeRemaining_value(caps_filterState_data_t *caps_data)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return caps_helper_filterState.attr_filterLifeRemaining.min - 1;
    }
    return caps_data->filterLifeRemaining_value;
}

static void caps_filterState_set_filterLifeRemaining_value(caps_filterState_data_t *caps_data, int value)
{
    if (!caps_data) {
        printf("caps_data is NULL\n");
        return;
    }
    caps_data->filterLifeRemaining_value = value;
}

static void caps_filterState_attr_filterLifeRemaining_send(caps_filterState_data_t *caps_data)
{
    IOT_EVENT *cap_evt;
    uint8_t evt_num = 1;
    int sequence_no;

    if (!caps_data || !caps_data->handle) {
        printf("fail to get handle\n");
        return;
    }

    cap_evt = st_cap_attr_create_int((char *) caps_helper_filterState.attr_filterLifeRemaining.name, caps_data->filterLifeRemaining_value, NULL);
    if (!cap_evt) {
        printf("fail to create cap_evt\n");
        return;
    }

    sequence_no = st_cap_attr_send(caps_data->handle, evt_num, &cap_evt);
    if (sequence_no < 0)
        printf("fail to send filterLifeRemaining value\n");

    printf("Sequence number return : %d\n", sequence_no);
    st_cap_attr_free(cap_evt);
}


static void caps_filterState_init_cb(IOT_CAP_HANDLE *handle, void *usr_data)
{
    caps_filterState_data_t *caps_data = usr_data;
    if (caps_data && caps_data->init_usr_cb)
        caps_data->init_usr_cb(caps_data);
    caps_filterState_attr_filterLifeRemaining_send(caps_data);
}

caps_filterState_data_t *caps_filterState_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data)
{
    caps_filterState_data_t *caps_data = NULL;

    caps_data = malloc(sizeof(caps_filterState_data_t));
    if (!caps_data) {
        printf("fail to malloc for caps_filterState_data\n");
        return NULL;
    }

    memset(caps_data, 0, sizeof(caps_filterState_data_t));

    caps_data->init_usr_cb = init_usr_cb;
    caps_data->usr_data = usr_data;

    caps_data->get_filterLifeRemaining_value = caps_filterState_get_filterLifeRemaining_value;
    caps_data->set_filterLifeRemaining_value = caps_filterState_set_filterLifeRemaining_value;
    caps_data->attr_filterLifeRemaining_send = caps_filterState_attr_filterLifeRemaining_send;
    caps_data->filterLifeRemaining_value = 0;
    if (ctx) {
        caps_data->handle = st_cap_handle_init(ctx, component, caps_helper_filterState.id, caps_filterState_init_cb, caps_data);
    }
    if (!caps_data->handle) {
        printf("fail to init filterState handle\n");
    }

    return caps_data;
}
