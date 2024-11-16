/**
 * @file        process_wrapper.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-10-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

/* opaque types for the user */
typedef struct fy_node yml_node_t;
typedef struct pipe_control_block_t pipe_cb_t;
typedef struct socket_control_block_t sock_cb_t;

// yml_parser wrapper
const char *proc_alloc_executable_path(yml_node_t *process_node);
int proc_get_executable_args_cnt(yml_node_t *process_node);
char **proc_alloc_executable_args(yml_node_t *process_node);
int proc_get_pipe_count(yml_node_t *process_node);
yml_node_t *proc_get_pipe(yml_node_t *process_node, int pos);
yml_node_t *proc_get_socket(yml_node_t *process_node, int pos);

// pipe wrapper
pipe_cb_t *proc_alloc_pipe_cb(yml_node_t *pipe_node);
void proc_release_pipe_cb(pipe_cb_t *pipe_cb);
void proc_pipe_ctx_init(pipe_cb_t *pipe_cb);
void proc_pipe_handle_fork_failure(pipe_cb_t *pipe_ctx);
void proc_pipe_handle_child_end(pipe_cb_t *pipe_ctx);
void proc_pipe_handle_parent_end(pipe_cb_t *pipe_ctx);

// socket wrapper
int proc_get_socket_count(yml_node_t *socket_node);
sock_cb_t *proc_alloc_socket_cb(yml_node_t *socket_node);
void proc_release_socket_cb(sock_cb_t *socket_cb);
