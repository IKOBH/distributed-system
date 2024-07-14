#pragma once

typedef enum
{
        E_PIPE_STEP_CALL,
        E_PIPE_STEP_CHILD,
        E_PIPE_STEP_PARENT,
        E_PIPE_STEP_FORK_FAILURE
} pipe_step_t;

typedef enum
{
        E_PIPE_DIR_PARENT_TO_CHILD, /*Parent write, child reads.*/
        E_PIPE_DIR_CHILD_TO_PARENT  /*Child write, parent reads.*/
} pipe_direction_t;

typedef struct pipe_ctx_t
{
        int pipe_fd[2];
        pipe_direction_t direction;
} pipe_ctx_t;

void pipe_ctx_init(pipe_ctx_t *pipe_ctx, pipe_direction_t direction);
void handle_child_pipe_end(pipe_ctx_t *pipe_ctx);
void handle_after_use_child_pipe_end(pipe_ctx_t *pipe_ctx);
void handle_parent_pipe_end(pipe_ctx_t *pipe_ctx);
void handle_pipe_fork_failure(pipe_ctx_t *pipe_ctx);
void pipe_ctx_exit(pipe_ctx_t *pipe_ctx);