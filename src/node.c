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

typedef enum
{
        E_NODE_PROC_IDX_SERVER,
        E_NODE_PROC_IDX_CLIENT,
        E_NODE_PROC_IDX_COUNT
} node_proc_idx_t;

typedef enum
{
        // TODO: This list of cmds should be changed & ratified.
        E_NODE_CMD_CLIENT,
        E_NODE_CMD_SERVER,
        E_NODE_CMD_COUNT
} node_cmd_t;

typedef struct node_cmd_ctx_t
{
        int cmd;
        char *cmd_args;
} node_cmd_ctx_t;

/**
 * @brief       Text
 *
 * @param       input_buff      My Param doc
 * @return      int
 */
static int node_get_input(char *input_buff)
{
        if (!fgets(input_buff, SEND_BUFFER_BYTE_SIZE, stdin))
        {
                if (feof(stdin))
                        printf("End of file reached.\n");
                else if (ferror(stdin))
                        perror("Error reading from input");
                else
                        perror("Unknown fgets error");

                return -1;
        }
        return 0;
}

/**
 * @brief       Text
 *
 * @param       input_buff      My Param doc
 * @param       cmd             My Param doc
 * @return      int
 */
int node_interpret_input(char *input_buff, node_cmd_ctx_t *cmd)
{
        // TODO: Implement. (The following is not the actual logic required.)
        cmd->cmd = E_NODE_CMD_CLIENT;
        cmd->cmd_args = input_buff;
}

/**
 * @brief       Text
 *
 * @param       cmd_args        My Param doc
 * @param       pipe_ctx        My Param doc
 */
void node_act_on_client_cmd(char *cmd_args, pipe_ctx_t *pipe_ctx)
{
        FILE *client_fp;
        pipe_end_t pipe_end = pipe_ctx->direction ? E_PIPE_END_READ : E_PIPE_END_WRITE;

        if ((client_fp = fdopen(pipe_ctx->pipe_fd[pipe_end], "w")) == NULL)
        {
                perror("Failed to open client's pipe");
                exit(EXIT_FAILURE);
        }

        if (!fputs(cmd_args, client_fp))
        {
                if (feof(client_fp))
                        printf("End of file reached.\n");
                else if (ferror(client_fp))
                        perror("Error writing to client");
                else
                        perror("Unknown fputs error");
        }
}

/**
 * @brief       Text
 *
 * @param       cmd             My Param doc
 * @param       pipe_ctx_list   My Param doc
 */
void node_act_on_cmd(node_cmd_ctx_t *cmd, pipe_ctx_t **pipe_ctx_list)
{
        switch (cmd->cmd)
        {
        case E_NODE_CMD_CLIENT:
                node_act_on_client_cmd(cmd->cmd_args, pipe_ctx_list[E_NODE_PROC_IDX_CLIENT]);
                break;

        default:
                printf("Failed: Command not found.");
                break;
        }
}

/**
 * @brief       Text
 *
 * @param       pipe_ctx_list   My Param doc
 */
static void node_interact(pipe_ctx_t **pipe_ctx_list)
{
        char input_buff[SEND_BUFFER_BYTE_SIZE];
        node_cmd_ctx_t cmd;

        while (true)
        {
                printf("> ");
                if (node_get_input(input_buff) == -1)
                        break;
                node_interpret_input(input_buff, &cmd);
                node_act_on_cmd(&cmd, pipe_ctx_list);
        }
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
        pipe_ctx_t **pipe_ctx_list = procs_mgr_alloc_pipes_ctxs();

        // TODO: Manage return values. Also return values of cmd_line_parser module.
        get_cmds(argc, argv);
        // TODO: Get processes_cmds_list from user cmd line or config_appname.yml file.
        process_cmd_ctx_t processes_cmds_list[E_NODE_PROC_IDX_COUNT] = {{server_cmd, SERVER_ARG_COUNT},
                                                                        {client_cmd, CLIENT_ARG_COUNT}};

        //  TODO: Consider having CONFIG_FILE_PATH_NODE & CONFIG_FILE_PATH_PROCESSES_MGR recieved as input from user & not from macro.
        // yaml_load_file(CONFIG_FILE_PATH_PROCESSES_MGR, &processes_ctx);
        // yaml_load_file(CONFIG_FILE_PATH_NODE, &input_ctx);
        // TODO: Place node_interact before procs_mgr_run & enable user ineraction to create processes.
        procs_mgr_run(processes_cmds_list, pipe_ctx_list);
        node_interact(pipe_ctx_list);
        procs_mgr_release_pipes_ctxs(pipe_ctx_list);
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