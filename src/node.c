/**
 * @file     node.c
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

#include "cmd_line_parser_api.h"
#include "yaml_parser_api.h"
#include "process_mgr_api.h"
#include "client_api.h"
#include "server_api.h"

// TODO: Move handle_error_en & handle_error into some error_handler_api.h file & start using it.
#define handle_error_en(en, msg)    \
        do                          \
        {                           \
                errno = en;         \
                perror(msg);        \
                exit(EXIT_FAILURE); \
        } while (0)

#define handle_error(msg)           \
        do                          \
        {                           \
                perror(msg);        \
                exit(EXIT_FAILURE); \
        } while (0)

#define CONFIG_FILE_PATH_NODE ("../src/config_node.yml")
#define CONFIG_FILE_PATH_PROCESSES_MGR ("../src/config_processes.yml")
typedef struct node_ctx_t
{
        client_ctx_t *client_ctx;
        server_ctx_t *server_ctx;
} node_ctx_t;

typedef struct input_ctx_t
{
        node_ctx_t *node_ctx_p;
        client_ctx_t *client_ctx_p;
        server_ctx_t *server_ctx_p;
        connection_ctx_t *connection_ctx_p;
        struct sockaddr_in *sockaddr_in_p;
        struct sin_addr *sin_addr_p;
        connect_retry_ctx_t *connect_retry_ctx_t_p;
} input_ctx_t;

/**
 * @brief    Text
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 * @return   int
 */
int main(int argc, char **argv)
{
        input_ctx_t input_ctx;
        processes_ctx_t processes_ctx;
        // TODO: Get processes_cmds_list from user cmd line or config_appname.yml file.
        process_cmd_ctx_t processes_cmds_list[PROCESS_COUNT] = {{server_cmd, SERVER_ARG_COUNT},
                                                                {client_cmd, CLIENT_ARG_COUNT}};

        // TODO: Manage return values. Also return values of cmd_line_parser module.
        get_cmds(argc, argv);

        //  TODO: Consider having CONFIG_FILE_PATH_NODE & CONFIG_FILE_PATH_PROCESSES_MGR recieved as input from user & not from macro.
        yaml_load_file(CONFIG_FILE_PATH_PROCESSES_MGR, &processes_ctx);
        yaml_load_file(CONFIG_FILE_PATH_NODE, &input_ctx);
        run_processes(processes_cmds_list);
        return EXIT_SUCCESS;
}

// TODO: Support non unix platforms.
// TODO: Add support for all kind of communication protocols (Not only IPv4)
// TODO: Add logger capabilities.
// TODO: Add shared memory between client & server optionally by user(user-decided or default size)
// TODO: Add config file support as input & parsed by parser. Config file will include all possibale params & if not specified by user, will resault in default config.
// TODO: Support multiline todos detection.
// TODO: Add unit-testing framework.

// TODO: Decouple process utilities from node(client-server) front-end app.
//       Use process_utils as a seperate lib that can spawn any kind of process.
//       Add config_process.yml to support process behaviour.
// TODO: Use doxygen to upload documentation online.