/**
 * @file        machine.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "machine_wrapper.h"

typedef struct machine_control_block_t
{
        proc_cb_t **processes;
        int process_cnt;

} machine_cb_t;

/**
 * @brief       Text
 *
 * @param       config_file_path My Param doc
 * @return      machine_cb_t*
 */
machine_cb_t *machine_alloc_cb(const char *config_file_path)
{
        FILE *config_file = fopen(config_file_path, "r");
        if (config_file == NULL)
        {
                perror("Error opening config_file");
                exit(EXIT_FAILURE);
        }

        machine_cb_t *machine_cb = (machine_cb_t *)malloc(sizeof(machine_cb_t));
        if (machine_cb == NULL)
        {
                perror("NULL POINTER machine_cb");
                exit(EXIT_FAILURE);
        }

        machine_cb->process_cnt = machine_get_process_count(config_file);
        machine_cb->processes = (proc_cb_t **)malloc(machine_cb->process_cnt * sizeof(proc_cb_t *));
        if (machine_cb->processes == NULL)
        {
                perror("NULL POINTER machine_cb->processes");
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < machine_cb->process_cnt; i++)
        {
                machine_cb->processes[i] = machine_alloc_process_cb(machine_get_process(config_file, i));
        }

        if (fclose(config_file) != 0)
        {
                perror("Error closing config_file");
                exit(EXIT_FAILURE);
        }

        return machine_cb;
}

/**
 * @brief       Text
 *
 * @param       machine_cb      My Param doc
 */
void machine_release_cb(machine_cb_t *machine_cb)
{
        for (int i = 0; i < machine_cb->process_cnt; i++)
        {
                machine_release_process_cb(machine_cb->processes[i]);
        }

        free(machine_cb->processes);
        free(machine_cb);
}

/**
 * @brief       Text
 *
 * @param       processes_cmds_list My Param doc
 * @param       pipe_ctx_list   My Param doc
 * @return      int
 */
int machine_run(const char *config_file_path)
{
        machine_cb_t *machine_cb = machine_alloc_cb(config_file_path);
        // TODO: No need in both ret & status
        int ret;
        int status;

        for (int i = 0; i < machine_cb->process_cnt; i++)
                ret |= machine_fork_process(machine_cb->processes[i]);

        // TODO: Handle 'status' before reusing it.
        for (int i = 0; i < machine_cb->process_cnt; i++)
                waitpid(machine_process_get_pid(machine_cb->processes[i]), &status, 0);

        machine_release_cb(machine_cb);

        return ret;
}