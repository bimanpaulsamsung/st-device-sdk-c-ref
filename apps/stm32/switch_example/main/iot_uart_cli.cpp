/* ***************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "mbed.h"
#include "Serial.h"

#include "iot_uart_cli.h"
#include "os/iot_os_util.h"

#define BUF_SIZE (20)

Serial pc(USBTX,USBRX);

static struct cli_command_list *cli_cmd_list;

static void cli_cmd_help(char *string);

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
        printf("command not found. please check 'help'\n");
        return;
    }

    command->command_fn(input_string);
}

void cli_register_command(cli_cmd_t* cmd)
{
    cli_cmd_list_t* now;


    if ( (!cmd) || (!cmd->command) ) {
        printf("register fail : cmd is invalid.\n");
        return;
    }

    if (cli_find_command(cmd->command)) {
        printf("register fail : same cmd is already exists.\n");
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

    printf("----------Command List\n");
    while (now) {
        if (!now->cmd)
            continue;

        printf("%15s : %s\n", now->cmd->command, now->cmd->help_string);
        now = now->next;
    }
}

static void mbed_cli_task()
{
    char serialInBuffer[BUF_SIZE]; 
    int serialCount = 0;

    cli_register_command(&help_cmd);

    while (1) {
        while (pc.readable()) {
   	    char byteIn = pc.getc();
   	    if ((byteIn == 0X0A) || (byteIn == 0X0D) || (serialCount == BUF_SIZE)) { 
                serialInBuffer[serialCount] == 0;

                if(serialCount > 0 )
                    cli_process_command(serialInBuffer);

                memset(serialInBuffer, 0, BUF_SIZE);
                serialCount = 0;
            } else {
                serialInBuffer[serialCount] = byteIn;

                if (serialCount < BUF_SIZE) 
                    serialCount++;
            }
       } 		  
    }
}

void uart_cli_main()
{
    iot_os_thread_create((void*)mbed_cli_task, "cli_task", CLI_TASK_SIZE, NULL, CLI_TASK_PRIORITY, NULL);
}
