/**
 * @file        communication_utils.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-07-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "communication_utils_api.h"
#include "networking_api.h"
#include "procs_mgr_api.h"

/**
 * @brief       Text
 *
 * @param       comm_chan_ctx   My Param doc
 * @return      int
 */
static int comm_get_network_input(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;
        int bytes_recived = 0;
        int socket_fd = comm_chan_ctx->comm_chan->fd;

        printf("Spin Recieve thread\n");
        if ((bytes_recived = recv(socket_fd, comm_chan_ctx->comm_chan->buffer, INPUT_BUFFER_BYTE_SIZE, 0)) == -1)
        {
                perror("Failed to receive data");
                close(socket_fd);
                exit(EXIT_FAILURE);
        }

        printf("Received: %s\n", comm_chan_ctx->comm_chan->buffer);

        return ret;
}

/**
 * @brief       Text
 *
 * @param       comm_chan_ctx   My Param doc
 * @return      int
 */
static int comm_get_pipe_input(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;
        int chan_input_fd = comm_chan_ctx->comm_chan->fd;
        // TODO: fdopen & fclose are called within a loop,
        //       Consider moving them to an outer function.
        FILE *chan_input_stream = fdopen(chan_input_fd, "r");
        if (chan_input_stream == NULL)
        {
                perror("Failed to open file stream");
                ret = -1;
        }

        if (!fgets(comm_chan_ctx->comm_chan->buffer, OUTPUT_BUFFER_BYTE_SIZE, chan_input_stream))
        {
                if (feof(chan_input_stream))
                        printf("End of file reached.\n");
                else if (ferror(chan_input_stream))
                        perror("Failed reading from pipe");
                else
                        perror("Failed: Unknown fgets error");

                comm_chan_ctx->comm_chan->buffer = NULL;
                ret = -1;
        }

        if (chan_input_fd != STDIN_FILENO && chan_input_fd != STDOUT_FILENO && chan_input_fd != STDERR_FILENO)
                fclose(chan_input_stream);

        return ret;
}

/**
 * @brief       Text
 *
 * @param       node_interaction_ctx My Param doc
 * @return      int
 */
static int comm_get_input(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;

        switch (comm_chan_ctx->comm_chan->channel)
        {
        case E_CHANNEL_STDIN:
        case E_CHANNEL_PIPE:
                ret |= comm_get_pipe_input(comm_chan_ctx);
                break;
        case E_CHANNEL_NETWORK:
                ret |= comm_get_network_input(comm_chan_ctx);
                break;

        default:
                perror("Invalid communication channel");
                exit(EXIT_FAILURE);
        }

        return ret;
}

/**
 * @brief       Text
 *
 * @param       comm_if         My Param doc
 * @param       comm_chan_ctx   My Param doc
 * @return      int
 */
int comm_interact(comm_if_t *comm_if, comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;

        while (true)
        {
                printf("> ");
                ret |= comm_get_input(comm_chan_ctx);
                // TODO: Decouple input parsing & action (comm_if_parse_input & comm_if_act_on_cmd) from comm_utils module to protocol module.
                //       Use anothre thread for it.
                ret |= comm_if->comm_if_parse_input(comm_chan_ctx);
                ret |= comm_if->comm_if_act_on_cmd(comm_chan_ctx);

                if (ret)
                        break;
        }

        return ret;
}

/**
 * @brief       Text
 *
 * @param       chan_type       My Param doc
 * @param       buffer_byte_count My Param doc
 * @return      comm_chan_ctx_t*
 */
comm_chan_ctx_t *comm_alloc_ctx(int buffer_byte_count)
{
        comm_chan_ctx_t *comm_chan_ctx = NULL;

        comm_chan_ctx = (comm_chan_ctx_t *)malloc(sizeof(comm_chan_ctx_t));
        if (!comm_chan_ctx)
                return NULL;

        comm_chan_ctx->comm_chan = (comm_chan_t *)malloc(sizeof(comm_chan_t));
        if (!comm_chan_ctx->comm_chan)
                goto release_chan_ctx;

        comm_chan_ctx->comm_chan->buffer = (char *)malloc(buffer_byte_count * sizeof(char));
        if (!comm_chan_ctx->comm_chan->buffer)
                goto release_chan_ctx_chan;

        // TODO: Might need to export comm_cmd allocation to a separate func, depends on comm_chan_ctx_t fields.
        comm_chan_ctx->comm_cmd = (comm_cmd_t *)malloc(sizeof(comm_cmd_t));
        if (!comm_chan_ctx->comm_cmd)
                goto release_chan_ctx_chan_buffer;

        // TODO: Move the 'procs_mgr_alloc_pipes_ctxs' call to 'comm_utils_wrapper.h' with all other outer API calls.
        comm_chan_ctx->chan_specific_ctx = procs_mgr_alloc_pipes_ctxs();
        if (!comm_chan_ctx->chan_specific_ctx)
                goto release_chan_ctx_cmd;

        return comm_chan_ctx;

release_chan_ctx_cmd:
        free(comm_chan_ctx->comm_cmd);
release_chan_ctx_chan_buffer:
        free(comm_chan_ctx->comm_chan->buffer);
release_chan_ctx_chan:
        free(comm_chan_ctx->comm_chan);
release_chan_ctx:
        free(comm_chan_ctx);
        return NULL;
}

/**
 * @brief       Text
 *
 * @param       comm_chan_ctx   My Param doc
 * @param       resource        My Param doc
 */
void comm_init_ctx(comm_chan_ctx_t *comm_chan_ctx, chan_t chan_type, int fd)
{
        comm_chan_ctx->comm_chan->channel = chan_type;
        comm_chan_ctx->comm_chan->fd = fd;
}

/**
 * @brief       Text
 *
 * @param       comm_chan_ctx   My Param doc
 */
void comm_release_ctx(comm_chan_ctx_t *comm_chan_ctx)
{

        procs_mgr_release_pipes_ctxs(comm_chan_ctx->chan_specific_ctx);
        free(comm_chan_ctx->comm_cmd);
        free(comm_chan_ctx->comm_chan->buffer);
        free(comm_chan_ctx->comm_chan);
        free(comm_chan_ctx);
}