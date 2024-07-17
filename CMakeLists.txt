cmake_minimum_required(VERSION 3.0.0)
project(node VERSION 0.1.0 LANGUAGES C)

add_executable(node 
        src/cmd_line_parser.c
        src/cmd_line_parser_api.h
        src/pipe_utils.c
        src/pipe_utils_api.h
        src/node.c)

add_executable(server
        src/server.c)

add_executable(client
        src/client.c)

