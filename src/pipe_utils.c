#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "pipe_utils.h"

typedef enum
{
        E_PIPE_END_READ,
        E_PIPE_END_WRITE
} pipe_end_t;

void handle_client_pipe(process_t proc, int *fd, pipe_step_t step, pipe_direction_t dir)
{
        pipe_end_t child_pipe_end_to_close = dir ? E_PIPE_END_READ : E_PIPE_END_WRITE;
        pipe_end_t child_pipe_end_to_duplicate = 1 - child_pipe_end_to_close;
        pipe_end_t parent_pipe_end_to_close = child_pipe_end_to_duplicate;

        if (proc != E_CLIENT_PROC)
        {
                return;
        }

        switch (step)
        {
        case (E_PIPE_STEP_CALL):
                if (pipe(fd) == -1)
                {
                        perror("Failed to create pipe");
                        exit(EXIT_FAILURE);
                }
                break;

        case (E_PIPE_STEP_CHILD):
                close(fd[child_pipe_end_to_close]);
                if (dup2(fd[child_pipe_end_to_duplicate], STDIN_FILENO) == -1)
                {
                        perror("Failed to duplicate read-end of pipe to STDIN");
                        exit(EXIT_FAILURE);
                }
                break;

        case (E_PIPE_STEP_PARENT):
                close(fd[parent_pipe_end_to_close]);
                break;
        case (E_PIPE_STEP_FORK_FAILURE):
                close(fd[E_PIPE_END_READ]);
                close(fd[E_PIPE_END_WRITE]);
                break;
        default:
                perror("Invalid pipe step");
                exit(EXIT_FAILURE);
        }
}