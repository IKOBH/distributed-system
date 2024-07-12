#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "cmd_line_parser_api.h"
#include "pipe_utils_api.h"

void try_handle_pipe_create(int pipe_fd[], bool use_pipe)
{
        if (use_pipe)
                handle_pipe_create(pipe_fd);
}
void try_handle_child_pipe_end(int pipe_fd[], pipe_direction_t direction, bool use_pipe)
{
        if (use_pipe)
                handle_child_pipe_end(pipe_fd, direction);
}
void try_handle_parent_pipe_end(int pipe_fd[], pipe_direction_t direction, bool use_pipe)
{
        if (use_pipe)
                handle_parent_pipe_end(pipe_fd, direction);
}
void try_handle_pipe_fork_failure(int pipe_fd[], bool use_pipe)
{
        if (use_pipe)
                handle_pipe_fork_failure(pipe_fd);
}