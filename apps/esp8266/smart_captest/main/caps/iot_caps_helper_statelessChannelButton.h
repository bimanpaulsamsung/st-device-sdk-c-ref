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

#ifndef _IOT_CAPS_HELPER_STATELESS_CHANNEL_BUTTON_
#define _IOT_CAPS_HELPER_STATELESS_CHANNEL_BUTTON_

#include "iot_caps_helper.h"

enum {
    CAP_ENUM_STATELESSCHANNELBUTTON_AVAILABLECHANNELBUTTONS_VALUE_CHANNELUP,
    CAP_ENUM_STATELESSCHANNELBUTTON_AVAILABLECHANNELBUTTONS_VALUE_CHANNELDOWN,
    CAP_ENUM_STATELESSCHANNELBUTTON_AVAILABLECHANNELBUTTONS_VALUE_MAX
};

const static struct iot_caps_statelessChannelButton {
    const char *id;
    const struct statelessChannelButton_attr_availableChannelButtons {
        const char *name;
        const unsigned char property;
        const unsigned char value_type;
        const char *values[CAP_ENUM_STATELESSCHANNELBUTTON_AVAILABLECHANNELBUTTONS_VALUE_MAX];
    } attr_availableChannelButtons;
    const struct statelessChannelButton_cmd_setButton { const char* name; } cmd_setButton;
} caps_helper_statelessChannelButton = {
    .id = "statelessChannelButton",
    .attr_availableChannelButtons = {
        .name = "availableChannelButtons",
        .property = ATTR_SET_VALUE_ARRAY,
        .value_type = VALUE_TYPE_STRING,
        .values = {"channelUp", "channelDown"},
    },
    .cmd_setButton = { .name = "setButton" }, // arguments: button(string) 
};

#endif /* _IOT_CAPS_HERLPER_STATELESS_CHANNEL_BUTTON_ */
