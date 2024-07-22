#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "communication_utils_api.h"
#include "networking_api.h"
#include "procs_mgr_api.h"

static int node_get_network_input(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;
        int bytes_recived = 0;
        int socket_fd = *(int *)comm_chan_ctx->comm_chan->resources;

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
 * @param       node_interaction_ctx My Param doc
 * @return      int
 */
static int node_get_pipe_input(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;

        if (!fgets(comm_chan_ctx->comm_chan->buffer, OUTPUT_BUFFER_BYTE_SIZE, stdin))
        {
                if (feof(stdin))
                        printf("End of file reached.\n");
                else if (ferror(stdin))
                        perror("Failed reading from pipe");
                else
                        perror("Failed: Unknown fgets error");

                comm_chan_ctx->comm_chan->buffer = NULL;
                ret = -1;
        }

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
        case E_CHANNEL_PIPE:
                ret |= node_get_pipe_input(comm_chan_ctx);
                break;
        case E_CHANNEL_NETWORK:
                ret |= node_get_network_input(comm_chan_ctx);
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
 * @param       node_interaction_ctx My Param doc
 * @return      int
 */
int comm_interact(comm_if_t *comm_if, comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;

        while (true)
        {
                printf("> ");
                ret |= comm_get_input(comm_chan_ctx);
                ret |= comm_if->comm_if_interpret_input(comm_chan_ctx);
                ret |= comm_if->comm_if_act_on_cmd(comm_chan_ctx);

                if (ret)
                        break;
        }

        return ret;
}

static void *comm_alloc_res(chan_t chan_type)
{
        switch (chan_type)
        {
        case E_CHANNEL_PIPE:
                return (FILE *)malloc(sizeof(FILE));
        case E_CHANNEL_NETWORK:
                return (int *)malloc(sizeof(int));

        default:
                perror("Invalid communication channel");
                exit(EXIT_FAILURE);
        }

        return NULL;
}

static void comm_release_res(void *resource)
{
        // NOTE: Calls to this function may be rplaced with just using 'free()'.
        //       But I've decided to add it since in the  future 'resource' may include
        //       more than just basic data types.
        if (resource != stdin && resource != stdout && resource != stderr)
                free(resource);
}

/**
 * @brief       Text
 *
 * @param       input_fd        My Param doc
 * @param       input_stream    My Param doc
 * @param       input_buffer_byte_size My Param doc
 * @return      node_comm_ctx_t*
 */
comm_chan_ctx_t *comm_alloc_ctx(chan_t chan_type, int input_buffer_byte_size)
{
        comm_chan_ctx_t *comm_chan_ctx;

        comm_chan_ctx = (comm_chan_ctx_t *)malloc(sizeof(comm_chan_ctx_t));
        comm_chan_ctx->comm_chan = (comm_chan_t *)malloc(sizeof(comm_chan_t));
        comm_chan_ctx->comm_chan->channel = chan_type;
        comm_chan_ctx->comm_chan->resources = comm_alloc_res(chan_type);
        comm_chan_ctx->comm_chan->buffer = (char *)malloc(input_buffer_byte_size * sizeof(char));
        // TODO: might need to export comm_cmd allocation to a seperate func, depends on comm_chan_ctx_t fields.
        comm_chan_ctx->comm_cmd = (comm_cmd_t *)(malloc(sizeof(comm_cmd_t)));
        // TODO: Move the 'procs_mgr_alloc_pipes_ctxs' call to 'comm_utils_wrapper.h' with all other outer API calls.
        comm_chan_ctx->chan_specific_ctx = procs_mgr_alloc_pipes_ctxs();

        return comm_chan_ctx;
}

/**
 * @brief       Text
 *
 * @param       node_interaction_ctx My Param doc
 * @param       input_fd        My Param doc
 * @param       input_stream    My Param doc
 */
void comm_init_ctx(comm_chan_ctx_t *comm_chan_ctx, void *resource)
{
        comm_chan_ctx->comm_chan->resources = resource;
}

/**
 * @brief       Text
 *
 * @param       node_interaction_ctx My Param doc
 */
void comm_release_ctx(comm_chan_ctx_t *comm_chan_ctx)
{

        procs_mgr_release_pipes_ctxs(comm_chan_ctx->chan_specific_ctx);
        free(comm_chan_ctx->comm_cmd);
        free(comm_chan_ctx->comm_chan->buffer);
        comm_release_res(comm_chan_ctx->comm_chan->resources);
        comm_chan_ctx->comm_chan->channel = E_CHANNEL_INVALID;
        free(comm_chan_ctx->comm_chan);
        free(comm_chan_ctx);
}