/**
 * @file     pipe_utils.c
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
#include "pipe_utils_api.h"

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

static void get_pipe_ctx_from_user(pipe_ctx_t *pipe_ctx)
{
        // TODO: Need to use malloc in order to allocate pipe_ctx structure if actually required by user.
        //       Need also to free it when done.
        // TODO: Get 'direction' from user config_process file.
        pipe_ctx->direction = E_PIPE_DIR_PARENT_TO_CHILD;
}
/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 * @param    state               My Param doc
 */

// TODO: Consider removing all pipe APIs that calls 'handle_pipe' & only use 'handle_pipe'.
//       To do so, I'll have tount on 'state' and increase it each call to 'handle_pipe'.
//       The trick is to distinguish between parent & child stats after 'E_PIPE_STATE_INITIALIZED'
//       Since the diverge after the 'fork()' call.
static void handle_pipe(pipe_ctx_t *pipe_ctx, pipe_state_t state)
{
        if (!pipe_ctx)
                return;

        int *pipe_fd = pipe_ctx->pipe_fd;
        pipe_direction_t direction = pipe_ctx->direction;
        pipe_end_t pipe_end;
        pipe_end_t other_pipe_end;
        pipe_ctx->state = state;

        switch (state)
        {
        case E_PIPE_STATE_USER_INITIALIZED:

                get_pipe_ctx_from_user(pipe_ctx);
                return;

        case E_PIPE_STATE_INITIALIZED:
                if (pipe(pipe_ctx->pipe_fd) == -1)
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
 * @brief       Text
 *
 * @param       pipe_ctx        My Param doc
 */
void pipe_ctx_user_init(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_USER_INITIALIZED);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 * @param    direction           My Param doc
 */
// TODO: Rename to 'pipe_init'
void pipe_ctx_init(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_INITIALIZED);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_child_pipe_end(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_CHILD_READY_TO_USE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_after_use_child_pipe_end(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_CHILD_DONE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_parent_pipe_end(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_PARENT_READY_TO_USE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_pipe_fork_failure(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_FORK_FAILURE);
}

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */

// TODO: Rename to 'pipe_exit'
void pipe_ctx_exit(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_PARENT_DONE);
}