/**
 * @file     pipe_utils_api.h
 * @author   Shahar Avidar  (ikobh7@gmail.com)
 * @brief    Text
 * @version  0.1
 * @date     2024-07-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "communication_utils_api.h"

typedef enum
{
        E_PIPE_STATE_UNINITIALIZED,
        E_PIPE_STATE_USER_INITIALIZED,
        E_PIPE_STATE_INITIALIZED,
        E_PIPE_STATE_CHILD_READY_TO_USE,
        E_PIPE_STATE_CHILD_DONE,
        E_PIPE_STATE_PARENT_READY_TO_USE,
        E_PIPE_STATE_PARENT_DONE,
        E_PIPE_STATE_FORK_FAILURE
} pipe_state_t;
// TODO: Delete enum after config.yml file is ready.
//       All user defined constansts should be exported to config.yml.
//       On second thougt - enums are compiled time constants & yml is not.
//       Need to rethink it.
typedef enum
{
        E_PIPE_END_READ,
        E_PIPE_END_WRITE
} pipe_end_t;

typedef enum
{
        E_PIPE_DIR_PARENT_TO_CHILD, /*Parent write, child reads.*/
        E_PIPE_DIR_CHILD_TO_PARENT  /*Child write, parent reads.*/
} pipe_direction_t;

typedef struct pipe_ctx_t
{
        int pipe_fd[2];
        pipe_state_t state;
        pipe_direction_t direction;
} pipe_ctx_t;

typedef struct pipe_channel_t
{
        comm_chan_t *pipe_chan;
        pipe_ctx_t *pipe_ctx;
} pipe_chan_t;

/**
 * @brief       Text
 *
 * @param       pipe_ctx        My Param doc
 */
void pipe_ctx_user_init(pipe_ctx_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_ctx_init(pipe_ctx_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_child_pipe_end(pipe_ctx_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_after_use_child_pipe_end(pipe_ctx_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_parent_pipe_end(pipe_ctx_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_pipe_fork_failure(pipe_ctx_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_ctx_exit(pipe_ctx_t *pipe_ctx);