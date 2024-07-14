#include "node_wrapper.h"

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

int fork_process(pid_t *pid_p, char *const args[], bool use_pipe, pipe_direction_t pipe_direct)
{
        int pipe_fd[2] = {0};

        try_handle_pipe_create(pipe_fd, use_pipe);

        *pid_p = fork();

        if (*pid_p == -1)
        {
                perror("Failed to fork process");

                try_handle_pipe_fork_failure(pipe_fd, use_pipe);
                exit(EXIT_FAILURE);
        }
        else if (*pid_p == 0)
        {

                try_handle_child_pipe_end(pipe_fd, pipe_direct, use_pipe);
                execvp(args[0], args);
                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }

        try_handle_parent_pipe_end(pipe_fd, pipe_direct, use_pipe);

        return EXIT_SUCCESS;
}

int run_client(pid_t *pid_p)
{
        int ret = EXIT_SUCCESS;
        bool use_pipe = true;
        pipe_direction_t pipe_direct = E_PIPE_DIR_PARENT_TO_CHILD;

        ret |= fork_process(pid_p, client_cmd, use_pipe, pipe_direct);
        return ret;
}

int run_server(pid_t *pid_p)
{
        int ret = EXIT_SUCCESS;
        bool use_pipe = false;
        // TODO: No need in  'pipe_direct' in server. Fix this.
        pipe_direction_t pipe_direct = E_PIPE_DIR_PARENT_TO_CHILD;

        ret |= fork_process(pid_p, server_cmd, use_pipe, pipe_direct);
        return ret;
}

int run_node()
{
        // TODO: No need in both ret & status
        int ret = EXIT_SUCCESS;
        pid_t pid[2];
        int status;

        ret |= run_server(&(pid[0]));
        ret |= run_client(&(pid[1]));

        // TODO: Handle 'status' before reusing it.
        waitpid(pid[0], &status, 0);
        waitpid(pid[1], &status, 0);
        return ret;
}

int main(int argc, char **argv)
{
        // TODO: Manage return values. Also return values of cmd_line_parser module.
        get_cmds(argc, argv);
        run_node();
        return EXIT_SUCCESS;
}