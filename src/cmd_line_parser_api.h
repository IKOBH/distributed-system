/**
 * @file     cmd_line_parser_api.h
 * @author   Shahar Avidar  (ikobh7@gmail.com)
 * @brief    Text
 * @version  0.1
 * @date     2024-07-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

extern char *server_cmd[];
extern char *client_cmd[];

/**
 * @brief    Get the cmds objectText
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 */
void get_cmds(int argc, char **argv);