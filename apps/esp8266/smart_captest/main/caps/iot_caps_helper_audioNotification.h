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

#ifndef _IOT_CAPS_HELPER_AUDIO_NOTIFICATION_
#define _IOT_CAPS_HELPER_AUDIO_NOTIFICATION_

#include "iot_caps_helper.h"

const static struct iot_caps_audioNotification {
    const char *id;
    const struct audioNotification_cmd_playTrack { const char* name; } cmd_playTrack;
    const struct audioNotification_cmd_playTrackAndRestore { const char* name; } cmd_playTrackAndRestore;
    const struct audioNotification_cmd_playTrackAndResume { const char* name; } cmd_playTrackAndResume;
} caps_helper_audioNotification = {
    .id = "audioNotification",
    .cmd_playTrack = { .name = "playTrack" }, // arguments: uri(string) level(integer) 
    .cmd_playTrackAndRestore = { .name = "playTrackAndRestore" }, // arguments: uri(string) level(integer) 
    .cmd_playTrackAndResume = { .name = "playTrackAndResume" }, // arguments: uri(string) level(integer) 
};

#endif /* _IOT_CAPS_HERLPER_AUDIO_NOTIFICATION_ */
