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

#ifndef _IOT_CAPS_HELPER_VIDEO_CAPTURE_
#define _IOT_CAPS_HELPER_VIDEO_CAPTURE_

#include "iot_caps_helper.h"

const static struct iot_caps_videoCapture {
    const char *id;
    const struct videoCapture_attr_clip {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_clip;
    const struct videoCapture_attr_stream {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_stream;
    const struct videoCapture_cmd_capture { const char* name; } cmd_capture;
} caps_helper_videoCapture = {
    .id = "videoCapture",
    .attr_clip = {
        .name = "clip",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_OBJECT,
    },
    .attr_stream = {
        .name = "stream",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_OBJECT,
    },
    .cmd_capture = { .name = "capture" }, // arguments: startTime(string) captureTime(string) endTime(string) correlationId(string) reason(string) 
};

#endif /* _IOT_CAPS_HERLPER_VIDEO_CAPTURE_ */
