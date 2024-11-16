/**
 * @file        interaction_utils_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-07-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

// TODO: This couples yml_parser module with 'channel' module. Need to decouple it.
typedef struct fy_node yml_node_t;

typedef enum channel_direction_t
{
        E_CHANNEL_DIR_INPUT,
        E_CHANNEL_DIR_OUTPUT,
        E_CHANNEL_DIR_DUPLEX,
        E_CHANNEL_DIR_COUNT,
        E_CHANNEL_DIR_INVALID = -1
} chan_dir_t;

typedef enum channel_t
{
        E_CHANNEL_SOCKET,
        E_CHANNEL_PIPE,
        E_CHANNEL_COUNT,
        E_CHANNEL_INVALID = -1
} chan_t;

// TODO: Move 'chan_cmd_t' struct into a protocol module
typedef struct channel_command_ctx_t
{
        int cmd;
        void *cmd_args;
} chan_cmd_t;

typedef struct channel_control_block_t
{
        chan_t channel;
        chan_dir_t direction;
        void *resource;
        char *buffer;
} chan_cb_t;

typedef struct channel_ctx_t
{
        chan_cb_t *chan_cb;
        // TODO: Decouple chan_cmd from chan_ctx_t.
        //       The command is application specific & determined by the protocol chosen by the user.
        chan_cmd_t *chan_cmd;
        void *chan_specific_ctx;
} chan_ctx_t;

typedef struct channel_interface_t
{
        int (*chan_if_parse_input)(chan_cb_t *chan_cb);
        int (*chan_if_act_on_cmd)(chan_cb_t *chan_cb);
} chan_if_t;

int chan_interact(chan_if_t *chan_if, chan_cb_t *chan_cb);

/**
 * @brief       Text
 *
 * @param       chan_node       My Param doc
 * @param       chan_cb      My Param doc
 * @return      chan_cb_t*
 */
chan_cb_t *chan_alloc_cb(yml_node_t *chan_node, chan_cb_t *chan_cb);

/**
 * @brief       Text
 *
 * @param       chan_cb      My Param doc
 */
void chan_release_cb(chan_cb_t *chan_cb);