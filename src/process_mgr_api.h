/**
 * @file        process_mgr_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <unistd.h>

// TODO: Get this value from user by counting the number of executables he configured in config_processes.yml
#define PROCESS_COUNT (2)

typedef struct process_cmd_ctx_t
{
        char **cmd_args;
        int arg_count;

} process_cmd_ctx_t;

typedef struct process_ctx_t
{
        char *executable_path;
        char *const args[];

} process_ctx_t;

typedef struct processes_ctx_t
{
        process_cmd_ctx_t *process_cmd_ctx;

} processes_ctx_t;

int run_processes(process_cmd_ctx_t processes_cmds_list[PROCESS_COUNT]);