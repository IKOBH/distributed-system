#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "cmd_line_parser.h"

int fork_process(pid_t pid, char *const args[])
{
        pid = fork();

        if (pid == -1)
        {
                perror("Failed to fork process");
                exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
                execvp(args[0], args);

                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }

        return EXIT_SUCCESS;
}

int run(char *const args[])
{
        pid_t pid;
        int status;

        fork_process(pid, args);
        waitpid(pid, &status, 0);
}

int main(int argc, char **argv)
{
        get_cmds(argc, argv);
        run(server_cmd);
        run(client_cmd);

        return EXIT_SUCCESS;
}