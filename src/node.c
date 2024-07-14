#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "cmd_line_parser_api.h"
#include "pipe_utils_api.h"

#define handle_error_en(en, msg)    \
        do                          \
        {                           \
                errno = en;         \
                perror(msg);        \
                exit(EXIT_FAILURE); \
        } while (0)

#define handle_error(msg)           \
        do                          \
        {                           \
                perror(msg);        \
                exit(EXIT_FAILURE); \
        } while (0)

int fork_process(pid_t *pid_p, char *const args[], pipe_ctx_t *pipe_ctx)
{
        *pid_p = fork();

        if (*pid_p == -1)
        {
                perror("Failed to fork process");

                handle_pipe_fork_failure(pipe_ctx);
                exit(EXIT_FAILURE);
        }
        else if (*pid_p == 0)
        {

                handle_child_pipe_end(pipe_ctx);
                execvp(args[0], args);
                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }
        handle_parent_pipe_end(pipe_ctx);

        return EXIT_SUCCESS;
}

int run_client(pid_t *pid_p)
{
        int ret = EXIT_SUCCESS;
        pipe_ctx_t pipe_ctx;

        pipe_ctx_init(&pipe_ctx, E_PIPE_DIR_PARENT_TO_CHILD);

        ret |= fork_process(pid_p, client_cmd, &pipe_ctx);
        pipe_ctx_exit(&pipe_ctx);
        return ret;
}

int run_server(pid_t *pid_p)
{
        int ret = EXIT_SUCCESS;
        bool use_pipe = false;
        // TODO: No need in  'pipe_direct' in server. Fix this.
        pipe_direction_t pipe_direct = E_PIPE_DIR_PARENT_TO_CHILD;

        ret |= fork_process(pid_p, server_cmd, NULL);
        return ret;
}

int run_node()
{
        // TODO: No need in both ret & status
        int ret = EXIT_SUCCESS;
        pid_t pid[2];
        int status;

        ret |= run_server(&(pid[0]));
        ret |= run_client(&(pid[1]));

        // TODO: Handle 'status' before reusing it.
        waitpid(pid[0], &status, 0);
        waitpid(pid[1], &status, 0);
        return ret;
}

int main(int argc, char **argv)
{
        // TODO: Manage return values. Also return values of cmd_line_parser module.
        get_cmds(argc, argv);
        run_node();
        return EXIT_SUCCESS;
}