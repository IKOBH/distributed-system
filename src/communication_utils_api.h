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

typedef enum channel_direction_t
{
        E_CHANNEL_DIR_INPUT,
        E_CHANNEL_DIR_OUTPUT,
        E_CHANNEL_DIR_INVALID = -1
} chan_dir_t;

typedef enum channel_t
{
        E_CHANNEL_NETWORK,
        E_CHANNEL_PIPE,
        E_CHANNEL_COUNT,
        E_CHANNEL_INVALID = -1
} chan_t;

typedef struct communication_channel_t
{
        chan_t channel;
        chan_dir_t direction;
        int fd;
        char *buffer;
} comm_chan_t;

typedef struct communication_command_ctx_t
{
        int cmd;
        void *cmd_args;
} comm_cmd_t;

typedef struct commmunication_channel_ctx_t
{
        comm_chan_t *comm_chan;
        comm_cmd_t *comm_cmd;
        void *chan_specific_ctx;
} comm_chan_ctx_t;

typedef struct communication_interface_t
{
        int (*comm_if_parse_input)(comm_chan_ctx_t *comm_chan_ctx);
        int (*comm_if_act_on_cmd)(comm_chan_ctx_t *comm_chan_ctx);
} comm_if_t;

int comm_interact(comm_if_t *comm_if, comm_chan_ctx_t *node_comm_ctx);
comm_chan_ctx_t *comm_alloc_ctx(int buffer_byte_count);
void comm_init_ctx(comm_chan_ctx_t *comm_chan_ctx, chan_t chan_type, int fd);
void comm_release_ctx(comm_chan_ctx_t *node_comm_ctx);