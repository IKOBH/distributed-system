#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "pipe_utils_api.h"

typedef enum
{
        E_PIPE_END_READ,
        E_PIPE_END_WRITE
} pipe_end_t;

void handle_pipe_create(int pipe_fd[])
{
        if (pipe(pipe_fd) == -1)
        {
                perror("Failed to create pipe");
                exit(EXIT_FAILURE);
        }
}

void handle_child_pipe_end(int pipe_fd[], pipe_direction_t direction)
{
        pipe_end_t child_pipe_end_to_close = direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;
        pipe_end_t child_pipe_end_to_duplicate = 1 - child_pipe_end_to_close;

        close(pipe_fd[child_pipe_end_to_close]);
        if (dup2(pipe_fd[child_pipe_end_to_duplicate], STDIN_FILENO) == -1)
        {
                perror("Failed to duplicate read-end of pipe to STDIN");
                exit(EXIT_FAILURE);
        }
}

void handle_parent_pipe_end(int pipe_fd[], pipe_direction_t direction)
{
        pipe_end_t parent_pipe_end_to_close = direction ? E_PIPE_END_WRITE : E_PIPE_END_READ;

        close(pipe_fd[parent_pipe_end_to_close]);
}

void handle_pipe_fork_failure(int pipe_fd[])
{
        close(pipe_fd[E_PIPE_END_READ]);
        close(pipe_fd[E_PIPE_END_WRITE]);
}