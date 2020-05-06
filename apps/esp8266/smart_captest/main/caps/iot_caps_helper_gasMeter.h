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

#ifndef _IOT_CAPS_HELPER_GAS_METER_
#define _IOT_CAPS_HELPER_GAS_METER_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_GASMETER_GASMETER_UNIT_KWH,
    CAP_ENUM_GASMETER_GASMETER_UNIT_MAX
};

enum {
    CAP_ENUM_GASMETER_GASMETERVOLUME_UNIT_M3,
    CAP_ENUM_GASMETER_GASMETERVOLUME_UNIT_MAX
};

const static struct iot_caps_gasMeter {
    const char *id;
    const struct gasMeter_attr_gasMeterTime {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_gasMeterTime;
    const struct gasMeter_attr_gasMeter {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *units[CAP_ENUM_GASMETER_GASMETER_UNIT_MAX];
        const double min;
    } attr_gasMeter;
    const struct gasMeter_attr_gasMeterCalorific {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const double min;
    } attr_gasMeterCalorific;
    const struct gasMeter_attr_gasMeterVolume {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *units[CAP_ENUM_GASMETER_GASMETERVOLUME_UNIT_MAX];
        const double min;
    } attr_gasMeterVolume;
    const struct gasMeter_attr_gasMeterPrecision {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_gasMeterPrecision;
    const struct gasMeter_attr_gasMeterConversion {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const double min;
    } attr_gasMeterConversion;
} caps_helper_gasMeter = {
    .id = "gasMeter",
    .attr_gasMeterTime = {
        .name = "gasMeterTime",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
    },
    .attr_gasMeter = {
        .name = "gasMeter",
        .property = ATTR_SET_VALUE_MIN | ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_NUMBER,
        .units = {"kWh"},
        .min = 0,
    },
    .attr_gasMeterCalorific = {
        .name = "gasMeterCalorific",
        .property = ATTR_SET_VALUE_MIN | ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_NUMBER,
        .min = 0,
    },
    .attr_gasMeterVolume = {
        .name = "gasMeterVolume",
        .property = ATTR_SET_VALUE_MIN | ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_NUMBER,
        .units = {"m^3"},
        .min = 0,
    },
    .attr_gasMeterPrecision = {
        .name = "gasMeterPrecision",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_OBJECT,
    },
    .attr_gasMeterConversion = {
        .name = "gasMeterConversion",
        .property = ATTR_SET_VALUE_MIN | ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_NUMBER,
        .min = 0,
    },
};

#endif /* _IOT_CAPS_HERLPER_GAS_METER_ */
