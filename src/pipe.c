/**
 * @file     pipe.c
 * @author   Shahar Avidar  (ikobh7@gmail.com)
 * @brief    Text
 * @version  0.1
 * @date     2024-07-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "pipe_wrapper.h"

typedef enum
{
        E_PIPE_STATE_UNINITIALIZED,
        E_PIPE_STATE_INITIALIZED,
        E_PIPE_STATE_CHILD_READY_TO_USE,
        E_PIPE_STATE_CHILD_DONE,
        E_PIPE_STATE_PARENT_READY_TO_USE,
        E_PIPE_STATE_PARENT_DONE,
        E_PIPE_STATE_FORK_FAILURE
} pipe_state_t;
// TODO: Delete enum after config.yml file is ready.
//       All user defined constansts should be exported to config.yml.
//       On second thougt - enums are compiled time constants & yml is not.
//       Need to rethink it.
typedef enum
{
        E_PIPE_END_READ,
        E_PIPE_END_WRITE
} pipe_end_t;

typedef enum
{
        E_PIPE_DIR_PARENT_TO_CHILD, /*Parent write, child reads.*/
        E_PIPE_DIR_CHILD_TO_PARENT  /*Child write, parent reads.*/
} pipe_direction_t;

typedef struct pipe_control_block_t
{
        chan_cb_t channel_cb;
        int pipe_fd[2];
        pipe_state_t state;
        pipe_direction_t direction; // TODO: Delete since I have it in chan_cb_t.
} pipe_cb_t;

typedef struct pipe_channel_t
{
        chan_cb_t *pipe_chan;
        pipe_cb_t *pipe_ctx;
} pipe_chan_t;

/**
 * @brief    Get the other pipe end objectText
 *
 * @param    pipe_end            My Param doc
 * @return   pipe_end_t
 */
static inline pipe_end_t get_other_pipe_end(pipe_end_t pipe_end)
{
        return 1 - pipe_end;
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 * @param    state               My Param doc
 */

// TODO: Consider removing all pipe APIs that calls 'pipe_handle' & only use 'pipe_handle'.
//       To do so, I'll have tount on 'state' and increase it each call to 'pipe_handle'.
//       The trick is to distinguish between parent & child stats after 'E_PIPE_STATE_INITIALIZED'
//       Since the diverge after the 'fork()' call.
static void pipe_handle(pipe_cb_t *pipe_cb, pipe_state_t state)
{
        if (!pipe_cb)
                return;

        int *pipe_fd = pipe_cb->pipe_fd;
        pipe_direction_t direction = pipe_cb->direction;
        pipe_end_t pipe_end;
        pipe_end_t other_pipe_end;
        pipe_cb->state = state;

        switch (state)
        {
        case E_PIPE_STATE_INITIALIZED:
                if (pipe(pipe_cb->pipe_fd) == -1)
                {
                        perror("Failed to create pipe");
                        exit(EXIT_FAILURE);
                }
                return;

        case E_PIPE_STATE_CHILD_DONE:
        case E_PIPE_STATE_PARENT_READY_TO_USE:
                pipe_end = direction ? E_PIPE_END_WRITE : E_PIPE_END_READ;
                break;

        case E_PIPE_STATE_CHILD_READY_TO_USE:
                pipe_end = direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;
                other_pipe_end = get_other_pipe_end(pipe_end);
                // TODO: Do I really need to use STDIN_FILENO or maybe it's better to use another fd?
                if (dup2(pipe_fd[other_pipe_end], STDIN_FILENO) == -1)
                {
                        perror("Failed to duplicate read-end of pipe to STDIN");
                        exit(EXIT_FAILURE);
                }
                close(pipe_fd[other_pipe_end]);
                break;
        case E_PIPE_STATE_PARENT_DONE:
                pipe_end = direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;
                break;
        case E_PIPE_STATE_FORK_FAILURE:
                pipe_end = E_PIPE_END_READ;
                other_pipe_end = get_other_pipe_end(pipe_end);
                close(pipe_fd[other_pipe_end]);
                break;

        default:
                break;
        }

        close(pipe_fd[pipe_end]);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 * @param    direction           My Param doc
 */
// TODO: Rename to 'pipe_init'
void pipe_ctx_init(pipe_cb_t *pipe_cb)
{
        pipe_handle(pipe_cb, E_PIPE_STATE_INITIALIZED);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_handle_child_end(pipe_cb_t *pipe_ctx)
{
        pipe_handle(pipe_ctx, E_PIPE_STATE_CHILD_READY_TO_USE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_after_use_child_pipe_end(pipe_cb_t *pipe_ctx)
{
        pipe_handle(pipe_ctx, E_PIPE_STATE_CHILD_DONE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_handle_parent_end(pipe_cb_t *pipe_ctx)
{
        pipe_handle(pipe_ctx, E_PIPE_STATE_PARENT_READY_TO_USE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_handle_fork_failure(pipe_cb_t *pipe_ctx)
{
        pipe_handle(pipe_ctx, E_PIPE_STATE_FORK_FAILURE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */

// TODO: Rename to 'pipe_exit'
void pipe_ctx_exit(pipe_cb_t *pipe_ctx)
{
        pipe_handle(pipe_ctx, E_PIPE_STATE_PARENT_DONE);
}

/**
 * @brief       Text
 *
 * @param       pipe_node       My Param doc
 * @return      pipe_cb_t*
 */
pipe_cb_t *pipe_alloc_cb(yml_node_t *pipe_node)
{
        pipe_cb_t *pipe_cb = (pipe_cb_t *)malloc(sizeof(pipe_cb_t));
        if (pipe_cb == NULL)
        {
                perror("NULL POINTER pipe_cb");
                exit(EXIT_FAILURE);
        }

        pipe_cb->channel_cb.channel = E_CHANNEL_PIPE;
        // TODO: Wrap 'chan_alloc_cb' with 'pipe_alloc_channel_cb' in pipe_wrapper.h
        pipe_alloc_chan_cb(pipe_node, &pipe_cb->channel_cb);
}

/**
 * @brief       Text
 *
 * @param       pipe_cb         My Param doc
 */
void pipe_release_cb(pipe_cb_t *pipe_cb)
{
        // TODO: Wrap 'chan_release_cb' with 'pipe_release_channel_cb' in pipe_wrapper.h
        pipe_release_chan_cb(&pipe_cb->channel_cb);
        free(pipe_cb);
}