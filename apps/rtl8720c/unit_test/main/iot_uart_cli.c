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
#include "iot_debug.h"
#include "iot_uart_cli.h"
#include "iot_util.h"
#include "serial_api.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "stdio_port_func.h"
#include "osdep_service.h"

#define UART_BUF_SIZE (20)
#define UART_LINE_SIZE (128)

#define PROMPT_STRING "STDK # "

/**
* to decide whether the main function is running or not by user action...
*  0 : running the main function
*  1 : stop for a timeout
*  2 : stop before selecting the go_main function.

*/
int g_StopMainTask = 1;
static struct cli_command_list *cli_cmd_list;
int front;
int rear;
unsigned char queue[UART_BUF_SIZE];
//SemaphoreHandle_t rx_mux = NULL;
_sema rx_mux;


static void cli_cmd_help(char *string);


void queue_init(void)
{
	front = 0;
	rear = 0;
}

int queue_is_empty(void)
{
	return (front == rear);
}

int queue_is_full(void)
{
	if ((front + 1) % UART_BUF_SIZE == rear)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int queue_enqueue(unsigned char value)
{
	if (queue_is_full())
	{
		printf("queue is full!");
		return 0;
	}
	queue[front] = value;
	front++;
	if (front == UART_BUF_SIZE)
	{
		front = 0;
	}

	return 1;
}

int queue_dequeue(unsigned char *value)
{
	if (queue_is_empty())
	{
		return 0;
	}
	*value = queue[rear];
	rear++;
	if (rear == UART_BUF_SIZE)
	{
		rear = 0;
	}
	return 1;
}



static struct cli_command help_cmd = {
	"help", "print command list", cli_cmd_help
};


static cli_cmd_t* cli_find_command (char* input_string) {
	cli_cmd_list_t* now = cli_cmd_list;

	while (now) {
		if (!now->cmd)
			continue;

		if (strncmp(input_string, now->cmd->command, strlen(now->cmd->command)) == 0) {
			switch (input_string[strlen(now->cmd->command)]) {
				case ' ':
				case '\r':
				case '\n':
				case '\0':
					return now->cmd;
			}
		}
		now = now->next;
	}

	return NULL;
}

static void cli_process_command(char* input_string)
{
	cli_cmd_t *command;

	command = cli_find_command(input_string);

	if (command == NULL) {
		printf("command not found. please check 'help'\r\n");
		return;
	}

	command->command_fn(input_string);
}

void cli_register_command(cli_cmd_t* cmd)
{
	cli_cmd_list_t* now;


	if ( (!cmd) || (!cmd->command) ) {
		IOT_ERROR("register fail : cmd is invalid.\r\n");
		return;
	}

	if (cli_find_command(cmd->command)) {
		IOT_ERROR("register fail : same cmd is already exists.\r\n");
		return;
	}

	if (!cli_cmd_list) {
		cli_cmd_list = (cli_cmd_list_t*) malloc(sizeof(struct cli_command_list));
		cli_cmd_list->next = NULL;
		cli_cmd_list->cmd = cmd;
	} else {
		now = cli_cmd_list;
		while (now->next) now = now->next;
		now->next = (cli_cmd_list_t*) malloc(sizeof(struct cli_command_list));

		now = now->next;
		now->next = NULL;
		now->cmd = cmd;
	}
}

static void cli_cmd_help(char *cmd) {
	cli_cmd_list_t* now = cli_cmd_list;

	printf("----------Command List\r\n");
	while (now) {
		if (!now->cmd)
			continue;

		printf("%10s : %s\r\n", now->cmd->command, now->cmd->help_string);
		now = now->next;
	}
}

/**
* If there is user input("\n") within a given timeout, the main function will be suspended.
*
*/
static void _cli_util_wait_for_user_input(unsigned int timeout_ms)
{
	TickType_t cur = xTaskGetTickCount();
	TickType_t end = cur + pdMS_TO_TICKS(timeout_ms);
	while (xTaskGetTickCount() < end) {
		taskENTER_CRITICAL();
		if (g_StopMainTask != 1) {
			taskEXIT_CRITICAL();
			break;
		}
		taskEXIT_CRITICAL();
		IOT_DELAY(100);
	}

	taskENTER_CRITICAL();
	if (g_StopMainTask == 1) {
		// When there is no input("\n") for a set time, main task will be executed...
		g_StopMainTask = 0;
	}
	taskEXIT_CRITICAL();

	if (g_StopMainTask != 0) {
		while (1) {
			taskENTER_CRITICAL();
			if (g_StopMainTask == 0) {
				taskEXIT_CRITICAL();
				break;
			}
			taskEXIT_CRITICAL();
			IOT_DELAY(100);
		}
	}
}

void uart_rx_irq(u32 id,u32 event)
{
	unsigned char rc=0;

	if (event == RxIrq) {
		stdio_port_getc((char*)&rc);
		queue_enqueue(rc);
		rtw_up_sema_from_isr(rx_mux);
	}
}

extern hal_uart_adapter_t log_uart;

static void rtl_uart_init()
{
	queue_init();

	hal_uart_reset_rx_fifo(&log_uart);
	hal_uart_rxind_hook(&log_uart,uart_rx_irq,0,0);
}


int uart_read_bytes(char *rxbuf, int len, uint32_t timeout_ms)
{
	int ret = 0;
	int ch = 0;
	int read = 0;

	if (xSemaphoreTake(rx_mux, (portTickType)(timeout_ms / portTICK_RATE_MS)) != pdTRUE) {
		return -1;
	}

	do {
		ret = queue_dequeue(&ch);
		if (ret == 0)
			break;

		rxbuf[read++] = ch;
	} while(1);

	return read;
}

void uart_write_bytes(char *txbuf, int len)
{
	unsigned int i = 0;

	for (i = 0; i < len; i++) {
		stdio_port_putc(*(txbuf+i));
	}
}

static void rtl_uart_cli_task()
{
	// Configure a temporary buffer for the incoming data
	uint8_t data[UART_BUF_SIZE];
	uint8_t line[UART_LINE_SIZE];
	uint8_t prev_line[UART_LINE_SIZE];
	memset(line, 0, UART_LINE_SIZE);
	memset(prev_line, 0, UART_LINE_SIZE);
	int line_len = 0;

	cli_register_command(&help_cmd);

	while (1) {
		memset(data, 0, UART_BUF_SIZE);

		// Read data from the UART
		int len = uart_read_bytes(data, UART_BUF_SIZE, 20);
		for (int i = 0; i < len; i++) {
			switch(data[i])
			{
				case '\r':
				case '\n':
					taskENTER_CRITICAL();
					if (g_StopMainTask == 1) {
						// when there is a user input("\n") within a given timeout, this value will be chaned into 2.
						// but, if there is no user input within a given timeout, this value will be changed into 0 in order to run the main function
						g_StopMainTask = 2;
					}
					taskEXIT_CRITICAL();

					uart_write_bytes("\r\n", 2);
					if (line_len) {
						cli_process_command((char *)line);
						memcpy(prev_line, line, UART_LINE_SIZE);
						memset(line, 0, UART_LINE_SIZE);
						line_len = 0;
					}
					uart_write_bytes(PROMPT_STRING, sizeof(PROMPT_STRING));
					break;

				case '\b':
					//backspace
					if (line_len > 0) {
						uart_write_bytes("\b \b", 3);
						line[--line_len] = '\0';
					}
					break;

				case 0x03: //Ctrl + C
					uart_write_bytes("^C\n", 3);
					memset(line, 0, UART_LINE_SIZE);
					line_len = 0;
					uart_write_bytes(PROMPT_STRING, sizeof(PROMPT_STRING));
					break;

				case 0x1B: //arrow keys : 0x1B 0x5B 0x41~44
					if ( data[i+1] == 0x5B ) {
						switch (data[i+2]) {
							case 0x41: //UP
								memcpy(line, prev_line, UART_LINE_SIZE);
								line_len = strlen((char*)line);
								uart_write_bytes((const char *)&data[i+1], 2);
								uart_write_bytes("\r\n", 2);
								uart_write_bytes(PROMPT_STRING, sizeof(PROMPT_STRING));
								uart_write_bytes((const char *)line, line_len);
								i+=3;
								break;
							case 0x42: //DOWN - ignore
								i+=3;
								break;
							case 0x43: //right
								if (line[line_len+1] != '\0') {
									line_len += 1;
									uart_write_bytes((const char *)&data[i], 3);
								}
								i+=3;
								break;
							case 0x44: //left
								if (line_len > 0) {
									line_len -= 1;
									uart_write_bytes((const char *)&data[i], 3);
								}
								i+=3;
								break;
							default:
								break;
						}
					}
					break;

					default:
					//check whether character is valid
					if ((data[i] >= ' ') && (data[i] <= '~')) {
						if (line_len >= UART_LINE_SIZE - 2)
							break;

						// print character back
						uart_write_bytes((const char *) &data[i], 1);

						line[line_len++] = data[i];
					}
			} // switch data[i]
		} //buf while loop
	} //main loop
}

void uart_cli_main(void)
{
	/* to decide whether the main function is running or not by user action... */
	g_StopMainTask = 1;	//default value is 1;  stop for a timeout
	rtw_init_sema(&rx_mux, 1);
	//rx_mux = xSemaphoreCreateMutex();

	rtl_uart_init();
	xTaskCreate(rtl_uart_cli_task, "uart_cli_task", 2048, NULL, CLI_TASK_PRIORITY, NULL);

	_cli_util_wait_for_user_input(2000);
}

