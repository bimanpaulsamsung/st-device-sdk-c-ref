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
#include <sys/time.h>
#include <nvs_flash.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#include "esp_libc.h"
//#include "esp_heap_trace.h"
#include "esp_system.h"
//#include "priv/esp_spi_flash_raw.h"
#include "iot_uart_cli.h"
#include "iot_debug.h"
#include "iot_bsp_wifi.h"
#include "iot_main.h"
#include "iot_nv_data.h"
#include "iot_util.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "device_control.h"
#define UART_BUF_SIZE (20)
extern int g_StopMainTask;

static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

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
		IOT_DEBUG("Fail to find %dth arg", n);
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
#if SAMSUNG //Todo : Check!
extern heap_region_t g_heap_region[HEAP_REGIONS_MAX];
extern void esp_task_wdt_reset(void);

static void _cli_heap_dump(uint32_t start, uint32_t size)
{
	uint32_t now;
	uint32_t real_start = start - start % 4;

	esp_task_wdt_reset();
	for (now = real_start; now < start + size; now+=4) {
		if (((now - real_start) % 32) == 0) {
			printf("\n%p :", (uint32_t *)now);
		}
		printf(" %08x", ((uint32_t *)now)[0]);
	}
	printf("\n");
}

static void _cli_cmd_heap_dump(char *string)
{
	char buf[15];
	uint32_t start = 0, size = 0;

	if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
		if (strncmp(buf, "all", 4) == 0) {
			for (int i = 0; i < HEAP_REGIONS_MAX; i++) {
				_cli_heap_dump((uint32_t)g_heap_region[i].start_addr, g_heap_region[i].total_size);
			}
			return;
		}
		start = strtoul(buf, NULL, 16);
	}

	if (_cli_copy_nth_arg(buf, string, sizeof(buf), 2) >= 0) {
		size = strtoul(buf, NULL, 16);
	}

	if ( (start > 0) && (size > 0) ) {
		_cli_heap_dump(start, size);
		return;
	}

	printf("wrong arg, check help\n");

}

static void _cli_cmd_heap_trace(char *string)
{
	char buf[8];

	if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
		if (strncmp(buf, "start", 6) == 0) {
			heap_trace_start(HEAP_TRACE_LEAKS);
		} else if (strncmp(buf, "stop", 5) == 0) {
			heap_trace_stop();
		} else if (strncmp(buf, "resume", 7) == 0) {
			heap_trace_resume();
		} else if (strncmp(buf, "dump", 5) == 0) {
			heap_trace_dump();
		} else {
			printf("wrong arg, check help\n");
			return;
		}

		printf("heap_trace_%s is done\n", buf);
	} else {
		printf("wrong arg, check help\n");
	}

}

static void _cli_cmd_heap_info(char *string)
{
	size_t total_size = 0;

	for (int i = 0; i <	HEAP_REGIONS_MAX; i++) {
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
#endif
static void _cli_cmd_memory_set(char *string)
{
	char buf[12];
	uint32_t address,value;


	 if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) < 0)
		 goto ERROR_OUT;

	 address = strtoul(buf, NULL, 16);
	 if (address % 4 != 0)
		 goto ERROR_OUT;

	 if (_cli_copy_nth_arg(buf, string, sizeof(buf), 2) < 0)
		 goto ERROR_OUT;
	 value = strtoul(buf, NULL, 16);

	 *(uint32_t *)address = value;
	 return;

ERROR_OUT:
	 printf("wrong arg, check help\n");
	 return;
}

static void _cli_cmd_task_info(char *string)
{
	const size_t bytes_per_task = 40; /* see vTaskList description */
	char *task_list_buffer = malloc(uxTaskGetNumberOfTasks() * bytes_per_task);
	if (task_list_buffer == NULL) {
		printf("failed to allocate buffer for vTaskList output");
		return;
	}
	/* cStatus:
		x: taskRUNNING_CHAR
		B: tskBLOCKED_CHAR
		R: tskREADY_CHAR
		D: tskDELETED_CHAR
		S: tskSUSPENDED_CHAR
	*/
	/*cStatus, uxCurrentPriority, usStackHighWaterMark, xTaskNumber */
	printf("Task Name\tStatus\tPrio\tHWM\tTask#\n");
	vTaskList(task_list_buffer);
	printf("%s\n", task_list_buffer);
	free(task_list_buffer);

	return;

}

static void _cli_cmd_gpio_info(char *string)
{
	/* For get, value is valid only if pad is configured for input or output. Otherwise it will return 0
	     For set, first set direction by using out/in parameter. For out, set 1(high) or 0(low) one more time */

	int i;
	char buf[8];
	unsigned int pin_num = 0xff;
	unsigned int value = 0xff;

	/* 1st parameter is get/set */
	if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
		if (strncmp(buf, "get", 3) == 0) {

			/* 2nd parameter is all/{pin num} for get */
			if (_cli_copy_nth_arg(buf, string, sizeof(buf), 2) >= 0) {
				if (strncmp(buf, "all", 3) == 0) {
					for (i = 0 ; i < GPIO_PIN_COUNT; i++) {
						printf("GPIO %d: %d\n", i, gpio_get_level(i));
					}
				} else {
					sscanf(buf, "%d", &pin_num);
					if (pin_num >= GPIO_PIN_COUNT) {
						printf("Invalid parameter. wrong pin num\n");
						return;
					}
					printf("GPIO %d: %d\n", pin_num, gpio_get_level(pin_num));
				}
			} else {
				printf("Invalid parameter. Need more args for get command\n");
			}
		} else if (strncmp(buf, "set", 3) == 0) {

			/* 2nd parameter is {pin num} for set */
			if (_cli_copy_nth_arg(buf, string, sizeof(buf), 2) >= 0) {
				sscanf(buf, "%d", &pin_num);
				if (pin_num >= GPIO_PIN_COUNT) {
					printf("Invalid parameter. wrong pin num\n");
					return;
				}

				/* 3rd parameter is in/out/1/0 for set */
				if (_cli_copy_nth_arg(buf, string, sizeof(buf), 3) >= 0) {

					if (strncmp(buf, "out", 3) == 0) {
						/* Set output */
						gpio_config_t io_conf;
						//disable interrupt
						io_conf.intr_type = GPIO_INTR_DISABLE;
						//set as output mode
						io_conf.mode = GPIO_MODE_OUTPUT;
						//bit mask of the pins that you want to set
						io_conf.pin_bit_mask = 1 << pin_num;
						//disable pull-down mode
						io_conf.pull_down_en = 0;
						//disable pull-up mode
						io_conf.pull_up_en = 0;
						//configure GPIO with the given settings
						gpio_config(&io_conf);
					} else if (strncmp(buf, "in", 2) == 0) {
						/* Set input */
						gpio_config_t io_conf;
						//interrupt of rising edge
						io_conf.intr_type = GPIO_INTR_POSEDGE;
						//bit mask of the pins
						io_conf.pin_bit_mask = 1 << pin_num;
						//set as input mode
						io_conf.mode = GPIO_MODE_INPUT;
						//enable pull-up mode
						io_conf.pull_up_en = 1;
						gpio_config(&io_conf);
					} else {
						sscanf(buf, "%d", &value);
						if (value > 1) {
							printf("Invalid parameter. out/in/0/1 allowed\n");
							return;
						}

						/* Set high or low. If PIN is not configured output, it will not work. */
						if (value == 1) {
							gpio_set_level(pin_num, 1);
						} else {
							gpio_set_level(pin_num, 0);
						}
					}

				} else {
					printf("Invalid parameter. Need more args for set command\n");
				}
			} else {
				printf("Invalid parameter. Need more args for set command\n");
			}
		} else {
			printf("Invalid parameter. get/set are supported\n");
			return;
		}
	} else {
		printf("Invalid parameter. args needed\n");
		return;
	}
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

static void _cli_cmd_time_info(char *string)
{
    char buf[20];
    struct timeval tv_now;
    struct timeval tv_set = {1559347200, 0}; /* default time is 2019-06-01 00:00:00 UTC */

	if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
		if (strncmp(buf, "get", 3) == 0) {
			gettimeofday(&tv_now, NULL);
			printf("current wall time is %ld\n", tv_now.tv_sec);
		} else if (strncmp(buf, "set", 3) == 0) {
			if (_cli_copy_nth_arg(buf, string, sizeof(buf), 2) >= 0) {
				sscanf(buf, "%ld", &tv_set.tv_sec);

				if (tv_set.tv_sec < 1546300800) {
					printf("settime target is too old. Need to be greater than 1546300800(2019-01-01 00:00:00 UTC)\n");
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


extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);

#ifndef BOOTLOADER_BUILD
#define FLASH_INTR_DECLARE(t)
#define FLASH_INTR_LOCK(t) vPortEnterCritical()
#define FLASH_INTR_UNLOCK(t) vPortExitCritical()
#else
#define FLASH_INTR_DECLARE(t)
#define FLASH_INTR_LOCK(t)
#define FLASH_INTR_UNLOCK(t)
#endif

//uint8_t FlashIsOnGoing = 0;
#if SAMSUNG //Todo : Check!
static unsigned long _cli_spi_flash_get_id()
{
	uint32_t rdid = 0;
	FLASH_INTR_DECLARE(c_tmp);

	FLASH_INTR_LOCK(c_tmp);

	//FlashIsOnGoing = 1;

	rdid = spi_flash_get_id_raw(&g_rom_flashchip);

	//FlashIsOnGoing = 0;

	FLASH_INTR_UNLOCK(c_tmp);

	return rdid;
}
#endif
static void _cli_flash_info() {
#if SAMSUNG //Todo : Check!
	unsigned long id = _cli_spi_flash_get_id();
	int real_size = (1 << ((id >> 16) & 0xFF));
	printf("Flash real size : %d(%dMB)\n", real_size, (real_size > 0) ? (real_size / 1024 / 1024) : 0);

	flash_size_map map = system_get_flash_size_map();
	//printf("enum map = %d\n", map);

	switch (map) {
	case FLASH_SIZE_4M_MAP_256_256:
		printf("Flash size : 4Mbits. Map : 256KBytes + 256KBytes\n");
		break;
	case  FLASH_SIZE_2M:
		printf("Flash size : 2Mbits. Map : 256KBytes\n");
		break;
	case FLASH_SIZE_8M_MAP_512_512:
		printf("Flash size : 8Mbits. Map : 512KBytes + 512KBytes\n");
		break;
	case FLASH_SIZE_16M_MAP_512_512:
		printf("Flash size : 16Mbits. Map : 512KBytes + 512KBytes\n");
		break;
	case FLASH_SIZE_32M_MAP_512_512:
		printf("Flash size : 32Mbits. Map : 512KBytes + 512KBytes\n");
		break;
	case FLASH_SIZE_16M_MAP_1024_1024:
		printf("Flash size : 16Mbits. Map : 1024KBytes + 1024KBytes\n");
		break;
	case FLASH_SIZE_32M_MAP_1024_1024:
		printf("Flash size : 32Mbits. Map : 1024KBytes + 1024KBytes\n");
		break;
	case FLASH_SIZE_32M_MAP_2048_2048:    // attention: don't support now ,just compatible for nodemcu;
		printf("Flash size : 32Mbits. Map : 2048KBytes + 2048KBytes\n");
		break;
	case FLASH_SIZE_64M_MAP_1024_1024:
		printf("Flash size : 64Mbits. Map : 1024KBytes + 1024KBytes\n");
		break;
	case FLASH_SIZE_128M_MAP_1024_1024:
		printf("Flash size : 128Mbits. Map : 1024KBytes + 1024KBytes\n");
		break;
	default:
		break;
	}
#endif
	size_t real_size = spi_flash_get_chip_size();
	printf("Flash real size : %d(%dMB)\n", real_size, (real_size > 0) ? (real_size / 1024 / 1024) : 0);

}

static void _cli_cmd_device_info(char *string)
{
	printf("\n");

	const char* buf = esp_get_idf_version();
	printf("ESP_IDF VERSION : %s\n", buf);

	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);

	if (chip_info.model == CHIP_ESP32) {
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

	_cli_flash_info();

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
static unsigned int _cli_util_gets(char *str)
{
	// Configure a temporary buffer for the input data
	uint8_t data[UART_BUF_SIZE];
	uint8_t line[33];	// the length of ssid is 32
	memset(line, 0, 33);
	int line_len = 0;

	while (1) {
		memset(data, 0, UART_BUF_SIZE);

		// Read data from the UART
		int len = uart_read_bytes(UART_NUM_0, data, UART_BUF_SIZE, 20 / portTICK_RATE_MS);
		for (int i = 0; i < len; i++) {
			switch(data[i])
			{
				case '\r':
				case '\n':
					uart_write_bytes(UART_NUM_0, "\r\n", 2);
					if (line_len >= 0) {
						line[line_len] = '\0';
						//memcpy(str, line, line_len);
						memcpy(str, line, line_len);
						IOT_INFO("line_len = %d, line = %s, str = %s", line_len, line, str);
						return line_len;
					}
					break;

				case '\b':
					//backspace
					if (line_len > 0) {
						uart_write_bytes(UART_NUM_0, "\b \b", 3);
						line[--line_len] = '\0';
					}
					break;

				default:
					uart_write_bytes(UART_NUM_0, (const char *) &data[i], 1);
					line[line_len++] = data[i];
			}
		}
	}
	return line_len;
}

static void _cli_cmd_go_main(char *string)
{
	portENTER_CRITICAL(&spinlock);
	g_StopMainTask = 0;
	portEXIT_CRITICAL(&spinlock);

	printf("go to the main task~\n");
}

static void _cli_cmd_reboot(char *string)
{
	printf("reboot the system\n");
	esp_restart();
}

static void _cli_cmd_read_prov(char *string)
{
        struct iot_device_prov_data prov_data;
        iot_error_t err;
	char location_id_buf[40] = {0};
	char room_id_buf[40] = {0};

        memset(&prov_data, 0, sizeof(prov_data));

        printf("%s: Read provisioning data", __func__);

        if ((err = iot_nv_get_prov_data(&prov_data)) != IOT_ERROR_NONE) {
                printf("%s: failed to get provisioning data: %d", __func__, err);
                return;
        }
	if((err = iot_util_convert_uuid_str(&prov_data.cloud.location_id, location_id_buf, sizeof(location_id_buf))) != IOT_ERROR_NONE)
		printf("%s: convert uuid fail %d", __func__, err);

	if((err = iot_util_convert_uuid_str(&prov_data.cloud.room_id, room_id_buf, sizeof(room_id_buf))) != IOT_ERROR_NONE)
		printf("%s: convert uuid fail %d", __func__, err);

        printf("\n\n<Provisioning data>\n"
                        "   WiFi SSID: %s\n"
                        "   WiFi PASS: %s\n"
                        "   ServerURL: %s:%d\n"
			"   Location ID: %s\n"
			"   Room ID: %s\n"
			"   Label: %s\n",
                        prov_data.wifi.ssid,
                        prov_data.wifi.password,
                        prov_data.cloud.broker_url,
                        prov_data.cloud.broker_port,
			location_id_buf, room_id_buf,
			prov_data.cloud.label);

	if(prov_data.cloud.broker_url != NULL)
		free(prov_data.cloud.broker_url);
}

static void _cli_cmd_write_prov(char *string)
{
	struct iot_device_prov_data prov_data;
	struct iot_cloud_prov_data cloud_prov = {
		.broker_url   = "mqtt-regionals-useast1.smartthingsgdev.com",
#if defined(CONFIG_STDK_IOT_CORE_NETWORK_TLS)
		.broker_port  = 8883,
#else
		.broker_port  = 1883,
#endif
	};

	char input[64] = {0};
	char output[96] = {0};
	char confirm[32] = {0};
	int  sz;
	iot_error_t err;

	memset(&prov_data, 0, sizeof(prov_data));
	memcpy(&prov_data.cloud, &cloud_prov, sizeof(cloud_prov));

	printf("%s: Write provisioning data manually\n", __func__);

	while (1) {
		printf(" WiFi SSID: ");
		memset(input, 0, sizeof(input));

		if (_cli_util_gets(input) > 0) {
		snprintf(output, sizeof(output) - 1, "\nSSID: \"%s\" [Y/n]? ", input);
		printf("%s", output);
		if (_cli_util_gets(confirm) == 0 || !strcasecmp(confirm, "y"))
		break;
		}
	}
	strncpy(prov_data.wifi.ssid, input, sizeof(prov_data.wifi.ssid) - 1);

	while (1) {
		printf(" WiFi PASS: ");
		memset(input, 0, sizeof(input));
		sz = _cli_util_gets(input);

		if (sz > 0) {
			snprintf(output, sizeof(output) - 1, "\nPASS: \"%s\" [Y/n]? ", input);
			printf("%s", output);

			if (_cli_util_gets(confirm) == 0 || !strcasecmp(confirm, "y")) {
				prov_data.wifi.security_type = IOT_WIFI_AUTH_WPA2_PSK;
				break;
			}
		} else if (sz == 0) {
			snprintf(output, sizeof(output) - 1, "\nPASS: Empty [Y/n]? ");
			printf("%s", output);

			if (_cli_util_gets(confirm) == 0 || !strcasecmp(confirm, "y")) {
				prov_data.wifi.security_type = IOT_WIFI_AUTH_OPEN;
				break;
			}
		}
	}

	strncpy(prov_data.wifi.password, input, sizeof(prov_data.wifi.password) - 1);

	printf(" location id update ? [Y/n]: ");
	if ((_cli_util_gets(confirm) > 0) && !strcasecmp(confirm, "y")) {
	while (1) {
		printf(" location id(uuid): ");
		memset(input, 0, sizeof(input));
		if (_cli_util_gets(input) > 0) {
			snprintf(output, sizeof(output) - 1, "\nUUID: \"%s\" [Y/n]? ", input);
			printf("%s", output);
		if (_cli_util_gets(confirm) == 0 || !strcasecmp(confirm, "y"))
		break;
		}
	}
	err = iot_util_convert_str_uuid(input, &prov_data.cloud.location_id);
	if (err != IOT_ERROR_NONE) {
		printf("%s: failed to convert location_id(%s): %d", __func__,
		input, err);
		return;
		}
	}

	printf(" room id update ? [Y/n]: ");

	if ((_cli_util_gets(confirm) > 0) && !strcasecmp(confirm, "y")) {
		while (1) {
			printf(" room id(uuid): ");
			memset(input, 0, sizeof(input));
			if (_cli_util_gets(input) > 0) {
				snprintf(output, sizeof(output) - 1, "\nUUID: \"%s\" [Y/n]? ", input);
				printf("%s", output);
			if (_cli_util_gets(confirm) == 0 || !strcasecmp(confirm, "y"))
				break;
			}
		}
	err = iot_util_convert_str_uuid(input, &prov_data.cloud.room_id);
	if (err != IOT_ERROR_NONE) {
		printf("%s: failed to convert profile_id(%s): %d", __func__,
		input, err);
		return;
		}
	}

	printf(" Label update ? [Y/n]: ");

	if ((_cli_util_gets(confirm) > 0) && !strcasecmp(confirm, "y")) {
		while (1) {
			printf(" Label : ");
			memset(input, 0, sizeof(input));
			if (_cli_util_gets(input) > 0) {
				snprintf(output, sizeof(output) - 1, "\nName: \"%s\" [Y/n]? ", input);
				printf("%s", output);
			if (_cli_util_gets(confirm) == 0 || !strcasecmp(confirm, "y"))
				break;
			}
		}

		if (strlen(input) > 32)
		input[32] = '\0';
		prov_data.cloud.label = input;
	}

	if ((err = iot_nv_set_prov_data(&prov_data)) != IOT_ERROR_NONE)
		printf("%s: failed to set provisioning data: %d", __func__, err);

	printf("%s: done\n", __func__);
}

static void _cli_cmd_erase_prov(char *string)
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
}

#define CLI_NV_PARTITION "stnv"
#define CLI_NV_NAMESPACE "stdk"

static void _cli_cmd_nvs_set(char *string)
{
	char buf[16], key[16];
	nvs_handle nvs_handle;
	nvs_open_mode nvs_open_mode = NVS_READWRITE;
	esp_err_t ret;

	ret = nvs_flash_init_partition(CLI_NV_PARTITION);
	if (ret != ESP_OK) {
		printf("nvs %s init fail %d\n", CLI_NV_PARTITION, ret);
		return;
	}

	ret = nvs_open_from_partition(CLI_NV_PARTITION, CLI_NV_NAMESPACE, nvs_open_mode, &nvs_handle);
	if (ret != ESP_OK) {
		printf("nvs namesapce %s open fail %d\n", CLI_NV_NAMESPACE, ret);
		return;
	}

	if (_cli_copy_nth_arg(buf, string, sizeof(buf), 1) >= 0) {
		if (strncmp(buf, "u64", 3) == 0) {
			uint64_t value;
			if ((ret = _cli_copy_nth_arg(key, string, sizeof(key), 2)) < 0) {
				printf("parse key string fail %d\n", ret);
				goto exit;
			}
			if ((ret = _cli_copy_nth_arg(buf, string, sizeof(buf), 3)) < 0) {
				printf("parse value string fail %d\n", ret);
				goto exit;
			}
			value = strtoull(buf, NULL, 10);
			ret = nvs_set_u64(nvs_handle, key, value);
			if (ret != ESP_OK) {
				printf("%s upper 4bytes: 0x%x(%d) lower 4bytes: 0x%x(%d) nvs set fail %d\n", key,
					(uint32_t)(value >> 32), (uint32_t)(value >> 32), (uint32_t)value, (uint32_t)value, ret);
				goto exit;
			}
			else
				printf("%s upper 4bytes: 0x%x(%d) lower 4bytes: 0x%x(%d) nvs set success\n", key,

					(uint32_t)(value >> 32), (uint32_t)(value >> 32), (uint32_t)value, (uint32_t)value);
			nvs_commit(nvs_handle);
		} else if (strncmp(buf, "str", 3) == 0) {
			int i, word_index = 0, start = 0;
			if ((ret = _cli_copy_nth_arg(key, string, sizeof(key), 2)) < 0) {
				printf("parse key string fail %d\n", ret);
				goto exit;
			}
			for (i = 0; string[i] != '\0'; i++) {
				if ((string[i] == ' ') && (string[i+1]!=' ') && (string[i+1]!='\0')) { //start check
					word_index++;
					if (word_index == 3) {
						start = i+1;
						break;
					}
				}
			}
			if (start != 0) {
				ret = nvs_set_str(nvs_handle, key, &string[start]);
				if (ret != ESP_OK) {
					printf("%s : %s nvs set fail %d\n", key, &string[start], ret);
					goto exit;
				}
				else
					printf("%s : %s nvs set success\n", key, &string[start]);

				nvs_commit(nvs_handle);
			} else
				printf("wrong str format\n");
		} else if (strncmp(buf, "blob", 4) == 0) {
			int i, word_index = 0, start = 0, end = -1;
			size_t length = 0;
			void *data;
			if ((ret = _cli_copy_nth_arg(key, string, sizeof(key), 2)) < 0) {
				printf("parse key string fail %d\n", ret);
				goto exit;
			}
			for (i = 0; string[i] != '\0'; i++) {
				if ((string[i] == ' ') && (string[i+1]!=' ') && (string[i+1]!='\0')) { //start check
					word_index++;
					if (word_index == 3) {
						start = i+1;
					}
				} else if ((string[i] != ' ') && ((string[i+1]==' ')||(string[i+1]=='\0'))) { //end check
					if (word_index == 3) {
						end = i;
						break;
					}
				}
			}
			if (start != 0 && end != -1) {
				data = malloc (sizeof(char) * ((end - start) / 2 + 1));
				if (data == NULL) {
					printf("data memory alloc fail\n");
					goto exit;
				}
				for (i = start; i <= end; i += 2) {
					if (string[i] >= '0' && string[i] <= '9') {
						((char *)data)[length] = (string[i] - '0') << 4;
					} else if (string[i] >= 'a' && string[i] <= 'f') {
						((char *)data)[length] = ((string[i] - 'a') + 10) << 4;
					} else if (string[i] >= 'A' && string[i] <= 'F') {
						((char *)data)[length] = ((string[i] - 'A') + 10) << 4;
					} else break;

					if (string[i + 1] >= '0' && string[i + 1] <= '9') {
						((char *)data)[length] += (string[i + 1] - '0');
					} else if (string[i + 1] >= 'a' && string[i + 1] <= 'f') {
						((char *)data)[length] += ((string[i + 1] - 'a') + 10);
					} else if (string[i + 1] >= 'A' && string[i + 1] <= 'F') {
						((char *)data)[length] += ((string[i + 1] - 'A') + 10);
					} else break;
					length++;
				}
				ret = nvs_set_blob(nvs_handle, key, data, length);
				if (ret != ESP_OK) {
					printf("%s nvs set fail %d\n", key, ret);
					goto exit;
				}
				else
					printf("%s nvs set success(%d bytes wriiten)\n", key, length);

				nvs_commit(nvs_handle);
				free(data);
			} else
				printf("wrong blob format\n");
		} else
			printf("not supporting type\n");
	} else
		printf("nvs_set [u64|str|blob] [key] [value]\n");

exit:
	nvs_close(nvs_handle);
}

static esp_err_t _cli_nvs_get_u64(nvs_handle nvs_handle, char *key) {
	uint64_t value = 0;
	esp_err_t ret;
	if ((ret = nvs_get_u64(nvs_handle, key, &value)) == ESP_OK)
		printf("%s(unsigned) : upper 4bytes: 0x%x (%d) lower 4bytes: 0x%x (%d)\n", key,
			(uint32_t)(value >> 32), (uint32_t)(value >> 32), (uint32_t)value, (uint32_t)value);
	else
		printf("%s nvs get u64 fail %d\n", key, ret);

	return ret;
}

static esp_err_t _cli_nvs_get_str(nvs_handle nvs_handle, char *key) {
	size_t length;
	char *data;
	esp_err_t ret = 0;

	if ((ret = nvs_get_str(nvs_handle, key, NULL, &length)) == ESP_OK) {
		data = malloc(sizeof(char) * length);
		if (data == NULL) {
			printf("data memory alloc fail\n");
			return -1;
		}
		ret = nvs_get_str(nvs_handle, key, data, &length);
		printf("%s : %s\n", key, data);
		free(data);
	} else
		printf("%s nvs get str fail %d\n", key, ret);

	return ret;
}

static esp_err_t _cli_nvs_get_blob(nvs_handle nvs_handle, char *key) {
	int i, o;
	size_t length;
	void *data;
	esp_err_t ret = 0;

	if ((ret = nvs_get_blob(nvs_handle, key, NULL, &length)) == ESP_OK) {
		data = malloc(sizeof(char) * length);
		if (data == NULL) {
			printf("data memory alloc fail\n");
			return -1;
		}
		ret = nvs_get_blob(nvs_handle, key, data, &length);
		printf("%s\n", key);
		for (i = 0; i < length; i += 16) {
			printf("%05d: ", i);
			for (o = 0; o < 16; o++) {
				if (i + o >= length)
					printf("   ");
				else
					printf("%02x ", ((char *)data)[i + o]);
			}
			printf("       |");
			for (o = 0; o < 16; o++) {
				if (i + o >= length)
					printf(" ");
				else
					printf("%c", ((char *)data)[i + o]);
			}
			printf("|\n");
		}
		free(data);
	} else
		printf("%s nvs get blob fail %d\n", key, ret);

	return ret;
}

static void _cli_cmd_nvs_get(char *string)
{
	char key[16], type[16];
	nvs_handle nvs_handle;
	nvs_open_mode nvs_open_mode = NVS_READONLY;
	esp_err_t ret;

	ret = nvs_flash_init_partition(CLI_NV_PARTITION);
	if (ret != ESP_OK) {
		printf("nvs %s init fail %d\n", CLI_NV_PARTITION, ret);
		return;
	}

	ret = nvs_open_from_partition(CLI_NV_PARTITION, CLI_NV_NAMESPACE, nvs_open_mode, &nvs_handle);
	if (ret != ESP_OK) {
		printf("nvs namesapce %s open fail %d\n", CLI_NV_NAMESPACE, ret);
		return;
	}

	if (_cli_copy_nth_arg(key, string, sizeof(key), 1) < 0) {
		printf("wrong format\n");
		goto exit;
	}

	/* If there is type option, get type data */
	if (strncmp(key, "-t", 2) == 0) {
		if (_cli_copy_nth_arg(type, string, sizeof(type), 2) < 0) {
			printf("type wrong format\n");
			goto exit;
		}
		if (_cli_copy_nth_arg(key, string, sizeof(key), 3) < 0) {
			printf("key wrong format\n");
			goto exit;
		}
		if (strncmp(type, "u64", 3) == 0)
			_cli_nvs_get_u64(nvs_handle, key);
		else if(strncmp(type, "str", 3) == 0)
			_cli_nvs_get_str(nvs_handle, key);
		else if(strncmp(type, "blob", 4) == 0)
			_cli_nvs_get_blob(nvs_handle, key);
		else
			printf("not supporting type\n");
	} else {
		/* If there is no type option, check every type */
		/* check if it is integer type first            */
		uint64_t value = 0;
		int found = 0;
		if (nvs_get_i8(nvs_handle, key, (int8_t*)&value) == ESP_OK) found = 1;
		else if (nvs_get_u8(nvs_handle, key, (uint8_t*)&value) == ESP_OK) found = 1;
		else if (nvs_get_i16(nvs_handle, key, (int16_t*)&value) == ESP_OK) found = 1;
		else if (nvs_get_u16(nvs_handle, key, (uint16_t*)&value) == ESP_OK) found = 1;
		else if (nvs_get_i32(nvs_handle, key, (int32_t*)&value) == ESP_OK) found = 1;
		else if (nvs_get_u32(nvs_handle, key, (uint32_t*)&value) == ESP_OK) found = 1;

		if (found) {
			printf("%s(unsigned) : 0x%x (%d)\n", key, (uint32_t)value, (uint32_t)value);
			goto exit;
		}

		if (nvs_get_i64(nvs_handle, key, (int64_t *)&value) == ESP_OK) found = 1;
		else if (nvs_get_u64(nvs_handle, key, &value) == ESP_OK) found = 1;

		if (found) {
			printf("%s(unsigned) : upper 4bytes: 0x%x (%d) lower 4bytes: 0x%x (%d)\n", key,
				(uint32_t)(value >> 32), (uint32_t)(value >> 32), (uint32_t)value, (uint32_t)value);
			goto exit;
		}

		/* check if it is str type or blob type */
		size_t length;
		void *data;

		/* To get string size */
		if (nvs_get_str(nvs_handle, key, NULL, &length) == ESP_OK) {
			data = malloc(sizeof(char) * length);
			if (data == NULL) {
				printf("data memory alloc fail\n");
				goto exit;
			}
			nvs_get_str(nvs_handle, key, (char *)data, &length);
			printf("%s : %s\n", key, (char *)data);
			free(data);
			goto exit;
		} else if (nvs_get_blob(nvs_handle, key, NULL, &length) == ESP_OK) {
			int i, o;
			data = malloc(sizeof(char) * length);
			if (data == NULL) {
				printf("data memory alloc fail\n");
				goto exit;
			}
			nvs_get_blob(nvs_handle, key, data, &length);
			printf("%s\n", key);
			for (i = 0; i < length; i += 16) {
				printf("%05d: ", i);
				for (o = 0; o < 16; o++) {
					if (i + o >= length)
						printf("   ");
					else
						printf("%02x ", ((char *)data)[i + o]);
				}
				printf("       |");
				for (o = 0; o < 16; o++) {
					if (i + o >= length)
						printf(" ");
					else
						printf("%c", ((char *)data)[i + o]);
				}
				printf("|\n");
			}
			free(data);
			goto exit;
		}
		printf("No such key data\n");
	}

exit:
	nvs_close(nvs_handle);
}

static void _cli_cmd_led_cmd(char *string)
{
	int i;
	char buf[10];
	printf("\n");
	if(_cli_copy_nth_arg(buf,string,sizeof(buf),1)>=0){
		if(strncmp(buf,"red",3) == 0){
			if(_cli_copy_nth_arg(buf,string,sizeof(buf),2) >= 0){
				if(strncmp(buf,"on",2)==0){
					printf("Red on\n");
					gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_ON);
				}else if(strncmp(buf,"off",3)==0){
					printf("Red off\n");
					gpio_set_level(GPIO_OUTPUT_LED_RED, LED_GPIO_OFF);
				}else{
					printf("Invalid parameters, on/off please\n");
				}
			}
		}else if(strncmp(buf,"blue",4) == 0){
			if(_cli_copy_nth_arg(buf,string,sizeof(buf),2) >= 0){
				if(strncmp(buf,"on",2)==0){
					printf("Blue on\n");
					gpio_set_level(GPIO_OUTPUT_LED_BLUE, LED_GPIO_ON);
				}else if(strncmp(buf,"off",3)==0){
					printf("Blue off\n");
					gpio_set_level(GPIO_OUTPUT_LED_BLUE, LED_GPIO_OFF);
				}else{
					printf("Invalid parameters, on/off please\n");
				}
			}
		}else if(strncmp(buf,"green",5) == 0){
			if(_cli_copy_nth_arg(buf,string,sizeof(buf),2) >= 0){
				if(strncmp(buf,"on",2)==0){
					printf("Green on\n");
					gpio_set_level(GPIO_OUTPUT_LED_GREEN, LED_GPIO_ON);
				}else if(strncmp(buf,"off",3)==0){
					printf("Green off\n");
					gpio_set_level(GPIO_OUTPUT_LED_GREEN, LED_GPIO_OFF);
				}else{
					printf("Invalid parameters, on/off please\n");
				}
			}
		}else{
			printf("Invalid parameters, red/blue/green please\n");
		}
	}else{
		printf("Please input parameters!\n");
	}
}

static struct cli_command cmd_list[] = {
#if SAMSUNG //Todo : Check!
	{"heap_dump", "heap_dump start_address size OR heap_dump all", _cli_cmd_heap_dump},
	{"heap_trace", "heap_trace [start|stop|resume|dump]", _cli_cmd_heap_trace},
	{"heap_info", "print heap free size and minimum size", _cli_cmd_heap_info},
#endif
	{"mem_set", "mem_set [address] [4byte(hex)], address%4=0", _cli_cmd_memory_set},
#if SAMSUNG	//Todo : Check!
	{"mem_get", "mem_get address size", _cli_cmd_heap_dump},
#endif
	{"nvs_set", "nvs_set [u64|str|blob] [key] [value]", _cli_cmd_nvs_set},
	{"nvs_get", "nvs_get [-t type] [key]", _cli_cmd_nvs_get},
	{"task_info", "show task_info", _cli_cmd_task_info},
	{"gpio", "gpio get [all|{num}] or gpio set {num} [out|in|0|1]", _cli_cmd_gpio_info},
	{"time", "time get or time set {since_epoch}", _cli_cmd_time_info},
	{"device_info", "show device info", _cli_cmd_device_info},
	{"go", "continue to process main function", _cli_cmd_go_main},
	{"reboot", "reboot the system", _cli_cmd_reboot},
	{"read_prov", "read provisioning data", _cli_cmd_read_prov},
	{"write_prov", "write provisioning data manually", _cli_cmd_write_prov},
	{"erase_prov", "erase provisioning data", _cli_cmd_erase_prov},
	{"dummy", "dummy cmd", _cli_cmd_dummy_cmd},
	{"led", "Control led [red/blue/green] [on/off]", _cli_cmd_led_cmd},
};

void register_iot_cli_cmd(void) {
	for (int i = 0; i < ARRAY_SIZE(cmd_list); i++)
		cli_register_command(&cmd_list[i]);
}
