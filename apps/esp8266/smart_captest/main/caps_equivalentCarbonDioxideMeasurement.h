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

#include "caps/iot_caps_helper_equivalentCarbonDioxideMeasurement.h"

typedef struct caps_equivalentCarbonDioxideMeasurement_data {
    IOT_CAP_HANDLE* handle;
    void *usr_data;
    void *cmd_data;

    double equivalentCarbonDioxideMeasurement_value;
    char *equivalentCarbonDioxideMeasurement_unit;

    double (*get_equivalentCarbonDioxideMeasurement_value)(struct caps_equivalentCarbonDioxideMeasurement_data *caps_data);
    void (*set_equivalentCarbonDioxideMeasurement_value)(struct caps_equivalentCarbonDioxideMeasurement_data *caps_data, double value);
    const char *(*get_equivalentCarbonDioxideMeasurement_unit)(struct caps_equivalentCarbonDioxideMeasurement_data *caps_data);
    void (*set_equivalentCarbonDioxideMeasurement_unit)(struct caps_equivalentCarbonDioxideMeasurement_data *caps_data, const char *unit);
    void (*attr_equivalentCarbonDioxideMeasurement_send)(struct caps_equivalentCarbonDioxideMeasurement_data *caps_data);

    void (*init_usr_cb)(struct caps_equivalentCarbonDioxideMeasurement_data *caps_data);
} caps_equivalentCarbonDioxideMeasurement_data_t;

caps_equivalentCarbonDioxideMeasurement_data_t *caps_equivalentCarbonDioxideMeasurement_initialize(IOT_CTX *ctx, const char *component, void *init_usr_cb, void *usr_data);