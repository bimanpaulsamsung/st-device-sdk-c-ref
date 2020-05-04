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

#ifndef _IOT_CAPS_HELPER_WINDOW_SHADE_PRESET_
#define _IOT_CAPS_HELPER_WINDOW_SHADE_PRESET_

#include "iot_caps_helper.h"

const static struct iot_caps_windowShadePreset {
    const char *id;
    const struct windowShadePreset_cmd_presetPosition { const char* name; } cmd_presetPosition;
} caps_helper_windowShadePreset = {
    .id = "windowShadePreset",
    .cmd_presetPosition = { .name = "presetPosition" },
};

#endif /* _IOT_CAPS_HERLPER_WINDOW_SHADE_PRESET_ */
