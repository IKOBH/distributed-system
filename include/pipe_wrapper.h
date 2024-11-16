/**
 * @file        pipe_wrapper.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "channel_api.h"

typedef struct fy_node yml_node_t;

void pipe_alloc_chan_cb(yml_node_t *pipe_node, chan_cb_t *channel_cb);
void pipe_release_chan_cb(chan_cb_t *channel_cb);