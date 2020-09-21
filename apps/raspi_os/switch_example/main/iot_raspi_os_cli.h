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

#ifndef _IOT_RASPI_OS_CLI_H_
#define _IOT_RASPI_OS_CLI_H_

#define ARRAY_SIZE(x) (int)(sizeof(x)/sizeof(x[0]))

typedef void (*command_function_t)(char *string);

typedef struct cli_command {
    char *command;
    char *help_string;
    command_function_t command_fn;
} cli_cmd_t;

typedef struct cli_command_list {
    cli_cmd_t* cmd;
    struct cli_command_list* next;
} cli_cmd_list_t;

void cli_main();
void cli_register_command(cli_cmd_t* cmd);

#endif /* _IOT_RASPI_OS_CLI_H_ */
