#pragma once

#include "cmd_line_parser_api.h"
#include "pipe_utils_api.h"

typedef enum
{
        E_SERVER_PROC,
        E_CLIENT_PROC
} process_t;

void handle_client_pipe(process_t proc, int *fd, pipe_step_t step, pipe_direction_t dir)
{
        if (proc != E_CLIENT_PROC)
        {
                return;
        }

        handle_pipe(fd, step, dir);
}