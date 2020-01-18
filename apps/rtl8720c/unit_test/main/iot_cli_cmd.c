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
#include "FreeRTOS.h"
#include "task.h"
#include "iot_cli_cmd.h"

extern int g_StopMainTask;
#define UART_BUF_SIZE	(20)

int iot_cli_copy_nth_arg(char* dest, char* src, int size, int n)
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

unsigned iot_cli_util_gets(char *str)
{
	// Configure a temporary buffer for the input data
	uint8_t data[UART_BUF_SIZE];
	uint8_t line[37];	// the length of uuid is 36
	memset(line, 0, 37);
	int line_len = 0;

	while (1) {
		memset(data, 0, UART_BUF_SIZE);

		// Read data from the UART
		int len = uart_read_bytes(data, UART_BUF_SIZE, 20 / portTICK_RATE_MS);
		for (int i = 0; i < len; i++) {
			switch(data[i])
			{
				case '\r':
				case '\n':
					uart_write_bytes("\r\n", 2);
					if (line_len >= 0) {
						line[line_len] = '\0';
						//memcpy(str, line, line_len);
						memcpy(str, line, line_len);
						//IOT_INFO("line_len = %d, line = %s, str = %s", line_len, line, str);
						return line_len;
					}
					break;

				case '\b':
					//backspace
					if (line_len > 0) {
						uart_write_bytes("\b \b", 3);
						line[--line_len] = '\0';
					}
					break;

				default:
					uart_write_bytes((const char *) &data[i], 1);
					line[line_len++] = data[i];
			}
		}
	}
	return line_len;
}

static void _cli_cmd_dummy_cmd(char *string)
{
	char buf[10];
	int i = 0;
	printf("input string : %s\n", string);

	while (1) {
		if (iot_cli_copy_nth_arg(buf, string, sizeof(buf), i) < 0) {
			break;
		}
		printf("%dth arg : %s\n", i++, buf);
	}
}

/*================ update cli cmd list here====================*/
struct cli_command cmd_list[] = {
        {"wifi_test", "for test wlan interface", iot_cli_cmd_wifi_test},
        {"dummy", "dummy command for cli", _cli_cmd_dummy_cmd},
		{"iperf", "run iperf : iperf -s -i 1 or iperf -c xx.xx.xx.xx -i 1", iot_cli_cmd_network_iperf},
};
/*================ update cli cmd list end====================*/

void register_iot_cli_cmd(void) {
        for (int i = 0; i < ARRAY_SIZE(cmd_list); i++)
                cli_register_command(&cmd_list[i]);
}

