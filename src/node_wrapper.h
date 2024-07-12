#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "cmd_line_parser_api.h"
#include "pipe_utils_api.h"

typedef enum
{
        E_SERVER_PROC,
        E_CLIENT_PROC
} process_t;