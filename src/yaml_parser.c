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
#include <yaml.h>

#include "yaml_parser_api.h"
#include "str_utils_api.h"

#define SUFFIX ("_flat")
#define DELIMITER ('.')
#define NAME_MAX_LEN (50)
#define VALUE_MAX_LEN (200)

char *cat_suffix_to_file_name(const char *file_name, const char *suffix, char *new_file_name)
{
        char *last_delim_p = strrchr(file_name, DELIMITER);
        if (last_delim_p == NULL)
        {
                perror("Invalid file name - extension not found.");
                exit(EXIT_FAILURE);
        }
        return str_embed(file_name, suffix, last_delim_p - file_name, new_file_name);
}

void update_curr_struct_p(char *struct_name, void *curr_struct_p, void *native_struct)
{
        // if (strcmp(struct_name, "B1") == 0)
        // {
        //         curr_struct_p = native_struct->
        // }
        // else if (strcmp(string, "xxx") == 0)
        // {
        //         // do something else
        // }
        // /* more else if clauses */
        // else /* default: */
        // {
        // }
}

void yaml_constrct_native_struct()
{
}

int yaml_parse_file(const char *file_name, void *native_struct)
{
        yaml_parser_t parser;
        yaml_event_t event;
        bool done = false;
        int status = 1;
        int map_stack = 0;
        int seq_stack = 0;
        bool update_struct_name = false;
        bool is_field_val_pair = false;
        bool is_field = false;
        bool is_val = false;
        // TODO: Handle cases where (name > NAME_MAX_LEN) or (value > VALUE_MAX_LEN)
        //       by checking strlen(event.data.scalar.value) > NAME_MAX_LEN\VALUE_MAX_LEN
        //       & calling malloc & free.
        char struct_name[NAME_MAX_LEN];
        char field_name[NAME_MAX_LEN];
        char value[VALUE_MAX_LEN];
        void *curr_struct_p = NULL;

        /* Set a file input. */
        FILE *file_p = fopen(file_name, "r");
        if (file_p == NULL)
        {
                perror("Failed to open config file");
                exit(EXIT_FAILURE);
        }

        /* Create the Parser object. */
        status = yaml_parser_initialize(&parser);
        if (!status)
                goto close_file;

        yaml_parser_set_input_file(&parser, file_p);

        /* Read the event sequence. */
        while (!done)
        {
                /* Get the next event. */
                status = yaml_parser_parse(&parser, &event);
                if (!status)
                        goto destroy_parser;

                switch (event.type)
                {
                case YAML_MAPPING_START_EVENT:
                        map_stack++;
                        // printf("MAP START: =\t%d\n", map_stack);
                        if (map_stack == 1)
                        {
                                update_struct_name = true;
                                is_field_val_pair = false;
                        }
                        if (map_stack == 2)
                        {
                                is_field_val_pair = true;
                                is_field = true;
                        }
                        // printf("Mapping:\t\t%s\n", event.data.mapping_start.anchor);
                        break;
                case YAML_MAPPING_END_EVENT:
                        map_stack--;
                        printf("\n");
                        break;
                case YAML_SEQUENCE_START_EVENT:
                        seq_stack++;
                        // printf("SEQ START: =\t%d\n", seq_stack);
                        //  printf("Sequence: anchor =\t%s\n", event.data.sequence_start.anchor);
                        //  printf("Sequence: anchor =\t%s\n", event.data.sequence_start.anchor);
                        break;
                case YAML_SEQUENCE_END_EVENT:
                        seq_stack--;
                        // printf("SEQ END: =\t%d\n", seq_stack);
                        break;
                case YAML_ALIAS_EVENT:
                        printf("\t%s", event.data.alias.anchor);
                        //  Handle alias
                        break;
                case YAML_SCALAR_EVENT:
                        if (update_struct_name)
                        {
                                strcpy(struct_name, event.data.scalar.value);
                                update_curr_struct_p(struct_name, curr_struct_p, native_struct);
                                update_struct_name = false;
                                printf("%s:\n", struct_name);
                        }
                        if (is_field_val_pair)
                        {
                                if (is_field)
                                {
                                        strcpy(field_name, event.data.scalar.value);
                                        is_field = false;
                                        is_val = true;
                                        printf("\t%s:", field_name);
                                }
                                else if (is_val)
                                {
                                        strcpy(value, event.data.scalar.value);
                                        is_val = false;
                                        printf("\t%s", value);
                                }
                        }
                        break;
                case YAML_STREAM_END_EVENT:
                        done = true;
                        break;
                default:
                        break;

                        /* Destroy event object. */
                        yaml_event_delete(&event);
                }
                yaml_constrct_native_struct();
        }

        /* error handling */
destroy_parser:
        /* Destroy Parser object. */
        yaml_parser_delete(&parser);
close_file:
        if (fclose(file_p) != 0)
                perror("Failed to close file");

        // TODO: Refactor the return value to use only status and not status-1.
        return status - 1;
}

void yaml_load_file(const char *file_name, void *native_struct)
{
        // int new_file_name_len = strcumlen(2, file_name, SUFFIX) + 1;
        // char *new_file_name = (char *)malloc(new_file_name_len);
        // if (new_file_name == NULL)
        // {
        //         perror("Failed to allocate memory for new file name.");
        //         exit(EXIT_FAILURE);
        // }

        // new_file_name = cat_suffix_to_file_name(file_name, SUFFIX, new_file_name);
        // printf("new_file_name = %s\n", new_file_name);

        // free(new_file_name);

        // yaml_compose_event_tree();
        yaml_parse_file(file_name, native_struct);
}