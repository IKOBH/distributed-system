/**
 * @file        shared_memory_utils_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-07-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <stdbool.h>

typedef struct shared_memory_entry_ctx_t
{
        char *data;
        bool is_valid;
} sh_mem_entry_ctx_t;

typedef struct shared_memory_ctx_t
{
        // TODO: Consider having 'table' to be of type 'sh_mem_entry_ctx_t **'
        //       This enables flexiable, async, big tables to perform well.
        //       This change will result in modification to almost all 'sh_mem' module functions.
        sh_mem_entry_ctx_t *table;
        int entry_size; // BYTES
        int entries_cnt;
        sh_mem_entry_ctx_t *read_head;
        sh_mem_entry_ctx_t *write_head;
} sh_mem_ctx_t;
