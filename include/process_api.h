/**
 * @file        process_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-10-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

typedef struct process_control_block_t proc_cb_t;
// TODO: This couples yml_parser module with "pipe" module. Need to decouple it.
typedef struct fy_node yml_node_t;

proc_cb_t *proc_alloc_cb(yml_node_t *yml_node);
void proc_release_cb(proc_cb_t *process_cb);
int proc_fork(proc_cb_t *process_cb);
int proc_get_pid(proc_cb_t *process_cb);
