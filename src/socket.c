
/**
 * @file        socket.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdlib.h>
#include <stdio.h>

#include "socket_wrapper.h"

typedef struct socket_control_block_t
{
        chan_cb_t chan_cb;
} sock_cb_t;

sock_cb_t *sock_alloc_cb(yml_node_t *socket_node)
{
        sock_cb_t *sock_cb = (sock_cb_t *)malloc(sizeof(sock_cb_t));
        if (sock_cb == NULL)
        {
                perror("NULL POINTER socket_cb");
                exit(EXIT_FAILURE);
        }

        sock_cb->chan_cb.channel = E_CHANNEL_SOCKET;
        // TODO: Wrap 'chan_alloc_cb' with 'socket_alloc_channel_cb' in socket_wrapper.h
        sock_alloc_chan_cb(socket_node, &sock_cb->chan_cb);
}

// TODO: Move to socket\network module?
void sock_release_cb(sock_cb_t *sock_cb)
{
        sock_release_chan_cb(&sock_cb->chan_cb);
        free(sock_cb);
}