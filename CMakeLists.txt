cmake_minimum_required(VERSION 3.10)
project(node VERSION 0.1.0 LANGUAGES C)

# Require pkg-config
find_package(PkgConfig REQUIRED)
# Check for libfyaml using pkg-config
pkg_check_modules(FYAML REQUIRED libfyaml)


# Include directories for the YAML, libfyaml and our header files
include_directories(${FYAML_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Define the shared library with implementation source files
add_library(machine SHARED 
    src/machine.c
    src/machine_wrapper.c
    src/process.c
    src/process_wrapper.c
    src/pipe.c
    src/pipe_wrapper.c
    src/socket.c
    src/socket_wrapper.c
    src/channel.c
    src/channel_wrapper.c
    src/yaml_parser.c
)

# Link the libfyaml library with the machine library
target_link_libraries(machine ${FYAML_LIBRARIES})

# Define the main executable
add_executable(node 
    src/cmd_line_parser.c
    src/str_utils.c
    src/node.c
)

# Link the machine library with node executable
target_link_libraries(node machine)

# Define the server executable
add_executable(server
    src/server.c
)

# Define the client executable
add_executable(client
    src/client.c
)