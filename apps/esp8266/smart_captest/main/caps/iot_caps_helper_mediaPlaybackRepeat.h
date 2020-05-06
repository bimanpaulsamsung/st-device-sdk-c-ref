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

#ifndef _IOT_CAPS_HELPER_MEDIA_PLAYBACK_REPEAT_
#define _IOT_CAPS_HELPER_MEDIA_PLAYBACK_REPEAT_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_MEDIAPLAYBACKREPEAT_PLAYBACKREPEATMODE_VALUE_ALL,
    CAP_ENUM_MEDIAPLAYBACKREPEAT_PLAYBACKREPEATMODE_VALUE_OFF,
    CAP_ENUM_MEDIAPLAYBACKREPEAT_PLAYBACKREPEATMODE_VALUE_ONE,
    CAP_ENUM_MEDIAPLAYBACKREPEAT_PLAYBACKREPEATMODE_VALUE_MAX
};

const static struct iot_caps_mediaPlaybackRepeat {
    const char *id;
    const struct mediaPlaybackRepeat_attr_playbackRepeatMode {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_MEDIAPLAYBACKREPEAT_PLAYBACKREPEATMODE_VALUE_MAX];
    } attr_playbackRepeatMode;
    const struct mediaPlaybackRepeat_cmd_setPlaybackRepeatMode { const char* name; } cmd_setPlaybackRepeatMode;
} caps_helper_mediaPlaybackRepeat = {
    .id = "mediaPlaybackRepeat",
    .attr_playbackRepeatMode = {
        .name = "playbackRepeatMode",
        .property = ATTR_SET_VALUE_REQUIRED,
        .value_type = VALUE_TYPE_STRING,
        .values = {"all", "off", "one"},
    },
    .cmd_setPlaybackRepeatMode = { .name = "setPlaybackRepeatMode" }, // arguments: mode(string) 
};

#endif /* _IOT_CAPS_HERLPER_MEDIA_PLAYBACK_REPEAT_ */
