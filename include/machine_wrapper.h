/**
 * @file        machine_wrapper.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <stdio.h>

/* opaque types for the user */
typedef struct fy_node yml_node_t;
typedef struct process_control_block_t proc_cb_t;

// yml_parser wrapper
int machine_get_process_count(FILE *config_file);
yml_node_t *machine_get_process(FILE *config_file, int pos);

// process wrapper
proc_cb_t *machine_alloc_process_cb(yml_node_t *process_node);
void machine_release_process_cb(proc_cb_t *proc_cb);
int machine_process_get_pid(proc_cb_t *proc_cb);
int machine_fork_process(proc_cb_t *proc_cb);