#include "node_wrapper.h"

int fork_process(pid_t pid, char *const args[], bool use_pipe, pipe_direction_t pipe_direct)
{
        int pipe_fd[2] = {0};

        if (use_pipe)
                handle_pipe_create(pipe_fd);

        pid = fork();

        if (pid == -1)
        {
                perror("Failed to fork process");
                if (use_pipe)
                        handle_pipe_fork_failure(pipe_fd);
                exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
                if (use_pipe)
                        handle_child_pipe_end(pipe_fd, pipe_direct);
                execvp(args[0], args);
                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }

        if (use_pipe)
                handle_parent_pipe_end(pipe_fd, pipe_direct);

        return EXIT_SUCCESS;
}

int executes(process_t proc, char *const args[], bool use_pipe)
{
        pid_t pid;
        int status;
        pipe_direction_t pipe_direct = E_PIPE_DIR_PARENT_TO_CHILD;

        // TODO: pass pid by refereance & not by value
        fork_process(pid, args, use_pipe, pipe_direct);
        waitpid(pid, &status, 0);

        // TODO: return value or change signature
}

int run_client()
{
        int ret = EXIT_SUCCESS;

        // printf("DEBUG: Client\n");
        ret |= executes(E_CLIENT_PROC, client_cmd, true);
        return ret;
}

int run_server()
{
        int ret = EXIT_SUCCESS;

        ret |= executes(E_SERVER_PROC, server_cmd, false);
        return ret;
}

int run_node()
{
        int ret = EXIT_SUCCESS;

        // TODO: Resolve race condition. Waiting for server pid before running client.
        ret |= run_server();
        ret |= run_client();
        return ret;
}

int main(int argc, char **argv)
{
        // TODO: Manage return values. Also return values of cmd_line_parser module.
        get_cmds(argc, argv);
        run_node();
        return EXIT_SUCCESS;
}