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
static int procs_mgr_fork(pid_t *pid_p, char *const args[], pipe_ctx_t *pipe_ctx)
{
        // TODO: Add support for multi-piping.
        pipe_ctx_user_init(pipe_ctx);
        pipe_ctx_init(pipe_ctx);
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
                printf("DEBUG: %s\n", args[0]);
                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }
        handle_parent_pipe_end(pipe_ctx);
        return EXIT_SUCCESS;
}

/**
 * @brief       Text
 *
 * @return      pipe_ctx_t**
 */
pipe_ctx_t **procs_mgr_alloc_pipes_ctxs()
{
        pipe_ctx_t **pipe_ctx_list;
        pipe_ctx_list = (pipe_ctx_t **)malloc(PROCESS_COUNT * sizeof(pipe_ctx_t));
        if (pipe_ctx_list == NULL)
        {
                perror("NULL POINTER pipe_ctx_list");
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < PROCESS_COUNT; i++)
        {
                pipe_ctx_list[i] = (pipe_ctx_t *)malloc(sizeof(pipe_ctx_t));
                if (pipe_ctx_list[i] == NULL)
                {
                        perror("NULL POINTER pipe_ctx_list[i]");
                        exit(EXIT_FAILURE);
                }
        }

        return pipe_ctx_list;
}

/**
 * @brief       Text
 *
 * @param       pipe_ctx_list   My Param doc
 */
void procs_mgr_release_pipes_ctxs(pipe_ctx_t **pipe_ctx_list)
{
        for (int i = 0; i < PROCESS_COUNT; i++)
        {
                pipe_ctx_exit(pipe_ctx_list[i]);
                free(pipe_ctx_list[i]);
        }

        free(pipe_ctx_list);
}

/**
 * @brief       Text
 *
 * @param       processes_cmds_list My Param doc
 * @param       pipe_ctx_list   My Param doc
 * @return      int
 */
int procs_mgr_run(process_cmd_ctx_t processes_cmds_list[PROCESS_COUNT], pipe_ctx_t **pipe_ctx_list)
{
        // TODO: No need in both ret & status
        int ret;
        pid_t pids[PROCESS_COUNT];
        int status;
        int i;

        // FIXME: Race condition when client process is created before server.
        // Note that clients are supposed to talk to remote servers,
        // not necesserally the server launced from same node.
        // Thus, it might not considered as a BUG.
        for (i = 0; i < PROCESS_COUNT; i++)
                ret |= procs_mgr_fork(&(pids[i]), processes_cmds_list[i].cmd_args, pipe_ctx_list[i]);

        // TODO: Handle 'status' before reusing it.
        for (i = 0; i < PROCESS_COUNT; i++)
                waitpid(pids[i], &status, 0);

        return ret;
}