/* ***************************************************************************
 *
 * Copyright (c) 2020 Samsung Electronics All Rights Reserved.
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

#ifndef STDK_LOG_UTIL_H
#define STDK_LOG_UTIL_H

#define LOG_HTTP_BUFFER_SIZE 2048
#define LOG_DUMP_SIZE 8192

int log_dump_send_to_url(IOT_CTX* ctx, char *url, size_t log_size);
#endif //STDK_LOG_UTIL_H