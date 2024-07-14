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

/* Used in parent process*/
void pipe_ctx_init(pipe_ctx_t *pipe_ctx, pipe_direction_t direction)
{
        if (!pipe_ctx)
                return;

        pipe_ctx->direction = direction;
        if (pipe(pipe_ctx->pipe_fd) == -1)
        {
                perror("Failed to create pipe");
                exit(EXIT_FAILURE);
        }
}

void handle_child_pipe_end(pipe_ctx_t *pipe_ctx)
{
        if (!pipe_ctx)
                return;

        int *pipe_fd = pipe_ctx->pipe_fd;
        pipe_direction_t direction = pipe_ctx->direction;
        pipe_end_t child_pipe_end_to_close = direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;
        pipe_end_t child_pipe_end_to_duplicate = 1 - child_pipe_end_to_close;

        close(pipe_fd[child_pipe_end_to_close]);
        if (dup2(pipe_fd[child_pipe_end_to_duplicate], STDIN_FILENO) == -1)
        {
                perror("Failed to duplicate read-end of pipe to STDIN");
                exit(EXIT_FAILURE);
        }
        close(pipe_fd[child_pipe_end_to_duplicate]);
}

void handle_after_use_child_pipe_end(pipe_ctx_t *pipe_ctx)
{
        if (!pipe_ctx)
                return;

        int *pipe_fd = pipe_ctx->pipe_fd;
        pipe_direction_t direction = pipe_ctx->direction;
        pipe_end_t child_pipe_end_to_close = direction ? E_PIPE_END_WRITE : E_PIPE_END_READ;

        close(pipe_fd[child_pipe_end_to_close]);
}

void handle_parent_pipe_end(pipe_ctx_t *pipe_ctx)
{
        if (!pipe_ctx)
                return;

        int *pipe_fd = pipe_ctx->pipe_fd;
        pipe_direction_t direction = pipe_ctx->direction;
        pipe_end_t parent_pipe_end_to_close = direction ? E_PIPE_END_WRITE : E_PIPE_END_READ;

        close(pipe_fd[parent_pipe_end_to_close]);
}

void handle_pipe_fork_failure(pipe_ctx_t *pipe_ctx)
{
        if (!pipe_ctx)
                return;

        int *pipe_fd = pipe_ctx->pipe_fd;

        close(pipe_fd[E_PIPE_END_READ]);
        close(pipe_fd[E_PIPE_END_WRITE]);
}

/* Used in parent process*/
void pipe_ctx_exit(pipe_ctx_t *pipe_ctx)
{
        if (!pipe_ctx)
                return;

        int *pipe_fd = pipe_ctx->pipe_fd;
        pipe_direction_t direction = pipe_ctx->direction;
        pipe_end_t parent_pipe_end_to_close = direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;

        close(pipe_fd[parent_pipe_end_to_close]);
}