/**
 * @file        channel_wrapper.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "yaml_parser_api.h"

typedef struct fy_node yml_node_t;

/**
 * @brief       Text
 *
 * @param       chan_node       My Param doc
 * @return      int
 */
const char *chan_get_direction(yml_node_t *chan_node) { return yml_parser_get_channel_direction(chan_node); }

/**
 * @brief       Text
 *
 * @param       chan_node       My Param doc
 * @return      int
 */
int chan_get_buffer_size(yml_node_t *chan_node)
{
        return 0; // TODO: Fix implementation
}