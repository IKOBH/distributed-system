/**
 * @file        process_wrapper.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-10-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "yaml_parser_api.h"
#include "pipe_api.h"
#include "socket_api.h"

// yml_parser wrapper
const char *proc_alloc_executable_path(fy_node_t *process_node) { return yml_parser_alloc_executable_path(process_node); }
int proc_get_executable_args_cnt(fy_node_t *process_node) { return yml_parser_get_executable_args_cnt(process_node); }
char **proc_alloc_executable_args(fy_node_t *process_node) { return yml_parser_alloc_executable_args(process_node); }
int proc_get_pipe_count(fy_node_t *process_node) { return yml_parser_get_pipe_count(process_node); }
int proc_get_socket_count(fy_node_t *process_node) { return yml_parser_get_socket_count(process_node); }
fy_node_t *proc_get_socket(fy_node_t *process_node, int pos) { return yml_parser_get_socket(process_node, pos); }
fy_node_t *proc_get_pipe(fy_node_t *process_node, int pos) { return yml_parser_get_pipe(process_node, pos); }

// pipe wrapper
pipe_cb_t *proc_alloc_pipe_cb(fy_node_t *pipe_node) { return pipe_alloc_cb(pipe_node); }
void proc_release_pipe_cb(pipe_cb_t *pipe_cb) { pipe_release_cb(pipe_cb); }
void proc_pipe_ctx_init(pipe_cb_t *pipe_cb) { pipe_ctx_init(pipe_cb); }
void proc_pipe_handle_fork_failure(pipe_cb_t *pipe_ctx) { pipe_handle_fork_failure(pipe_ctx); }
void proc_pipe_handle_child_end(pipe_cb_t *pipe_ctx) { pipe_handle_child_end(pipe_ctx); }
void proc_pipe_handle_parent_end(pipe_cb_t *pipe_ctx) { pipe_handle_parent_end(pipe_ctx); }

// socket wrapper
sock_cb_t *proc_alloc_socket_cb(fy_node_t *socket_node) { return sock_alloc_cb(socket_node); }
void proc_release_socket_cb(sock_cb_t *sock_cb) { sock_release_cb(sock_cb); }
