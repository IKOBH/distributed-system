/**
 * @file        process_mgr.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include "process_mgr_api.h"
#include "pipe_utils_api.h"

typedef struct input_ctx_t
{
        pipe_ctx_t *pipe_ctx;
} input_ctx_t;

/**
 * @brief    Text
 *
 * @param    pid_p               My Param doc
 * @param    args                My Param doc
 * @param    pipe_ctx            My Param doc
 * @return   int
 */
static int fork_process(pid_t *pid_p, char *const args[])
{
        // TODO: Make pipe_ctx a pointer & use malloc\free instead in get_pipe_ctx_from_user according to user's input.
        //       This will also open the path to support different pipe configs to different processes.
        //       Currently, all processes share the same pipe config.
        pipe_ctx_t pipe_ctx;

        // TODO: Add support for multi-piping.
        pipe_ctx_user_init(&pipe_ctx);
        pipe_ctx_init(&pipe_ctx);
        *pid_p = fork();

        if (*pid_p == -1)
        {
                perror("Failed to fork process");

                handle_pipe_fork_failure(&pipe_ctx);
                exit(EXIT_FAILURE);
        }
        else if (*pid_p == 0)
        {

                handle_child_pipe_end(&pipe_ctx);
                execvp(args[0], args);
                printf("DEBUG: %s\n", args[0]);
                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }
        handle_parent_pipe_end(&pipe_ctx);
        pipe_ctx_exit(&pipe_ctx);

        return EXIT_SUCCESS;
}

/**
 * @brief    Text
 *
 * @return   int
 */
int run_processes(process_cmd_ctx_t processes_cmds_list[PROCESS_COUNT])
{
        // TODO: No need in both ret & status
        int ret = EXIT_SUCCESS;
        pid_t pids[PROCESS_COUNT];
        int status;
        int i;

        // FIXME: Race condition when client process is created before server.
        // Note that clients are supposed to talk to remote servers,
        // not necesserally the server launced from same node.
        // Thus, it might not considered as a BUG.
        for (i = 0; i < PROCESS_COUNT; i++)
        {
                ret |= fork_process(&(pids[i]), processes_cmds_list[i].cmd_args);
        }

        // TODO: Handle 'status' before reusing it.
        for (i = 0; i < PROCESS_COUNT; i++)
                waitpid(pids[i], &status, 0);

        return ret;
}