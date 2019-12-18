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
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "FreeRTOS.h"
#include "syslog.h"
#include "io_def.h"
#include "st_util.h"

#define BASE_YEAR 2000

uint32_t SEC_PER_YEAR[2] = { 31536000, 31622400 };
uint32_t SEC_PER_MONTH[2][12] =  {
	{ 2678400, 2419200, 2678400, 2592000, 2678400, 2592000,
	  2678400, 2678400, 2592000, 2678400, 2592000, 2678400 },
	{ 2678400, 2505600, 2678400, 2592000, 2678400, 2592000,
	  2678400, 2678400, 2592000, 2678400, 2592000, 2678400 },
};
uint32_t SEC_PER_DAY = 86400;
uint32_t SEC_PER_HOUR = 3600;

/**
 * Returns 1 if current year id a leap year
 */
int is_leap_year(int yr)
{
	if (!(yr%100))
		return (yr%400 == 0) ? 1 : 0;
	else
		return (yr%4 == 0) ? 1 : 0;
}

unsigned long iot_get_time(struct tm *tm)
{
	int i;
	int leapyr=0;

	unsigned long ret = 0;

	for (i = 1970; i < (tm->tm_year + 1900); i++) {
		ret += SEC_PER_YEAR[is_leap_year(i)];
	}

	if (is_leap_year(tm->tm_year + 1900))
		leapyr = 1;

	for (i = 0; i < (tm->tm_mon); i++) {
		ret += SEC_PER_MONTH[leapyr][i];
	}

	ret += ((tm->tm_mday)-1) * SEC_PER_DAY;
	ret += (tm->tm_hour) * SEC_PER_HOUR;
	ret += (tm->tm_min) * 60;
	ret += tm->tm_sec;

	return ret;
}

unsigned int iot_util_gets(char *s)
{
	char c;
	int len = 0;

	if (!s)
		return 0;

	while (1) {
		if ((c = io_def_uart_getchar()) == 0xd) {
			log_write("\n", 1);
			s[len] = '\0';
			break;
		}

		io_def_uart_putchar(c);
		s[len++] = c;
	}

	return len;
}

void iot_util_run_shell(struct iot_util_user_cmd *cmd_list, unsigned int nr_cmd)
{
	char *prompt = "STDK # ";
	unsigned int prompt_len = strlen(prompt);
	char input[128] = {0};
	int i;

	while (1) {
		log_write(prompt, prompt_len);
		memset(input, 0, sizeof(input));

		if (iot_util_gets(input) > 0) {
			if (!strncmp(input, "exit", 4))
				break;
			for (i = 0; i < nr_cmd; i++) {
				if (strncmp((const char *)input, cmd_list[i].cmd,
							strlen(cmd_list[i].cmd)) == 0) {
					log_write("\n", 1);
					cmd_list[i].fn(input);
					break;
				}
			}

			if (i >= nr_cmd)
				printf("unknown cmd: %s\n", input);
		}
	}
}

int iot_util_wait_for_user_input(unsigned int timeout_ms)
{
	const char MAGIC_KEY = 0xd;
	TickType_t cur = xTaskGetTickCount();
	TickType_t end = cur + pdMS_TO_TICKS(timeout_ms);
	char ch;

	while (xTaskGetTickCount() < end) {
		hal_uart_receive_dma(CONSOLE_UART, (unsigned char *)&ch, 1);
		if (ch == MAGIC_KEY)
			return true;
	}

	return false;
}
