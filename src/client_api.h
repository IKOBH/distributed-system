/**
 * @file        client_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-07-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "networking_api.h"

// TODO: These values should be user-defined & defaulted to config_node.yml or config_client.yml
#define CONNECT_SLEEP_GAP_US (50)
#define CONNECT_RETRIES (5)

typedef struct client_connection_retry_ctx_t
{
        int retry_cnt;
        int retry_sleep_time;
} client_conn_retry_ctx_t;

typedef struct client_ctx_t
{
        conn_ctx_t *connection_ctx;
        client_conn_retry_ctx_t *retry_ctx;
        // TODO: Handle 'user_args' recieved from cmd line & corresponds to client_args_t after parser refactor.
        char *user_args;

} client_ctx_t;