/**
 * @file        networking_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <arpa/inet.h>

#include "channel_api.h"

#define IP ("127.0.0.1")
#define PORT (8080)
// TODO: Support varaiable length for both send & rsc buffers.
#define OUTPUT_BUFFER_BYTE_SIZE (1024)
#define INPUT_BUFFER_BYTE_SIZE (1024)

typedef struct connection_ctx_t
{
        int fd;
        struct sockaddr_in address;
        // TODO:  Consider removing addrlen from struct since we can infer it from address.
        unsigned int addrlen;
        int port;
        // TODO: Switch to static allocated pointer (char ip[16]) or using string API.
        char *ip;
        // TODO: Switch to dynamic allocated pointer (char *send_buff)
        char send_buff[OUTPUT_BUFFER_BYTE_SIZE];
        // TODO: Switch to dynamic allocated pointer (char *send_buff)
        char recv_buff[INPUT_BUFFER_BYTE_SIZE];
} conn_ctx_t;

typedef struct network_ctx_t
{
        // TODO: Implement.
} net_ctx_t;

typedef struct network_channel_t
{
        chan_cb_t *net_chan;
        net_ctx_t *net_ctx;
} net_chan_t;