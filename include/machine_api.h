/**
 * @file        machine_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       Text
 * @version     0.1
 * @date        2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

typedef struct machine_control_block_t machine_cb_t;

/**
 * @brief       Text
 *
 * @param       config_file_path My Param doc
 * @return      int
 */
int machine_run(const char *config_file_path);

// TODO: API - start, stop, start_all, stop_all