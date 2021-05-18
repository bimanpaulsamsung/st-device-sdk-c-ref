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

#include "esp_http_client.h"
#include "st_dev.h"
#include "log_util.h"

static int _send_buf_to_url(char *url, char *buf, int buf_len)
{
    int ret = 0;

    esp_http_client_config_t config = {
            .url = url,
#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4,0,0))
            .buffer_size_tx = LOG_HTTP_BUFFER_SIZE,
#else
            .buffer_size = LOG_HTTP_BUFFER_SIZE,
#endif
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        printf("Failed to initialise HTTP connection\n");
        return -1;
    }

    esp_http_client_set_method(client, HTTP_METHOD_PUT);
    esp_http_client_set_header(client, "Content-Type", "application/octet-stream");
    esp_http_client_set_post_field(client, buf, buf_len);

    ret = esp_http_client_perform(client);
    printf("ret = %d, HTTP Status = %d, content_length = %d\n", ret,
           esp_http_client_get_status_code(client),
           esp_http_client_get_content_length(client));
    esp_http_client_cleanup(client);
    return ret;
}

int log_dump_send_to_url(IOT_CTX* ctx, char *url, size_t log_size)
{
    int ret;
    char* log;
    size_t written_size = 0;
    int dump_mode = IOT_DUMP_MODE_NEED_BASE64 | IOT_DUMP_MODE_NEED_DUMP_STATE;

    ret = st_create_log_dump(ctx, &log, log_size, &written_size, dump_mode);
    if (ret < 0) {
        printf("Fail to get log dump!\n");
        return ret;
    }

    printf("log_dump - size: %d / %d\n", written_size, log_size);
    ret = _send_buf_to_url(url, log, written_size);
    free(log);
    return ret;
}