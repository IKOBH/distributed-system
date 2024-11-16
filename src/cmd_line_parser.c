/**
 * @file     cmd_line_parser.c
 * @author   Shahar Avidar  (ikobh7@gmail.com)
 * @brief    Text
 * @version  0.1
 * @date     2024-07-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "cmd_line_parser_api.h"

#define DEFAULT_CONFIG_FILE_PATH ("../src/default_config.yml")

// TODO: Consider removing node_args struct?
typedef struct node_args_t
{
        char *config_file_path;
} node_args_t;

node_args_t node_args;

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"config_file", required_argument, 0, 'c'},
    {0, 0, 0, 0}};

/**
 * @brief    Text
 *
 * @param    prog_name           My Param doc
 */
static void parser_print_usage(const char *prog_name)
{
        printf("Desc:\t%s is a distributed peer to peer network skeleton capable of spinning both a client & a server.\n", prog_name);
        printf("\t It can run any custom client & server pair you wish for, as long as you supply the relevant executables & args.\n\n");
        printf("Usage:\t%s -c config_file_path [-h]\n\n", prog_name);
        printf("\t-c config_file_path \tSpecify the processes to be executed\n");
        printf("\t-h\t\t\tDisplay this help message and exit\n");
}

/**
 * @brief    Set the default command line argument values.
 *
 */
static void parser_set_default()
{
        node_args.config_file_path = DEFAULT_CONFIG_FILE_PATH;
}
/**
 * @brief    Text
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 */
void parser_parse_input(int argc, char **argv)
{
        int opt;

        parser_set_default();
        while ((opt = getopt_long(argc, argv, "hc:", long_options, NULL)) != -1)
        {
                switch (opt)
                {
                case 'h':
                        parser_print_usage(argv[0]);
                        exit(EXIT_SUCCESS);
                case 'c':
                        node_args.config_file_path = optarg;
                        break;
                default:
                        printf("\tFailed to find option.\n\n");
                        parser_print_usage(argv[0]);
                        exit(EXIT_FAILURE);
                }
        }
}

/**
 * @brief       Get the config file path objectText
 *
 * @return      char*
 */
char *parser_get_config_file_path()
{
        return node_args.config_file_path;
}