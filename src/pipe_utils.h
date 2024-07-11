#pragma once

typedef enum
{
        E_SERVER_PROC,
        E_CLIENT_PROC
} process_t;

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

void handle_client_pipe(process_t proc, int *fd, pipe_step_t step, pipe_direction_t dir);