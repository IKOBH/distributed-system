#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "cmd_line_parser.h"
#include "pipe_utils.h"

int fork_process(process_t proc, pid_t pid, char *const args[])
{
        int input_fd[2] = {0};

        handle_client_pipe(proc, input_fd, E_PIPE_STEP_CALL, E_PIPE_DIR_PARENT_TO_CHILD);

        pid = fork();

        if (pid == -1)
        {
                perror("Failed to fork process");
                handle_client_pipe(proc, input_fd, E_PIPE_STEP_FORK_FAILURE, E_PIPE_DIR_PARENT_TO_CHILD);
                exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
                handle_client_pipe(proc, input_fd, E_PIPE_STEP_CHILD, E_PIPE_DIR_PARENT_TO_CHILD);
                execvp(args[0], args);
                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }

        handle_client_pipe(proc, input_fd, E_PIPE_STEP_PARENT, E_PIPE_DIR_PARENT_TO_CHILD);

        return EXIT_SUCCESS;
}

int run(process_t proc, char *const args[])
{
        pid_t pid;
        int status;

        fork_process(proc, pid, args);
        waitpid(pid, &status, 0);
}

int main(int argc, char **argv)
{
        get_cmds(argc, argv);
        run(E_SERVER_PROC, server_cmd);
        run(E_CLIENT_PROC, client_cmd);

        return EXIT_SUCCESS;
}