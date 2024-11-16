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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "process_wrapper.h"
#include "str_utils_api.h"

typedef struct process_control_block_t
{
        pid_t pid;
        char *executable_path;
        int executable_args_cnt;
        char **executable_args;
        int pipe_cnt;
        pipe_cb_t **pipes;
        int socket_cnt;
        sock_cb_t **sockets;
} proc_cb_t;

/**
 * @brief       Text
 *
 * @param       yml_node        My Param doc
 * @return      proc_cb_t*
 */
proc_cb_t *proc_alloc_cb(yml_node_t *yml_node)
{

        proc_cb_t *proc_cb = (proc_cb_t *)malloc(sizeof(proc_cb_t));
        if (proc_cb == NULL)
        {
                perror("NULL POINTER process_cb");
                exit(EXIT_FAILURE);
        }

        proc_cb->executable_path = (char *)proc_alloc_executable_path(yml_node);
        if (proc_cb->executable_path == NULL)
        {
                perror("NULL POINTER process_cb->executable_path");
                exit(EXIT_FAILURE);
        }

        proc_cb->executable_args_cnt = proc_get_executable_args_cnt(yml_node);
        proc_cb->executable_args = proc_alloc_executable_args(yml_node);
        if (proc_cb->executable_args == NULL)
        {
                perror("NULL POINTER process_cb->executable_args");
                exit(EXIT_FAILURE);
        }

        // PIPES
        proc_cb->pipe_cnt = proc_get_pipe_count(yml_node);
        proc_cb->pipes = (pipe_cb_t **)malloc(proc_cb->pipe_cnt * sizeof(pipe_cb_t *));
        if (proc_cb->pipes == NULL && proc_cb->pipe_cnt > 0)
        {
                perror("NULL POINTER process_cb->pipes");
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < proc_cb->pipe_cnt; i++)
        {
                proc_cb->pipes[i] = proc_alloc_pipe_cb(proc_get_pipe(yml_node, i));
        }

        // SOCKETS
        proc_cb->socket_cnt = proc_get_socket_count(yml_node);
        proc_cb->sockets = (sock_cb_t **)malloc(proc_cb->socket_cnt * sizeof(sock_cb_t *));
        if (proc_cb->sockets == NULL && proc_cb->socket_cnt > 0)
        {
                perror("NULL POINTER process_cb->sockets");
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < proc_cb->socket_cnt; i++)
        {
                proc_cb->sockets[i] = proc_alloc_socket_cb(proc_get_socket(yml_node, i));
        }

        return proc_cb;
}

/**
 * @brief       Text
 *
 * @param       proc_cb      My Param doc
 */
void proc_release_cb(proc_cb_t *proc_cb)
{
        for (int i = 0; i < proc_cb->pipe_cnt; i++)
        {
                proc_release_pipe_cb(proc_cb->pipes[i]);
        }
        free(proc_cb->pipes);

        for (int i = 0; i < proc_cb->socket_cnt; i++)
        {
                proc_release_socket_cb(proc_cb->sockets[i]);
        }
        free(proc_cb->sockets);

        free(proc_cb->executable_path);
        for (int i = 0; i < proc_cb->executable_args_cnt; i++)
        {
                free(proc_cb->executable_args[i]);
        }
        free(proc_cb->executable_args);
        free(proc_cb);
}

/**
 * @brief       Text
 *
 * @param       proc_cb      My Param doc
 * @return      int
 */
int proc_fork(proc_cb_t *proc_cb)
{
        for (int i = 0; i < proc_cb->pipe_cnt; i++)
                proc_pipe_ctx_init(proc_cb->pipes[i]);
        proc_cb->pid = fork();

        if (proc_cb->pid == -1)
        {
                perror("Failed to fork process");
                for (int i = 0; i < proc_cb->pipe_cnt; i++)
                        proc_pipe_handle_fork_failure(proc_cb->pipes[i]);
                exit(EXIT_FAILURE);
        }
        else if (proc_cb->pid == 0)
        {
                for (int i = 0; i < proc_cb->pipe_cnt; i++)
                        proc_pipe_handle_child_end(proc_cb->pipes[i]);
                execvp(proc_cb->executable_path, proc_cb->executable_args);
                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }
        for (int i = 0; i < proc_cb->pipe_cnt; i++)
                proc_pipe_handle_parent_end(proc_cb->pipes[i]);
        return EXIT_SUCCESS;
}

/**
 * @brief       Text
 *
 * @param       proc_cb      My Param doc
 * @return      int
 */
int proc_get_pid(proc_cb_t *proc_cb) { return proc_cb->pid; }