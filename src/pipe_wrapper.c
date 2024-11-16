/**
 * @file        pipe_wrapper.c
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

void pipe_alloc_chan_cb(fy_node_t *pipe_node, chan_cb_t *channel_cb) { chan_alloc_cb(pipe_node, channel_cb); }
void pipe_release_chan_cb(chan_cb_t *channel_cb) { chan_release_cb(channel_cb); }