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

#ifndef _IOT_CAPS_HELPER_WASHER_OPERATING_STATE_
#define _IOT_CAPS_HELPER_WASHER_OPERATING_STATE_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_PAUSE,
    CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_RUN,
    CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_STOP,
    CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_MAX
};

enum {
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_AIRWASH,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_COOLING,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_DELAYWASH,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_DRYING,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_FINISH,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_NONE,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_PREWASH,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_RINSE,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_SPIN,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_WASH,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_WEIGHTSENSING,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_WRINKLEPREVENT,
    CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_MAX
};

const static struct iot_caps_washerOperatingState {
    const char *id;
    const struct washerOperatingState_attr_machineState {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_WASHEROPERATINGSTATE_MACHINESTATE_VALUE_MAX];
    } attr_machineState;
    const struct washerOperatingState_attr_completionTime {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_completionTime;
    const struct washerOperatingState_attr_supportedMachineStates {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_supportedMachineStates;
    const struct washerOperatingState_attr_washerJobState {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_WASHEROPERATINGSTATE_WASHERJOBSTATE_VALUE_MAX];
    } attr_washerJobState;
    const struct washerOperatingState_cmd_setMachineState { const char* name; } cmd_setMachineState;
} caps_helper_washerOperatingState = {
    .id = "washerOperatingState",
    .attr_machineState = {
        .name = "machineState",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"pause", "run", "stop"},
    },
    .attr_completionTime = {
        .name = "completionTime",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
    },
    .attr_supportedMachineStates = {
        .name = "supportedMachineStates",
        .property = ATTR_SET_VALUE_ARRAY,
        .value_type = VALUE_TYPE_STRING,
    },
    .attr_washerJobState = {
        .name = "washerJobState",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"airWash", "cooling", "delayWash", "drying", "finish", "none", "preWash", "rinse", "spin", "wash", "weightSensing", "wrinklePrevent"},
    },
    .cmd_setMachineState = { .name = "setMachineState" }, // arguments: state(string) 
};

#endif /* _IOT_CAPS_HERLPER_WASHER_OPERATING_STATE_ */
