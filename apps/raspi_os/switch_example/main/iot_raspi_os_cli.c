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

#include "iot_raspi_os_cli.h"
#include "os/iot_os_util.h"

#define BUF_SIZE (20)

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

static void clean_stdin()
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

static void raspi_os_cli_task()
{
    char data[BUF_SIZE];

    cli_register_command(&help_cmd);

    while (1) {
        unsigned int len = 0;
        memset(data, 0, sizeof(data));

        if (fgets(data, BUF_SIZE, stdin) != NULL) {
            if (data[0] == '\n')
                continue;
            else {
                len = strlen(data);
                if (len && data[len-1] == '\n')
                    data[--len] = '\0';
                else if (len == (BUF_SIZE - 1))
                    clean_stdin();

                cli_process_command(data);
	    }
        }
    }
}

void cli_main()
{
    iot_os_thread_create(raspi_os_cli_task, "raspi_os_cli_task", 4096, NULL, 10, NULL);
}
