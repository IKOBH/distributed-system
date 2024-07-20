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

#define IP ("127.0.0.1")
#define PORT (8080)
// TODO: Support varaiable length for both send & rsc buffers.
#define SEND_BUFFER_BYTE_SIZE (1024)
#define RECV_BUFFER_BYTE_SIZE (1024)

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
        char send_buff[SEND_BUFFER_BYTE_SIZE];
        // TODO: Switch to dynamic allocated pointer (char *send_buff)
        char recv_buff[RECV_BUFFER_BYTE_SIZE];
} connection_ctx_t;