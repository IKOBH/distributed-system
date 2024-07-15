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

#define SERVER_INPUT_ARG (1000)
#define CLIENT_INPUT_ARG (1001)

#define SERVER_ARG_COUNT (3)
#define CLIENT_ARG_COUNT (3)

char *server_cmd[SERVER_ARG_COUNT];
char *client_cmd[CLIENT_ARG_COUNT];

typedef struct client_args_t
{
        char *executable;
        char *input;
} client_args_t;

typedef struct server_args_t
{
        char *executable;
        char *input;
} server_args_t;
typedef struct node_args_t
{
        server_args_t server_args;
        client_args_t client_args;
} node_args_t;

static struct node_args_t node_args;

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"server_executable", required_argument, 0, 's'},
    {"server_args", required_argument, 0, SERVER_INPUT_ARG},
    {"client_executable", required_argument, 0, 'c'},
    {"client_args", required_argument, 0, CLIENT_INPUT_ARG},
    {0, 0, 0, 0}};

/**
 * @brief    Text
 *
 * @param    prog_name           My Param doc
 */
static void print_usage(const char *prog_name)
{
        printf("Desc:\t%s is a distributed peer to peer network skeleton capable of spinning both a client & a server.\n", prog_name);
        printf("\t It can run any custom client & server pair you wish for, as long as you supply the relevant executables & args.\n\n");
        printf("Usage:\t%s -s server executable [--server_args] -c client executable [--client_args] [-h]\n\n", prog_name);
        printf("\t-s server executable\tSpecify the server executable\n");
        printf("\t--server_args server arguments\tSpecify the server arguments\n");
        printf("\t-c client executable\tSpecify the client executable\n");
        printf("\t--client_args client arguments\tSpecify the client arguments\n");
        printf("\t-h\t\t\tDisplay this help message and exit\n");
}

/**
 * @brief    Text
 *
 */
static void verify_required_args()
{
        // TODO: Implement.
}

/**
 * @brief    Text
 *
 */
static void build_cmds()
{
        server_cmd[0] = node_args.server_args.executable;
        server_cmd[1] = node_args.server_args.input;
        server_cmd[2] = NULL;
        client_cmd[0] = node_args.client_args.executable;
        client_cmd[1] = node_args.client_args.input;
        client_cmd[2] = NULL;
}

/**
 * @brief    Set the default command line argument values.
 *
 */
void set_default()
{
        node_args.server_args.executable = "./server";
        node_args.server_args.input = "";
        node_args.client_args.executable = "./client";
        node_args.client_args.input = "";
}
/**
 * @brief    Text
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 */
static void parse_input(int argc, char **argv)
{
        int opt;

        set_default();
        while ((opt = getopt_long(argc, argv, "hs:c:", long_options, NULL)) != -1)
        {
                switch (opt)
                {
                case 'h':
                        print_usage(argv[0]);
                        exit(EXIT_SUCCESS);
                case 's':
                        node_args.server_args.executable = optarg;
                        break;
                case SERVER_INPUT_ARG:
                        node_args.server_args.input = optarg;
                        break;
                case 'c':
                        node_args.client_args.executable = optarg;
                        break;
                case CLIENT_INPUT_ARG:
                        node_args.client_args.input = optarg;
                        break;
                default:
                        printf("\tFailed to find option.\n\n");
                        print_usage(argv[0]);
                        exit(EXIT_FAILURE);
                }
        }
}

/**
 * @brief    Get the cmds objectText
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 */
void get_cmds(int argc, char **argv)
{
        parse_input(argc, argv);
        verify_required_args();
        build_cmds();
}