/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include "FreeRTOS.h"
#include "syslog.h"
#include "st_util.h"
#ifdef MTK_NVDM_ENABLE
#include "nvdm.h"
#endif
#include "wifi_api.h"
#include "hal_flash.h"
#include "task.h"
#include "iot_bsp_wifi.h"
#include "wifi_nvdm_config.h"
#include "iot_bsp_nv_data.h"
#include "hal_gpio.h"
#include "hal_platform.h"
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
		printf("Fail to find %dth arg\n", n);
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
static void st_main_cmd_read_mac(const char *cmd)
{
  uint8_t sta_mac[6] = {0};
  uint8_t ap_mac[6]  = {0};

  wifi_config_get_mac_address(WIFI_PORT_STA, sta_mac);
  wifi_config_get_mac_address(WIFI_PORT_AP, ap_mac);

  printf("Station Mode: %02X:%02X:%02X:%02X:%02X:%02X\n",
      sta_mac[0], sta_mac[1], sta_mac[2], sta_mac[3], sta_mac[4], sta_mac[5]);

  printf("Soft AP Mode: %02X:%02X:%02X:%02X:%02X:%02X\n",
      ap_mac[0], ap_mac[1], ap_mac[2], ap_mac[3], ap_mac[4], ap_mac[5]);
}

#ifdef MTK_NVDM_ENABLE
static void st_main_cmd_write_mac(const char *cmd)
{
  uint8_t ap_mac[18]  = {0,};
  uint8_t sta_mac[18] = {0,};
  uint8_t input[32]   = {0,};
  uint8_t output[64]  = {0,};
  uint8_t confirm[16] = {0,};

  while (1) {
    log_write(" STA MAC: ", 10);
    memset(input, 0, sizeof(input));
    if (iot_util_gets(input) > 0) {
      snprintf(output, sizeof(output) - 1, "\nSTA MAC: \"%s\" [Y/n]? ", input);
      log_write(output, strlen(output));
      if (iot_util_gets(confirm) == 0 || !strcasecmp(confirm, "y"))
        break;
    }
  }
  strncpy(sta_mac, input, strlen(input));

  if (nvdm_write_data_item("STA", "MacAddr",
      NVDM_DATA_ITEM_TYPE_STRING, sta_mac, 17) != NVDM_STATUS_OK) {
    printf("Failed to write mac\n");
  }

  while (1) {
    log_write(" AP MAC: ", 9);
    memset(input, 0, sizeof(input));
    if (iot_util_gets(input) > 0) {
      snprintf(output, sizeof(output) - 1, "\nAP MAC: \"%s\" [Y/n]? ", input);
      log_write(output, strlen(output));
      if (iot_util_gets(confirm) == 0 || !strcasecmp(confirm, "y"))
        break;
    }
  }
  strncpy(ap_mac, input, strlen(input));

  if (nvdm_write_data_item("AP", "MacAddr",
      NVDM_DATA_ITEM_TYPE_STRING, ap_mac, 17) != NVDM_STATUS_OK) {
    printf("Failed to write ap mac\n");
  }
}
#endif

void iot_util_cmd_help(const char *cmd);

extern uint16_t iot_bsp_wifi_get_scan_result(iot_wifi_scan_result_t *scan_result);

static void run_wifi_scan(const char *cmd)
{
	iot_wifi_scan_result_t *scan_result = NULL;
	scan_result = (iot_wifi_scan_result_t *) malloc(sizeof(iot_wifi_scan_result_t));
	iot_bsp_wifi_get_scan_result(scan_result);
}

static void run_wifi_sta(const char *cmd)
{
	uint8_t prv_op_mode = 0;
	uint8_t target_mode;
	uint8_t mismatch = 0;
	int32_t ret = 0;
	int new_conf;

	ret = wifi_config_get_opmode(&prv_op_mode);
	if (ret < 0) {
		printf("MT7682 can't get curr WIFI mode\n");
	}

	if (prv_op_mode == WIFI_MODE_STA_ONLY)
		mismatch = 1;

	new_conf = wlan_check_ssid_psk(WIFI_PORT_STA,
					"mt7682", "12345678", 0);

	target_mode = WIFI_MODE_STA_ONLY;
	printf("MT7682's C/T WIFI mode:%d/%d, new_conf(%d)\n",
					prv_op_mode, target_mode, new_conf);

	if (mismatch || (new_conf != 0)) {
		printf("WIFI mode Stop & RADIO off\n");
		if (prv_op_mode == WIFI_MODE_AP_ONLY)
			ret = wlan_ap_stop();
		else if (prv_op_mode == WIFI_MODE_STA_ONLY)
			ret = wlan_sta_stop();
		else
			printf("Undefined WIFI prv_op_mode(%d)!!\n", prv_op_mode);

		if (ret != 0)
			printf("Failed STOP wifi mode(%d)!!\n",prv_op_mode);
	} else {
		wifi_config_set_radio(1);
		printf("same WIFI mode, just RADIO on\n");
		return;
	}
	int chan = 0;
	if (target_mode == WIFI_MODE_STA_ONLY) {
		ret = wlan_sta_start("mt7682", "12345678", chan);
		if (ret < 0)
			printf("MT7682 can't set new WIFI mode:%d\n", target_mode);

	} else {
		printf("Undefined WIFI target_mode(%d)!!\n",target_mode);
		ret = -1;
	}
}

static void run_wifi_ap(const char *cmd)
{
	uint8_t prv_op_mode = 0;
	uint8_t target_mode;
	uint8_t mismatch = 0;
	int32_t ret = 0;
	int new_conf;

	ret = wifi_config_get_opmode(&prv_op_mode);
	if (ret < 0) {
		printf("MT7682 can't get curr WIFI mode\n");
	}

	if (prv_op_mode == WIFI_MODE_AP_ONLY)
		mismatch = 1;

	new_conf = wlan_check_ssid_psk(WIFI_PORT_AP,
					"mt7682", "12345678", 0);

	target_mode = WIFI_MODE_AP_ONLY;
	printf("MT7682's C/T WIFI mode:%d/%d, new_conf(%d)\n",
					prv_op_mode, target_mode, new_conf);

	if (mismatch || (new_conf != 0)) {
		printf("WIFI mode Stop & RADIO off\n");
		if (prv_op_mode == WIFI_MODE_AP_ONLY)
			ret = wlan_ap_stop();
		else if (prv_op_mode == WIFI_MODE_STA_ONLY)
			ret = wlan_sta_stop();
		else
			printf("Undefined WIFI prv_op_mode(%d)!!\n", prv_op_mode);

		if (ret != 0)
			printf("Failed STOP wifi mode(%d)!!\n",prv_op_mode);
	} else {
		wifi_config_set_radio(1);
		printf("same WIFI mode, just RADIO on\n");
		return;
	}
	int chan = 6;
	if (target_mode == WIFI_MODE_AP_ONLY) {
		ret = wlan_ap_start("mt7682", "12345678", chan);
		if (ret < 0)
			printf("MT7682 can't set new WIFI mode:%d\n", target_mode);
	} else {
		printf("Undefined WIFI target_mode(%d)!!\n",target_mode);
		ret = -1;
	}
}

static void cmd_erase_prov(char *string)
{
	iot_error_t err;

	printf("%s: erase provisioning data...\n", __func__);

	if ((err = iot_nv_erase_prov_data()) != IOT_ERROR_NONE)
		printf("%s: failed to erase provisioning data: %d\n", __func__, err);

	if ((err = iot_nv_erase(IOT_NVD_DEVICE_ID)) != IOT_ERROR_NONE) {
		if (err == IOT_ERROR_NV_DATA_NOT_EXIST) {
			printf("%s: provisioning data does not exist: %d\n", __func__, err);
		} else {
			printf("%s: failed to erase provisioning data: %d\n", __func__, err);
		}
	}

	printf("%s: done\n", __func__);
	printf("Please reset device...\n");
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	device_reset();
}

static void reboot_device(char *string)
{
	printf("Please reset device...\n");
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	device_reset();
}

#define NR_LED 24
#define BSP_LED_MAX 24
hal_gpio_pin_t test_led_gpio_number[BSP_LED_MAX] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22};
/**< The data of this array means pin number corresponding to LED, it is decided by configuration of the board */
#define PINMUX_GPIO_FUNCTION_INDEX	0	/**< This value is decided by configuration of the board */

#define BSP_LED_STATE_ON	HAL_GPIO_DATA_HIGH
#define BSP_LED_STATE_OFF	HAL_GPIO_DATA_LOW
static void run_led_init(char *string)
{
	int led_number = 0;
	hal_gpio_status_t ret_gpio;
	hal_pinmux_status_t ret_pinmux;
	char buf[8] = {0};

	_cli_copy_nth_arg(buf, string, sizeof(string), 1);
	sscanf(buf, "%d", &led_number);

	printf("%d led init...\n",led_number);

	/** initialzie selected LED **/
	ret_gpio = hal_gpio_init(test_led_gpio_number[led_number]);
		if (HAL_GPIO_STATUS_OK != ret_gpio) {
		printf("init the %d gpio:%d failed\n",led_number, test_led_gpio_number[led_number]);
		return;
		}

		ret_pinmux = hal_pinmux_set_function(test_led_gpio_number[led_number], PINMUX_GPIO_FUNCTION_INDEX);
		if (HAL_PINMUX_STATUS_OK != ret_pinmux) {
		printf("set the %d gpio:%d function failed\n",led_number, test_led_gpio_number[led_number]);
		return;
		}

		ret_gpio = hal_gpio_set_direction(test_led_gpio_number[led_number], HAL_GPIO_DIRECTION_OUTPUT);
		if (HAL_GPIO_STATUS_OK != ret_gpio) {
		printf("set the %d gpio:%d direction failed\n",led_number, test_led_gpio_number[led_number]);
		return;
		}
		ret_gpio = hal_gpio_set_output(test_led_gpio_number[led_number], BSP_LED_STATE_ON);
		if (HAL_GPIO_STATUS_OK != ret_gpio) {
		printf("set the %d gpio:%d output failed\n",led_number, test_led_gpio_number[led_number]);
		return;
		}
}

static void run_led_on(char *string)
/** turn on the selected LED **/
{
	int led_number = 0;
	hal_gpio_status_t ret_gpio;
	hal_pinmux_status_t ret_pinmux;
	char buf[8] = {0};

	_cli_copy_nth_arg(buf, string, sizeof(string), 1);
	sscanf(buf, "%d", &led_number);

	printf("%d led on...\n",led_number);

	ret_gpio = hal_gpio_set_output(test_led_gpio_number[led_number], BSP_LED_STATE_ON);
	if (HAL_GPIO_STATUS_OK != ret_gpio) {
	printf("set the %d gpio:%d output failed\n",led_number, test_led_gpio_number[led_number]);
	return;
	}
}

static void run_led_off(char *string)
{
	int led_number = 0;
	hal_gpio_status_t ret_gpio;
	hal_pinmux_status_t ret_pinmux;
	char buf[8] = {0};

	_cli_copy_nth_arg(buf, string, sizeof(string), 1);
	sscanf(buf, "%d", &led_number);

	printf("%d led off...\n",led_number);

	ret_gpio = hal_gpio_set_output(test_led_gpio_number[led_number], BSP_LED_STATE_OFF);
	if (HAL_GPIO_STATUS_OK != ret_gpio) {
		printf("set the %d gpio:%d output failed\n",led_number, test_led_gpio_number[led_number]);
		return;
	}
}

static struct iot_util_user_cmd usr_cmd[] = {
  {"help",            "Help",                             iot_util_cmd_help},
  {"read_mac",        "read mac addr",                    st_main_cmd_read_mac},
#ifdef MTK_NVDM_ENABLE
  {"write_mac",       "write mac addr",                   st_main_cmd_write_mac},
  {"scan",			  "run wifi scan mode",				  run_wifi_scan},
  {"sta",			 "run wifi sta mode",				  run_wifi_sta},
  {"ap",				 "run wifi ap mode",					  run_wifi_ap},
  {"ledinit",				 "led init test",					  run_led_init},
  {"ledon",				 "led on test",					  run_led_on},
  {"ledoff",				 "led off test",					  run_led_off},
#endif
  {"erase_prov",      "erase prov",                       cmd_erase_prov},
  {"reboot",      "reboot device",                       reboot_device},
  {"exit",            "Leave from shell",                 NULL},
};

void iot_util_cmd_help(const char *cmd)
{
  int i;

  printf("<command>\n");

  for (i = 0; i < ARRAY_SIZE(usr_cmd); i++)
    printf(" %-15s : %s\n", usr_cmd[i].cmd, usr_cmd[i].desc);
}

void app_shell_task(void)
{
  iot_util_run_shell(usr_cmd, ARRAY_SIZE(usr_cmd));
}

void app_run_shell(void)
{
  xTaskCreate(app_shell_task, "app_shell_task", 2048, NULL, 5, NULL);
  iot_util_wait_for_user_input(2000);
}
