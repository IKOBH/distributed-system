/**
 * @file     pipe_api.h
 * @author   Shahar Avidar  (ikobh7@gmail.com)
 * @brief    Text
 * @version  0.1
 * @date     2024-07-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <stdio.h>

typedef struct pipe_control_block_t pipe_cb_t;
// TODO: This couples yml_parser module with 'pipe' module. Need to decouple it.
typedef struct fy_node yml_node_t;

/**
 * @brief       Text
 *
 * @param       pipe_ctx        My Param doc
 */
void pipe_ctx_user_init(pipe_cb_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_ctx_init(pipe_cb_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_handle_child_end(pipe_cb_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void handle_after_use_child_pipe_end(pipe_cb_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_handle_parent_end(pipe_cb_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_handle_fork_failure(pipe_cb_t *pipe_ctx);

/**
 * @brief    Text
 *
 * @param    pipe_ctx            My Param doc
 */
void pipe_ctx_exit(pipe_cb_t *pipe_ctx);

/**
 * @brief       Text
 *
 * @param       pipe_node       My Param doc
 * @return      pipe_cb_t*
 */
pipe_cb_t *pipe_alloc_cb(yml_node_t *pipe_node);

/**
 * @brief       Text
 *
 * @param       pipe_cb         My Param doc
 */
void pipe_release_cb(pipe_cb_t *pipe_cb);