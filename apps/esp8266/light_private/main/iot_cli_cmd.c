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

//private
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_libc.h"
#include "esp_heap_trace.h"
#include "esp_system.h"
#include "iot_main.h"
#include "iot_debug.h"


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
    ret = st_create_log_dump(ctx, &log, size, &written_size, mode);
    if (ret < 0) {
        printf("Fail to get log dump!\n");
        return;
    }
    printf("log_dump - size: %d / %d\n", written_size, size);
    printf("%s\n", log);
    free(log);
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

extern int monitor_enable;
static void _cli_cmd_monitor_enable(char *string)
{
    char buf[MAX_UART_LINE_SIZE];

    if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
        monitor_enable = strtol(buf, NULL, 10);
        printf("change monitor mode to %d\n", monitor_enable);
    }
}

extern int monitor_period_ms;
static void _cli_cmd_monitor_period(char *string)
{
    char buf[MAX_UART_LINE_SIZE];

    if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
        monitor_period_ms = strtol(buf, NULL, 10);
        printf("change monitor period to %d ms\n", monitor_period_ms);
    }
}

// PRIVATE

extern heap_region_t g_heap_region[HEAP_REGIONS_MAX];
static void _cli_cmd_heap_info(char *string)
{
    size_t total_size = 0;

    for (int i = 0; i < HEAP_REGIONS_MAX; i++) {
        printf("Region[%d] : %p ~ %p (%u), %08x\n", i,
               g_heap_region[i].start_addr, g_heap_region[i].start_addr + g_heap_region[i].total_size,
               g_heap_region[i].total_size, g_heap_region[i].caps);
        total_size += g_heap_region[i].total_size;
    }

    printf("heap_info : CU:%d, CR:%d, PU:%d, PR:%d\n",
           total_size - heap_caps_get_free_size(MALLOC_CAP_32BIT),
           heap_caps_get_free_size(MALLOC_CAP_32BIT),
           total_size - heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT),
           heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT));
}

static void _cli_cmd_time_info(char *string)
{
    char buf[20];
    struct timeval tv_now;
    struct timeval tv_set = {1577836800, 0}; /* default time is 2020-01-01 00:00:00 UTC */

    if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
        if (strncmp(buf, "get", 3) == 0) {
            gettimeofday(&tv_now, NULL);
            printf("current wall time is %ld\n", tv_now.tv_sec);
        } else if (strncmp(buf, "set", 3) == 0) {
            if (_cli_copy_nth_arg(buf, string, sizeof(buf), 2) >= 0) {
                sscanf(buf, "%ld", &tv_set.tv_sec);

                if (tv_set.tv_sec < 1577836800) {
                    printf("settime target is too old. Need to be greater than 1577836800(2020-01-01 00:00:00 UTC)\n");
                    return;
                }
            }

            /* Set time to tv_set */
            settimeofday(&tv_set, NULL);

            /* Wait 100ms and check current time */
            vTaskDelay(100 / portTICK_RATE_MS);
            gettimeofday(&tv_now, NULL);
            printf("current wall time is %ld\n", tv_now.tv_sec);

        } else {
            printf("Invalid parameter.\n");
            return;
        }
    } else {
        printf("Invalid parameter.\n");
        return;
    }
}

static void _cli_cmd_reboot(char *string)
{
    printf("reboot the system\n");
    esp_restart();
}

static void _cli_cmd_device_info(char *string)
{
    printf("\n");

    const char* buf = esp_get_idf_version();
    printf("ESP_IDF VERSION : %s\n", buf);

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    if (chip_info.model == CHIP_ESP8266) {
        printf("MODEL NAME : ESP8266\n");
    } else if (chip_info.model == CHIP_ESP32) {
        printf("MODEL NAME : ESP32\n");
    }

    /*
     * Todo : Support ESP32 Feature
     * #define CHIP_FEATURE_EMB_FLASH      BIT(0)      //!< Chip has embedded flash memory
     * #define CHIP_FEATURE_WIFI_BGN       BIT(1)      //!< Chip has 2.4GHz WiFi
     * #define CHIP_FEATURE_BLE            BIT(4)      //!< Chip has Bluetooth LE
     * #define CHIP_FEATURE_BT             BIT(5)      //!< Chip has Bluetooth Classic
     *
     */
    if (chip_info.features == CHIP_FEATURE_WIFI_BGN) {
        printf("CHIP FEATURE : WIFI_BGN\n");
    }

    printf("NUMBER OF CPU CORES : %d\n", chip_info.cores);
    printf("REVISION : %d\n", chip_info.revision);

    uint8_t mac_addr[6];
    esp_err_t ret = esp_read_mac(mac_addr, ESP_MAC_WIFI_STA);
    if (ret == ESP_OK) {
        printf("MAC ADDRESS (STATION) : %02x:%02x:%02x:%02x:%02x:%02x\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    }

    ret = esp_read_mac(mac_addr, ESP_MAC_WIFI_SOFTAP);
    if (ret == ESP_OK) {
        printf("MAC ADDRESS (SOFTAP) : %02x:%02x:%02x:%02x:%02x:%02x\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    }

    printf("\n");

    return;
}

#ifdef CONFIG_SAMSUNG_BUILD_ENG
extern int st_publish_event_raw(IOT_CTX *iot_ctx, char *event_payload);
static void _cli_cmd_pub_event(char *string)
{
    char buf[MAX_UART_LINE_SIZE];

    if (ctx == NULL) {
        printf("ctx is NULL\n");
        return;
    }

    if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
        printf("event payload %s\n", buf);
        st_publish_event_raw(ctx, buf);
    }
}
#else
static void _cli_cmd_pub_event(char *string)
{
    printf("pub_event is disabled : enable CONFIG_SAMSUNG_BUILD_ENG\n");
}
#endif
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
    {"get_log_dump", "get_log_dump {size(default 2048)} ",  _cli_cmd_get_log_dump},
    {"button", "button {count} {type} : ex) button 5 / button 1 long", _cli_cmd_butten_event},
    {"monitor_enable", "monitor_enable {0|1}", _cli_cmd_monitor_enable},
    {"monitor_period", "monitor_period {period_ms}", _cli_cmd_monitor_period},
    //private
    {"heap_info", "[private] print heap free size and minimum size", _cli_cmd_heap_info},
    {"time", "[private] time get or time set {since_epoch}", _cli_cmd_time_info},
    {"reboot", "[private] reboot the system", _cli_cmd_reboot},
    {"pub_event", "[private] pub_event [event payload]", _cli_cmd_pub_event},
    {"device_info", "[private] show device info", _cli_cmd_device_info},
};

void register_iot_cli_cmd(void) {
    for (int i = 0; i < ARRAY_SIZE(cmd_list); i++)
        cli_register_command(&cmd_list[i]);
}
