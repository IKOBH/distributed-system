#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "pipe_utils_api.h"
typedef enum
{
        E_PIPE_END_READ,
        E_PIPE_END_WRITE
} pipe_end_t;

static void handle_pipe(pipe_ctx_t *pipe_ctx, pipe_state_t state)
{
        if (!pipe_ctx)
                return;

        int *pipe_fd = pipe_ctx->pipe_fd;
        pipe_direction_t direction = pipe_ctx->direction;
        pipe_end_t pipe_end;
        pipe_end_t duplicated_pipe_end;

        switch (state)
        {
        case E_PIPE_STATE_INITIALIZED:
                pipe_ctx->direction = direction;
                if (pipe(pipe_ctx->pipe_fd) == -1)
                {
                        perror("Failed to create pipe");
                        exit(EXIT_FAILURE);
                }
        case E_PIPE_STATE_CHILD_DONE:
        case E_PIPE_STATE_PARENT_READY_TO_USE:
                pipe_end = direction ? E_PIPE_END_WRITE : E_PIPE_END_READ;
                break;

        case E_PIPE_STATE_CHILD_READY_TO_USE:
                duplicated_pipe_end = direction ? E_PIPE_END_WRITE : E_PIPE_END_READ;
        case E_PIPE_STATE_PARENT_DONE:
                pipe_end = direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;
                break;
        case E_PIPE_STATE_FORK_FAILURE:
                close(pipe_fd[E_PIPE_END_READ]);
                close(pipe_fd[E_PIPE_END_WRITE]);
                break;

        default:
                break;
        }

        close(pipe_fd[pipe_end]);
        duplicated_pipe_end = 1 - pipe_end;

        if (pipe_ctx->state == E_PIPE_STATE_CHILD_READY_TO_USE)
        {
                if (dup2(pipe_fd[duplicated_pipe_end], STDIN_FILENO) == -1)
                {
                        perror("Failed to duplicate read-end of pipe to STDIN");
                        exit(EXIT_FAILURE);
                }
                close(pipe_fd[duplicated_pipe_end]);
        }

        pipe_ctx->state = state;
}

/* Used in parent process*/
void pipe_ctx_init(pipe_ctx_t *pipe_ctx, pipe_direction_t direction)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_INITIALIZED);
}

void handle_child_pipe_end(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_CHILD_READY_TO_USE);
}

void handle_after_use_child_pipe_end(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_CHILD_DONE);
}

void handle_parent_pipe_end(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_PARENT_READY_TO_USE);
}

void handle_pipe_fork_failure(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_FORK_FAILURE);
}

/* Used in parent process*/
void pipe_ctx_exit(pipe_ctx_t *pipe_ctx)
{
        handle_pipe(pipe_ctx, E_PIPE_STATE_PARENT_DONE);
}