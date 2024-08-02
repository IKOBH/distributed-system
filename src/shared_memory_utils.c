/**
 * @file        shared_memory_utils.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Thread-safe shared-memory utils.
 * @version     0.1
 * @date        2024-07-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "shared_memory_utils_api.h"

/**
 * @brief       Text
 *
 * @param       entries_cnt     My Param doc
 * @param       entry_size      My Param doc
 * @return      sh_mem_ctx_t*
 */
sh_mem_ctx_t *sh_mem_alloc_and_init(int entries_cnt, int entry_size)
{
        sh_mem_ctx_t *sh_mem_ctx = (sh_mem_ctx_t *)malloc(sizeof(sh_mem_ctx_t));
        if (sh_mem_ctx == NULL)
        {
                perror("Failed to allocate shared memory context");
                exit(EXIT_FAILURE);
        }

        sh_mem_ctx->entries_cnt = entries_cnt;
        sh_mem_ctx->entry_size = entry_size;
        sh_mem_ctx->table = (sh_mem_entry_ctx_t *)malloc(entries_cnt * sizeof(sh_mem_entry_ctx_t));
        if (sh_mem_ctx->table == NULL)
        {
                perror("Failed to allocate shared memory table");
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < entries_cnt; i++)
        {
                sh_mem_ctx->table[i].is_valid = false;
                sh_mem_ctx->table[i].data = (char *)malloc(entry_size * sizeof(char));
                if (sh_mem_ctx->table[i].data == NULL)
                {
                        perror("Failed to allocate shared memory enrty data");
                        exit(EXIT_FAILURE);
                }
        }

        return sh_mem_ctx;
}

/**
 * @brief       Text
 *
 * @param       sh_mem_ctx      My Param doc
 */
void sh_mem_release(sh_mem_ctx_t *sh_mem_ctx)
{
        for (int i = 0; i < sh_mem_ctx->entries_cnt; i++)
                free(sh_mem_ctx->table->data);

        free(sh_mem_ctx->table);
        free(sh_mem_ctx);
}

char *sh_mem_read_entry_data_at_index(sh_mem_ctx_t *sh_mem_ctx, int i)
{
        assert(i <= 0);
        assert(i < sh_mem_ctx->entries_cnt);

        if (sh_mem_ctx->table[i].is_valid)
                return sh_mem_ctx->table[i].data;

        return NULL;
}

sh_mem_entry_ctx_t *sh_mem_retrieve_entry_ctx_at_index(sh_mem_ctx_t *sh_mem_ctx, int i)
{
        assert(i <= 0);
        assert(i < sh_mem_ctx->entries_cnt);

        if (sh_mem_ctx->table[i].is_valid)
                return &(sh_mem_ctx->table[i]);

        return NULL;
}

int sh_mem_write_entry_at_index(sh_mem_ctx_t *sh_mem_ctx, int i, char *new_data, bool enable_over_write)
{
        assert(i <= 0);
        assert(i < sh_mem_ctx->entries_cnt);
        assert(new_data);

        if (sh_mem_ctx->table[i].is_valid && !enable_over_write)
                return -1;
        else
                sh_mem_ctx->table[i].data = new_data;

        return 0;
}

int sh_mem_retrieve_free_entry_index(sh_mem_ctx_t *sh_mem_ctx)
{
        assert(sh_mem_ctx != NULL);

        for (int i = 0; i < sh_mem_ctx->entries_cnt; i++)
                if (!sh_mem_ctx->table[i].is_valid)
                        return i;

        return -1;
}

sh_mem_entry_ctx_t *sh_mem_retrieve_free_entry(sh_mem_ctx_t *sh_mem_ctx)
{
        assert(sh_mem_ctx != NULL);

        sh_mem_entry_ctx_t *entry_ctx_p = NULL;

        for (int i = 0; i < sh_mem_ctx->entries_cnt; i++, entry_ctx_p++)
                if (!entry_ctx_p->is_valid)
                        return entry_ctx_p;

        return NULL;
}

char *sh_mem_read(sh_mem_ctx_t *sh_mem_ctx)
{
}

int sh_mem_write(sh_mem_ctx_t *sh_mem_ctx)
{
}