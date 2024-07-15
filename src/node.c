/**
 * @file     node.c
 * @author   Shahar Avidar  (ikobh7@gmail.com)
 * @brief    Text
 * @version  0.1
 * @date     2024-07-15
 *
 * @copyright Copyright (c) 2024
 *
 */

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

/**
 * @brief    Text
 *
 * @param    pid_p               My Param doc
 * @param    args                My Param doc
 * @param    pipe_ctx            My Param doc
 * @return   int
 */
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

/**
 * @brief    Text
 *
 * @param    pid_p               My Param doc
 * @return   int
 */
int run_client(pid_t *pid_p)
{
        int ret = EXIT_SUCCESS;
        pipe_ctx_t pipe_ctx;

        pipe_ctx_init(&pipe_ctx, E_PIPE_DIR_PARENT_TO_CHILD);

        ret |= fork_process(pid_p, client_cmd, &pipe_ctx);
        pipe_ctx_exit(&pipe_ctx);
        return ret;
}

/**
 * @brief    Text
 *
 * @param    pid_p               My Param doc
 * @return   int
 */
int run_server(pid_t *pid_p)
{
        int ret = EXIT_SUCCESS;
        bool use_pipe = false;

        ret |= fork_process(pid_p, server_cmd, NULL);
        return ret;
}

/**
 * @brief    Text
 *
 * @return   int
 */
int run_node()
{
        // TODO: No need in both ret & status
        int ret = EXIT_SUCCESS;
        pid_t pid[2];
        int status;

        // FIXME: Race condition when client process is created before server.
        // Note that clients are supposed to talk to remote servers,
        // not necesserally the server launced from same node.
        // Thus, it might not considered as a BUG.
        ret |= run_server(&(pid[0]));
        ret |= run_client(&(pid[1]));

        // TODO: Handle 'status' before reusing it.
        waitpid(pid[0], &status, 0);
        waitpid(pid[1], &status, 0);
        return ret;
}

/**
 * @brief    Text
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 * @return   int
 */
int main(int argc, char **argv)
{
        // TODO: Manage return values. Also return values of cmd_line_parser module.
        get_cmds(argc, argv);
        run_node();
        return EXIT_SUCCESS;
}

// TODO:Add logger capabilities.
// TODO: Add shared memory between client & server optionally by user(user-decided or default size)
// TODO: Add config file support as input & parsed by parser. Config file will include all possibale params & if not specified by user, will resault in default config.
// TODO: Support multiline todos detection.