/**
 * @file        yaml_parser.c
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       yaml_parser expose an easy API for config initialization.
 *              It aims to reduce code dependencies & build times by enabling simple export of user-defined configurations,
 *              into a yaml file, then easily instantianting structs using a config.yml file.
 * @dependecies libyaml, str_utils
 * @version     0.1
 * @date        2024-07-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libfyaml.h>

#include "yaml_parser_api.h"
#include "str_utils_api.h"

static struct fy_document *get_fyd_from_file_pointer(FILE *config_file)
{
        static struct fy_document *fyd = NULL;

        if (fyd == NULL)
        {
                fyd = fy_document_build_from_fp(NULL, config_file);
                if (!fyd)
                {
                        perror("Failed to build document from file");
                        exit(EXIT_FAILURE);
                }
        }

        return fyd;
}

int yml_parser_get_process_count(FILE *config_file)
{
        struct fy_document *fyd = get_fyd_from_file_pointer(config_file);

        return fy_node_sequence_item_count(fy_document_root(fyd));
}

struct fy_node *yml_parser_get_process(FILE *config_file, int pos)
{
        struct fy_document *fyd = get_fyd_from_file_pointer(config_file);

        return fy_node_sequence_get_by_index(fy_document_root(fyd), pos);
}

const char *yml_parser_alloc_executable_path(struct fy_node *process_node)
{
        struct fy_node *executable_path_node = fy_node_by_path(process_node, "/executable_path", FY_NT, FYNWF_PTR_DEFAULT);
        if (!executable_path_node || !fy_node_is_scalar(executable_path_node))
        {
                perror("Failed to get executable_path node or node is not a scalar");
                exit(EXIT_FAILURE);
        }

        return fy_node_get_scalar0(executable_path_node);
}

int yml_parser_get_executable_args_cnt(struct fy_node *process_node)
{
        struct fy_node *executable_args_node = fy_node_by_path(process_node, "/executable_args", FY_NT, FYNWF_PTR_DEFAULT);
        if (!executable_args_node || !fy_node_is_sequence(executable_args_node))
        {
                perror("Failed to get executable_args node or node is not a sequence");
                exit(EXIT_FAILURE);
        }

        return fy_node_sequence_item_count(executable_args_node);
}

char **yml_parser_alloc_executable_args(struct fy_node *process_node)
{
        struct fy_node *executable_args_node = fy_node_by_path(process_node, "/executable_args", FY_NT, FYNWF_PTR_DEFAULT);
        if (!executable_args_node || !fy_node_is_sequence(executable_args_node))
        {
                perror("Failed to get executable_args node or node is not a sequence");
                exit(EXIT_FAILURE);
        }

        int count = fy_node_sequence_item_count(executable_args_node);
        char **args = malloc((count + 1) * sizeof(char *));
        if (!args)
        {
                perror("Failed to allocate memory for executable_args");
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < count; i++)
        {
                struct fy_node *arg_node = fy_node_sequence_get_by_index(executable_args_node, i);
                args[i] = strdup(fy_node_get_scalar0(arg_node));
                if (!args[i])
                {
                        perror("Failed to duplicate argument string");
                        exit(EXIT_FAILURE);
                }
        }
        args[count] = NULL;

        return args;
}

int yml_parser_get_pipe_count(fy_node_t *process_node)
{
        struct fy_node *pipes_node = fy_node_by_path(process_node, "/pipes", FY_NT, FYNWF_PTR_DEFAULT);
        if (!pipes_node)
                return 0;
        if (!fy_node_is_sequence(pipes_node))
        {
                perror("Node is not a sequence");
                exit(EXIT_FAILURE);
        }

        return fy_node_sequence_item_count(pipes_node);
}

int yml_parser_get_socket_count(fy_node_t *process_node)
{
        struct fy_node *sockets_node = fy_node_by_path(process_node, "/sockets", FY_NT, FYNWF_PTR_DEFAULT);
        if (!sockets_node)
                return 0;
        if (!fy_node_is_sequence(sockets_node))
        {
                perror("Node is not a sequence");
                exit(EXIT_FAILURE);
        }

        return fy_node_sequence_item_count(sockets_node);
}

struct fy_node *yml_parser_get_pipe(fy_node_t *process_node, int pos)
{
        struct fy_node *pipes_node = fy_node_by_path(process_node, "/pipes", FY_NT, FYNWF_PTR_DEFAULT);
        if (!pipes_node || !fy_node_is_sequence(pipes_node))
        {
                perror("Failed to get pipes node or node is not a sequence");
                exit(EXIT_FAILURE);
        }

        return fy_node_sequence_get_by_index(pipes_node, pos);
}

struct fy_node *yml_parser_get_socket(fy_node_t *process_node, int pos)
{
        struct fy_node *sockets_node = fy_node_by_path(process_node, "/sockets", FY_NT, FYNWF_PTR_DEFAULT);
        if (!sockets_node || !fy_node_is_sequence(sockets_node))
        {
                perror("Failed to get sockets node or node is not a sequence");
                exit(EXIT_FAILURE);
        }

        return fy_node_sequence_get_by_index(sockets_node, pos);
}

const char *yml_parser_get_channel_direction(struct fy_node *chan_node)
{
        struct fy_node *direction_node = fy_node_by_path(chan_node, "/direction", FY_NT, FYNWF_PTR_DEFAULT);
        if (!direction_node || !fy_node_is_scalar(direction_node))
        {
                perror("Failed to get direction node or node is not a scalar");
                exit(EXIT_FAILURE);
        }

        return fy_node_get_scalar0(direction_node);
}