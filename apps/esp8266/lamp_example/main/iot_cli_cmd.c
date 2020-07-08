/* ***************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iot_uart_cli.h"
#include "device_control.h"

#include "st_dev.h"

#include "iot_debug.h" //private

extern IOT_CTX *ctx;

static int _cli_copy_nth_arg(char* dest, char* src, int size, int n)
{
    int start = 0, end = -1;
    int i = 0, word_index = 0;
    int len;

    for (i = 0; src[i] != '\0'; i++) {
        if ((src[i] == ' ') && (src[i+1]!=' ') && (src[i+1]!='\0')) { //start check
            word_index++;
            if (word_index == n) {
                start = i+1;
            }
        } else if ((src[i] != ' ') && ((src[i+1]==' ')||(src[i+1]=='\0'))) { //end check
            if (word_index == n) {
                end = i;
                break;
            }
        }
    }

    if (end == -1) {
        //printf("Fail to find %dth arg\n", n);
        return -1;
    }

    len = end - start + 1;
    if ( len > size - 1) {
        len = size - 1;
    }

    strncpy(dest, &src[start], len);
    dest[len] = '\0';
    return 0;

}

static void _cli_cmd_cleanup(char *string)
{
    printf("clean-up data with reboot option");
    st_conn_cleanup(ctx, true);
}

extern void button_event(IOT_CAP_HANDLE *handle, int type, int count);
static void _cli_cmd_butten_event(char *string)
{
    char buf[10];
    int count = 1;
    int type = BUTTON_SHORT_PRESS;

    if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
        count = strtol(buf, NULL, 10);
    }
    if (_cli_copy_nth_arg(buf, string, sizeof(buf), 2) >= 0) {
        if (strncmp(buf, "long", 4) == 0) {
            type = BUTTON_LONG_PRESS;
        }
    }

    printf("button_event : count %d, type %d\n", count, type);
    button_event(ctx, type, count);
}

static void _cli_cmd_dummy_cmd(char *string)
{
    char buf[10];
    int i = 0;
    printf("input string : %s\n", string);

    while (true) {
        if (_cli_copy_nth_arg(buf, string, sizeof(buf), i) < 0) {
            break;
        }
        printf("%dth arg : %s\n", i++, buf);
    }
}

static void _cli_cmd_get_log_dump(char *string)
{
    char buf[MAX_UART_LINE_SIZE];
    char* log;
    size_t size = 2048;
    size_t written_size = 0;
    int ret;
    int mode = IOT_DUMP_MODE_NEED_BASE64 | IOT_DUMP_MODE_NEED_DUMP_STATE;

    if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
        size = strtol(buf, NULL, 10);
    }
    ret = iot_dump_create_all_log_dump((struct iot_context *)ctx, &log, size, &written_size, mode);
    if (ret < 0) {
        printf("Fail to get log dump!\n");
        return;
    }
    printf("all_log_dump - size: %d / %d\n", written_size, size);
    printf("%s\n", log);
    free(log);
}


static struct cli_command cmd_list[] = {
    {"cleanup", "clean-up data with reboot option", _cli_cmd_cleanup},
    {"button", "button {count} {type} : ex) button 5 / button 1 long", _cli_cmd_butten_event},
    {"get_log_dump", "[private] get_log_dump [size(default 2048)] ",  _cli_cmd_get_log_dump},
};

void register_iot_cli_cmd(void) {
    for (int i = 0; i < ARRAY_SIZE(cmd_list); i++)
        cli_register_command(&cmd_list[i]);
}