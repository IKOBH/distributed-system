/**
 * @file        communication_utils_wrapper.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-10-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <stdio.h>

#include "networking_api.h"
#include "process_api.h"
#include "pipe_api.h"

typedef enum channel_direction_t chan_dir_t;
typedef enum channel_t chan_t;

typedef struct fy_node yml_node_t;

/**
 * @brief       Text
 *
 * @param       chan_node       My Param doc
 * @return      const char*
 */
const char *chan_get_direction(yml_node_t *chan_node);

/**
 * @brief       Text
 *
 * @param       chan_node       My Param doc
 * @return      int
 */
int chan_get_buffer_size(yml_node_t *chan_node);