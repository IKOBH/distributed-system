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

/**
 * @brief       Text
 *
 * @param       argc            My Param doc
 * @param       argv            My Param doc
 */

void parser_parse_input(int argc, char **argv);

/**
 * @brief       Get the config file path objectText
 *
 * @return      char*
 */
char *parser_get_config_file_path();