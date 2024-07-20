cmake_minimum_required(VERSION 3.0.0)
project(node VERSION 0.1.0 LANGUAGES C)

# Find the YAML library
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML REQUIRED yaml-0.1)

# Include the YAML headers
include_directories(${YAML_INCLUDE_DIRS})

add_executable(node 
        src/networking_api.h
        src/cmd_line_parser.c
        src/cmd_line_parser_api.h
        src/yaml_parser.c
        src/yaml_parser_api.h
        src/str_utils.c
        src/str_utils_api.h
        src/process_mgr.c
        src/process_mgr_api.h
        src/pipe_utils.c
        src/pipe_utils_api.h
        src/node.c)


target_link_libraries(node ${YAML_LIBRARIES})

add_executable(server
        src/server.c)

add_executable(client
        src/client.c)

