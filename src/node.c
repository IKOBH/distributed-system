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
#include <stdbool.h>

#include "cmd_line_parser_api.h"
#include "yaml_parser_api.h"
#include "procs_mgr_api.h"
#include "networking_api.h"
#include "communication_utils_api.h"

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

#define INVALID_FD (-1)
#define CONFIG_FILE_PATH_NODE ("../src/config_node.yml")
#define CONFIG_FILE_PATH_PROCESSES_MGR ("../src/config_processes.yml")

typedef enum
{
        E_NODE_PROC_IDX_SERVER,
        E_NODE_PROC_IDX_CLIENT,
        E_NODE_PROC_IDX_COUNT
} node_proc_idx_t;

typedef enum
{
        // TODO: This list of cmds should be changed & ratified.
        E_NODE_CMD_TO_CLIENT,
        E_NODE_CMD_SERVER,
        E_NODE_CMD_COUNT
} node_cmd_t;

// communication_interface_t node_interaction_interface = {
//     .interact_loop = node_interact,
//     .get_input = comm_get_input,
//     .interpret_input = node_interpret_input,
//     .act_on_cmd = node_act_on_cmd};
typedef struct node_cmd_ctx_t
{
        int cmd;
        char *cmd_args;
} node_cmd_ctx_t;

typedef struct node_communication_ctx_t
{
        // TODO: Make use of 'comm_chan_t' pointer.
        chan_t channel;
        int input_fd;
        FILE *input_stream;
        char *input_buff;
        node_cmd_ctx_t *node_cmd_ctx;
        pipe_ctx_t **pipe_ctx_list;
} node_comm_ctx_t;

/**
 * @brief       Text
 *
 * @param       node_interaction_ctx My Param doc
 * @return      int
 */
int node_interpret_input(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;

        if (comm_chan_ctx->comm_chan->buffer == NULL)
                return -1;
        // TODO: Implement. (The following is not the actual logic required.)
        comm_chan_ctx->comm_cmd->cmd = E_NODE_CMD_TO_CLIENT;
        comm_chan_ctx->comm_cmd->cmd_args = comm_chan_ctx->comm_chan->buffer;

        return ret;
}

/**
 * @brief       Text
 *
 * @param       node_interaction_ctx My Param doc
 * @return      int
 */
static int node_send_to_client_cmd(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;
        FILE *client_fp;
        pipe_ctx_t **pipe_ctx_list_p = (pipe_ctx_t **)comm_chan_ctx->chan_specific_ctx;
        pipe_end_t pipe_end = pipe_ctx_list_p[E_NODE_PROC_IDX_CLIENT]->direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;

        if ((client_fp = fdopen(pipe_ctx_list_p[E_NODE_PROC_IDX_CLIENT]->pipe_fd[pipe_end], "w")) == NULL)
        {
                perror("Failed to open client's pipe");
                exit(EXIT_FAILURE);
        }

        if (!fputs(comm_chan_ctx->comm_cmd->cmd_args, client_fp))
        {
                if (feof(client_fp))
                        printf("End of file reached.\n");
                else if (ferror(client_fp))
                        perror("Error writing to client");
                else
                        perror("Unknown fputs error");

                ret = -1;
        }

        return ret;
}

/**
 * @brief       Text
 *
 * @param       node_interaction_ctx My Param doc
 * @return      int
 */
int node_act_on_cmd(comm_chan_ctx_t *comm_chan_ctx)
{
        int ret = 0;

        if (comm_chan_ctx->comm_chan->buffer == NULL)
                return -1;

        switch (comm_chan_ctx->comm_cmd->cmd)
        {
        case E_NODE_CMD_TO_CLIENT:
                ret |= node_send_to_client_cmd(comm_chan_ctx);
                break;

        default:
                printf("Failed: Command not found.");
                break;
        }

        return ret;
}

/**
 * @brief    Text
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 * @return   int
 */
int main(int argc, char **argv)
{
        processes_ctx_t processes_ctx;
        comm_chan_ctx_t *user_to_node_comm_ctx = comm_alloc_ctx(E_CHANNEL_PIPE, INPUT_BUFFER_BYTE_SIZE);
        comm_chan_ctx_t *node_to_client_comm_ctx = comm_alloc_ctx(E_CHANNEL_PIPE, OUTPUT_BUFFER_BYTE_SIZE);
        comm_init_ctx(user_to_node_comm_ctx, stdin);
        comm_init_ctx(node_to_client_comm_ctx, stdin);
        comm_if_t comm_if;
        comm_if.comm_if_interpret_input = node_interpret_input;
        comm_if.comm_if_act_on_cmd = node_act_on_cmd;

        // TODO: Manage return values. Also return values of cmd_line_parser module.
        get_cmds(argc, argv);
        // TODO: Get procs_cmds_list from user cmd line or config_appname.yml file.
        process_cmd_ctx_t procs_cmds_list[E_NODE_PROC_IDX_COUNT] = {{server_cmd, SERVER_ARG_COUNT},
                                                                    {client_cmd, CLIENT_ARG_COUNT}};

        //  TODO: Consider having CONFIG_FILE_PATH_NODE & CONFIG_FILE_PATH_PROCESSES_MGR recieved as input from user & not from macro.
        // yaml_load_file(CONFIG_FILE_PATH_PROCESSES_MGR, &processes_ctx);
        // yaml_load_file(CONFIG_FILE_PATH_NODE, &input_ctx);
        // TODO: Place comm_interact before procs_mgr_run & enable user ineraction to create processes.
        procs_mgr_run(procs_cmds_list, user_to_node_comm_ctx->chan_specific_ctx);
        comm_interact(&comm_if, user_to_node_comm_ctx);
        comm_release_ctx(node_to_client_comm_ctx);
        comm_release_ctx(user_to_node_comm_ctx);
        return EXIT_SUCCESS;
}

// TODO: Support non unix platforms (Consider using APR: https://apr.apache.org/)
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