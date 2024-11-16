/**
 * @file        socket_wrapper.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "yaml_parser_api.h"
#include "channel_api.h"

void sock_alloc_chan_cb(fy_node_t *socket_node, chan_cb_t *channel_cb) { chan_alloc_cb(socket_node, channel_cb); }
void sock_release_chan_cb(chan_cb_t *channel_cb) { chan_release_cb(channel_cb); }