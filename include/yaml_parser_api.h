/**
 * @file        yaml_parser_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdio.h>

typedef struct fy_node fy_node_t;

/**
 * @brief       Construct a new yml parser get process count objectText
 *
 * @param       config_file     My Param doc
 * @return      int
 */
int yml_parser_get_process_count(FILE *config_file);

/**
 * @brief       Memory Allocation for the executable arguments.
 *              User must free the returned pointer.
 *
 * @param       process_node    My Param doc
 * @return      const char*
 */
const char *yml_parser_alloc_executable_path(fy_node_t *process_node);

/**
 * @brief       Get the number of executable arguments.
 *
 * @param       process_node    My Param doc
 * @return      int
 */
int yml_parser_get_executable_args_cnt(fy_node_t *process_node);

/**
 * @brief       Memory Allocation for the executable arguments.
 *              User must free the returned pointer.
 *
 * @param       process_node    My Param doc
 * @return      char**
 */
char **yml_parser_alloc_executable_args(fy_node_t *process_node);

/**
 * @brief       Text
 *
 * @param       process_node    My Param doc
 * @return      int
 */
int yml_parser_get_pipe_count(fy_node_t *process_node);

/**
 * @brief       Text
 *
 * @param       process_node    My Param doc
 * @param       pos             My Param doc
 * @return      struct fy_node*
 */
struct fy_node *yml_parser_get_pipe(fy_node_t *process_node, int pos);

/**
 * @brief       Text
 *
 * @param       process_node    My Param doc
 * @return      int
 */
int yml_parser_get_socket_count(fy_node_t *process_node);

/**
 * @brief       Text
 *
 * @param       process_node    My Param doc
 * @param       pos             My Param doc
 * @return      struct fy_node*
 */
struct fy_node *yml_parser_get_socket(fy_node_t *process_node, int pos);

/**
 * @brief       Memory Allocation for the channel direction.
 *              User must free the returned pointer.
 *
 * @param       chan_node       My Param doc
 * @return      int
 */
const char *yml_parser_get_channel_direction(struct fy_node *chan_node);

/**
 * @brief       Text
 *
 * @param       config_file     My Param doc
 * @param       pos             My Param doc
 * @return      void*
 */
struct fy_node *yml_parser_get_process(FILE *config_file, int pos);
