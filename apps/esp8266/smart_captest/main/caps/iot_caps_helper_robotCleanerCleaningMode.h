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

#ifndef _IOT_CAPS_HELPER_ROBOT_CLEANER_CLEANING_MODE_
#define _IOT_CAPS_HELPER_ROBOT_CLEANER_CLEANING_MODE_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_AUTO,
    CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_PART,
    CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_REPEAT,
    CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_MANUAL,
    CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_STOP,
    CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_MAP,
    CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_MAX
};

const static struct iot_caps_robotCleanerCleaningMode {
    const char *id;
    const struct robotCleanerCleaningMode_attr_robotCleanerCleaningMode {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_ROBOTCLEANERCLEANINGMODE_ROBOTCLEANERCLEANINGMODE_VALUE_MAX];
    } attr_robotCleanerCleaningMode;
    const struct robotCleanerCleaningMode_cmd_setRobotCleanerCleaningMode { const char* name; } cmd_setRobotCleanerCleaningMode;
} caps_helper_robotCleanerCleaningMode = {
    .id = "robotCleanerCleaningMode",
    .attr_robotCleanerCleaningMode = {
        .name = "robotCleanerCleaningMode",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"auto", "part", "repeat", "manual", "stop", "map"},
    },
    .cmd_setRobotCleanerCleaningMode = { .name = "setRobotCleanerCleaningMode" }, // arguments: mode(string) 
};

#endif /* _IOT_CAPS_HERLPER_ROBOT_CLEANER_CLEANING_MODE_ */
