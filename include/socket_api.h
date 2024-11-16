
#pragma once

typedef struct socket_control_block_t sock_cb_t;
// TODO: This couples yml_parser module with 'pipe' module. Need to decouple it.
typedef struct fy_node yml_node_t;

/**
 * @brief       Text
 *
 * @param       chan_node     My Param doc
 * @return      sock_cb_t*
 */
sock_cb_t *sock_alloc_cb(yml_node_t *chan_node);

/**
 * @brief       Text
 *
 * @param       socket_cb       My Param doc
 */
void sock_release_cb(sock_cb_t *socket_cb);