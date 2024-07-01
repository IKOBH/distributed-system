#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int fork_process(pid_t pid, char *const args[]){
        pid = fork();

        if (pid < 0){
                perror("Failed to fork process");
                exit(EXIT_FAILURE);
        } else if (pid == 0){
                execvp(args[0], args);

                perror("Failed to execute process");
                exit(EXIT_FAILURE);
        }

        return EXIT_SUCCESS;
}

int run(char *const args[]){
        pid_t pid;
        int status;

        fork_process(pid, args);
        waitpid(pid, &status, 0);
}

int main (int argc, char **argv){

        // CMD line parsing...
        char *const server_args[] = {argv[1], NULL};
        char *const client_args[] = {argv[2], NULL};
        run(server_args);
        run(client_args);

        return EXIT_SUCCESS;
}