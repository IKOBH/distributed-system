/**
 * @file        machine_wrapper.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>

#include "yaml_parser_api.h"
#include "process_api.h"

// yml_parser wrapper
int machine_get_process_count(FILE *config_file) { return yml_parser_get_process_count(config_file); }
fy_node_t *machine_get_process(FILE *config_file, int pos) { return yml_parser_get_process(config_file, pos); }

// process wrapper
proc_cb_t *machine_alloc_process_cb(fy_node_t *process_node) { return proc_alloc_cb(process_node); }
void machine_release_process_cb(proc_cb_t *process_cb) { proc_release_cb(process_cb); }
int machine_process_get_pid(proc_cb_t *process_cb) { return proc_get_pid(process_cb); }
int machine_fork_process(proc_cb_t *process_cb) { return proc_fork(process_cb); }