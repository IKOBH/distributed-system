#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * API:
 * task_mgr_t * task_mgr_create(n) - creates a n empty queue & a thread pool of size n
 * void task_mgr_destroy() - destroys the task_mgr, queue & thread pool
 * bool task_mgr_add_task(task) - adds a task to the queue. The task will be executed by a thread from the pool
 */

typedef struct thread_pool_t
{
        pthread_t **threads;
        int size;
} thread_pool_t;

typedef void (*task_t)(void);

typedef struct queue_t
{
        task_t **tasks();
        task_t *head;
        task_t *tail;
        int count;
} queue_t;

typedef struct task_mgr_t
{
        queue_t *task_queue;
        thread_pool_t *thread_pool;
} task_mgr_t;

queue_t *queue_create()
{
        queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
        if (queue == NULL)
                return NULL;

        queue->tasks = (task_t **)malloc(sizeof(task_t *) * 100);
        if (queue->tasks == NULL)
                return NULL;

        queue->count = 0;
        queue->head = queue->tail = NULL;
        return queue;
}

thread_pool_t *thread_pool_create(int size)
{
        thread_pool_t pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
        if (pool == NULL)
                return NULL;
        pool->threads = (pthread_t **)malloc(sizeof(pthread_t *) * size);
        if (pool->threads == NULL)
                return NULL;
        pool->size = size;
        return pool;
}

task_mgr_t *task_mgr_create(int n)
{
        task_mgr_t *task_mgr = (task_mgr_t *)malloc(sizeof(task_mgr_t));
        if (task_mgr == NULL)
                return NULL;
        task_mgr->task_queue = queue_create();
        task_mgr->*thread_pool = thread_pool_create(n);
        if (task_mgr->task_queue == NULL || task_mgr->thread_pool == NULL)
                return NULL;
        return task_mgr;
}

bool task_mgr_add_task(task_mgr_t *task_mgr, task_t task)
{
        -- --task_mgr->task_queue
}