/**
 * @file        channel.c
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
#include <string.h>
#include <assert.h>

#include "channel_wrapper.h"

/**
 * @brief       Text
 *
 * @param       chan_cb   My Param doc
 * @return      int
 */
static int comm_get_socket_input(chan_cb_t *chan_cb)
{
        int ret = 0;
        int bytes_recived = 0;
        int socket_fd = *(int *)chan_cb->resource;

        printf("Spin Recieve thread\n");
        if ((bytes_recived = recv(socket_fd, chan_cb->buffer, INPUT_BUFFER_BYTE_SIZE, 0)) == -1)
        {
                perror("Failed to receive data");
                close(socket_fd);
                exit(EXIT_FAILURE);
        }

        printf("Received: %s\n", chan_cb->buffer);

        return ret;
}

/**
 * @brief       Text
 *
 * @param       chan_cb   My Param doc
 * @return      int
 */
static int comm_get_pipe_input(chan_cb_t *chan_cb)
{
        int ret = 0;

        if (!fgets(chan_cb->buffer, OUTPUT_BUFFER_BYTE_SIZE, stdin))
        {
                if (feof(stdin))
                        printf("End of file reached.\n");
                else if (ferror(stdin))
                        perror("Failed reading from pipe");
                else
                        perror("Failed: Unknown fgets error");

                chan_cb->buffer = NULL;
                ret = -1;
        }

        return ret;
}

/**
 * @brief       Text
 *
 * @param       chan_cb    My Param doc
 * @return      int
 */
static int chan_get_input(chan_cb_t *chan_cb)
{
        int ret = 0;

        switch (chan_cb->channel)
        {
        case E_CHANNEL_PIPE:
                ret |= comm_get_pipe_input(chan_cb);
                break;
        case E_CHANNEL_SOCKET:
                ret |= comm_get_socket_input(chan_cb);
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
 * @param       chan_if         My Param doc
 * @param       chan_cb         My Param doc
 * @return      int
 */
int chan_interact(chan_if_t *chan_if, chan_cb_t *chan_cb)
{
        int ret = 0;

        while (true)
        {
                printf("> ");
                ret |= chan_get_input(chan_cb);
                // TODO: Decouple input parsing & action (chan_if_parse_input & chan_if_act_on_cmd) from 'channel' module to protocol module.
                //       Use another thread for it.
                ret |= chan_if->chan_if_parse_input(chan_cb);
                ret |= chan_if->chan_if_act_on_cmd(chan_cb);

                if (ret)
                        break;
        }

        return ret;
}

/**
 * @brief       Text
 *
 * @param       chan    My Param doc
 * @return      void*
 */
void *chan_alloc_resource(chan_t chan)
{
        switch (chan)
        {
        case E_CHANNEL_PIPE:
                return (FILE *)malloc(sizeof(FILE));
        case E_CHANNEL_SOCKET:
                return (int *)malloc(sizeof(int));

        default:
                perror("Invalid communication channel");
                exit(EXIT_FAILURE);
        }
}

chan_dir_t chan_dir_str_to_enum(const char *chan_dir_str)
{
        if (strcmp(chan_dir_str, "duplex") == 0)
                return E_CHANNEL_DIR_DUPLEX;
        else if (strcmp(chan_dir_str, "input") == 0)
                return E_CHANNEL_DIR_INPUT;
        else if (strcmp(chan_dir_str, "output") == 0)
                return E_CHANNEL_DIR_OUTPUT;
        else
        {
                perror("Invalid channel direction");
                exit(EXIT_FAILURE);
        }
}

/**
 * @brief       Text
 *
 * @param       chan_node       My Param doc
 * @param       chan_cb         My Param doc
 * @return      chan_cb_t*
 */
// TODO: This function doesnt actually allocate the channel.
//       Only channel internals are allocated.
//       Need to treat\name it differently than pipe or socket modules
chan_cb_t *chan_alloc_cb(yml_node_t *chan_node, chan_cb_t *chan_cb)
{
        chan_cb->direction = chan_dir_str_to_enum(chan_get_direction(chan_node));
        assert(chan_cb->direction >= 0 && chan_cb->direction < E_CHANNEL_DIR_COUNT);
        chan_cb->resource = chan_alloc_resource(chan_cb->channel);
        chan_cb->buffer = (char *)malloc(chan_get_buffer_size(chan_node));

        return chan_cb;
}

/**
 * @brief       Text
 *
 * @param       chan_cb      My Param doc
 */
// TODO: This function doesnt actually releases allocated channel.
//       Only channel internals are released.
//       Need to treat\name it differently than pipe or socket modules
void chan_release_cb(chan_cb_t *chan_cb)
{
        free(chan_cb->resource);
        free(chan_cb->buffer);
}

/**
 * @brief       Text
 *
 * @param       resource        My Param doc
 */
static void chan_release_res(void *resource)
{
        // NOTE: Calls to this function may be rplaced with just using 'free()'.
        //       But I've decided to add it since in the  future 'resource' may include
        //       more than just basic data types.
        if (resource != stdin && resource != stdout && resource != stderr)
                free(resource);
}