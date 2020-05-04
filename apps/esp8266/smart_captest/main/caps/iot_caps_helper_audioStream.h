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

#ifndef _IOT_CAPS_HELPER_AUDIO_STREAM_
#define _IOT_CAPS_HELPER_AUDIO_STREAM_

#include "iot_caps_helper.h"

const static struct iot_caps_audioStream {
    const char *id;
    const struct audioStream_attr_uri {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
    } attr_uri;
    const struct audioStream_cmd_startAudio { const char* name; } cmd_startAudio;
    const struct audioStream_cmd_stopAudio { const char* name; } cmd_stopAudio;
} caps_helper_audioStream = {
    .id = "audioStream",
    .attr_uri = {
        .name = "uri",
        .property = NULL,
        .value_type = VALUE_TYPE_STRING,
    },
    .cmd_startAudio = { .name = "startAudio" },
    .cmd_stopAudio = { .name = "stopAudio" },
};

#endif /* _IOT_CAPS_HERLPER_AUDIO_STREAM_ */
