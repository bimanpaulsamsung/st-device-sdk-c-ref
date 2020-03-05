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
#include <reent.h>
#include <sys/time.h>
#include "iot_debug.h"
#include "iot_uart_cli.h"
#include "log_service.h"
#include "iot_util.h"
#include "platform_stdlib.h"
#include "build_info.h"
#include "wifi_conf.h"
#include "iot_bsp_nv_data.h"
#include "iot_cli_cmd.h"

/******************for wifi api test***************************************/
void iot_api_wifi_tc_init(iot_wifi_mode_t mode)
{
	iot_bsp_wifi_init();
}

void iot_api_wifi_tc_mode_change(iot_wifi_mode_t mode, char *ssid, char *passwd)
{
    IOT_INFO("iot_wifi_test:auth mode %d ssid %s pass=%s", mode, ssid, passwd);
    iot_wifi_conf config;
	config.mode = mode;
	strcpy(config.ssid, ssid);
	strcpy(config.pass, passwd);
	IOT_INFO("iot_wifi_test:auth mode %d ssid %s pass=%s", config.mode, config.ssid, config.pass);
	iot_bsp_wifi_set_mode(&config);
}

void iot_api_wifi_tc_mode_change_open(iot_wifi_mode_t mode, char *ssid)
{
    iot_wifi_conf config;
	config.mode = mode;
	strcpy(config.ssid, ssid);

	IOT_INFO("iot_wifi_test:auth mode %d ssid %s pass=%s", config.mode, config.ssid, "none");
	iot_bsp_wifi_set_mode(&config);
}

void iot_api_wifi_tc_mode_sta(char *ssid, char *passwd)
{
	iot_api_wifi_tc_mode_change(IOT_WIFI_MODE_STATION, ssid, passwd);
}

void iot_api_wifi_tc_mode_sta_open(char *ssid)
{
	iot_api_wifi_tc_mode_change_open(IOT_WIFI_MODE_STATION, ssid);
}

void iot_api_wifi_tc_mode_ap(char *ssid, char *passwd)
{
	iot_api_wifi_tc_mode_change(IOT_WIFI_MODE_SOFTAP, ssid, passwd);

}

void iot_api_wifi_tc_mode_ap_open(char *ssid)
{
	iot_api_wifi_tc_mode_change_open(IOT_WIFI_MODE_SOFTAP, ssid);
}

void iot_api_wifi_tc_mode_off()
{
	iot_api_wifi_tc_mode_change(IOT_WIFI_MODE_OFF, "default", "default");
}

void iot_api_wifi_tc_mode_scan()
{

	iot_wifi_scan_result_t *scan_result = (iot_wifi_scan_result_t *) malloc(IOT_WIFI_MAX_SCAN_RESULT * sizeof(iot_wifi_scan_result_t));
	memset(scan_result, 0x0, (IOT_WIFI_MAX_SCAN_RESULT * sizeof(iot_wifi_scan_result_t)));

	int len = iot_bsp_wifi_get_scan_result(scan_result);
	IOT_DEBUG("station number: %d \n", len);

}

void iot_api_wifi_tc_info()
{

        int ret;
        struct iot_mac mac;

        printf("\n");

        ret = iot_bsp_wifi_get_mac(&mac);
        if (ret == RTW_SUCCESS) {
                printf("MAC ADDRESS (STATION) : %02x:%02x:%02x:%02x:%02x:%02x\n", mac.addr[0], mac.addr[1], mac.addr[2], mac.addr[3], mac.addr[4], mac.addr[5]);
        }

        printf("\n");
}

void iot_api_wifi_set_mac(char mac[12])
{
	wifi_set_mac_address(mac);
}

void iot_api_wifi_get_mac()
{
        int ret;
        struct iot_mac mac;

        printf("\n");

        ret = iot_bsp_wifi_get_mac(&mac);
        if (ret == RTW_SUCCESS) {
                printf("MAC ADDRESS (STATION) : %02x:%02x:%02x:%02x:%02x:%02x\n", mac.addr[0], mac.addr[1], mac.addr[2], mac.addr[3], mac.addr[4], mac.addr[5]);
        }

        printf("\n");

}

void iot_cli_cmd_wifi_test(char *string)
{
	int wifi_tc_type = 0;

	char dest[16] = {0};
	iot_cli_copy_nth_arg(dest, string, strlen(string), 1);

	/*init wifi before test*/
	iot_bsp_wifi_init();

	if (strncmp (dest, "setmac", strlen("setmac")) == 0) {
		char mac[12] = {0};
		iot_cli_copy_nth_arg(mac, string, strlen(string), 2);
		iot_api_wifi_set_mac(mac);
	}
	else if (strncmp (dest, "info", strlen("info")) == 0) {
		iot_api_wifi_tc_info();
	}
	else if (strncmp (dest, "init", strlen("init")) == 0) {
		iot_api_wifi_tc_init(IOT_WIFI_MODE_STATION);
	}
	else if (strcmp (dest, "sta") == 0) {
	    char ssid[32 + 1] = {0};
	    char passwd[64 + 1] = {0};
	    iot_cli_copy_nth_arg(ssid, string, strlen(string), 2);
	    iot_cli_copy_nth_arg(passwd, string, strlen(string), 3);
	    printf("iot_wifi_test:auth mode %d ssid %s pass=%s \r\n", IOT_WIFI_MODE_STATION, ssid, passwd);
	    if(strlen(ssid) == 0)
	        goto print_help;
	    if(strlen(passwd) == 0)
	        goto print_help;
	    printf("iot_wifi_test:auth mode %d ssid %s pass=%s\r\n", IOT_WIFI_MODE_STATION, ssid, passwd);
		iot_api_wifi_tc_mode_change(IOT_WIFI_MODE_STATION, ssid, passwd);
	}
	else if (strcmp (dest, "ap") == 0) {
	    char ssid[32 + 1] = {0};
	    char passwd[64 + 1] = {0};
		iot_cli_copy_nth_arg(ssid, string, strlen(string), 2);
	    iot_cli_copy_nth_arg(passwd, string, strlen(string), 3);
	    if(strlen(ssid) == 0)
	        goto print_help;
	    if(strlen(passwd) == 0)
	        goto print_help;
		iot_api_wifi_tc_mode_change(IOT_WIFI_MODE_SOFTAP, ssid, passwd);
	}
	else if (strcmp (dest, "scan") == 0) {
		iot_api_wifi_tc_mode_scan();
	}
	else if (strcmp (dest, "mode") == 0) {
		//iot_api_wifi_test(1);
	}
	else if (strcmp (dest, "opensta") == 0) {
	    char ssid[32 + 1] = {0};
		iot_cli_copy_nth_arg(ssid, string, strlen(string), 2);
	   if(strlen(ssid) == 0)
	        goto print_help;
		iot_api_wifi_tc_mode_change_open(IOT_WIFI_MODE_STATION, ssid);
	}
	else if (strcmp (dest, "openap") == 0) {
	    char ssid[32 + 1] = {0};
		iot_cli_copy_nth_arg(ssid, string, strlen(string), 2);
	    if(strlen(ssid) == 0)
	        goto print_help;
		iot_api_wifi_tc_mode_change_open(IOT_WIFI_MODE_SOFTAP, ssid);
	}
	else {
print_help:
		printf("wifi_test info : display the info of wifi\n");
		printf("wifi_test setmac aabbccddeeff : set the mac aabbccddeeff to wlan, you should replace aabbccddeeff with your mac\n");
		printf("wifi_test init : bootup the wlan\n");
		printf("wifi_test sta $ssid $passwd : the wlan change to sta mode\n");
		printf("wifi_test ap $ssid $passwd: the wlan change to ap mode\n");
		printf("wifi_test scan : the wlan change to scan mode\n");
		printf("wifi_test mode : the wlan mode change from: sta->ap->scan->off\n");
		printf("wifi_test opensta : the wlan mode change to sta, and open security\n");
		printf("wifi_test openap : the wlan mode change to ap, and open security\n");
		printf("wifi_test help : cmd help\n");
	}
}



