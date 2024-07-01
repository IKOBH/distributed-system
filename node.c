#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int fork_process(pid_t pid, char *args[]){
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

int run(){
        pid_t server_pid;
        pid_t client_pid;
        int server_status;
        int client_status;
        char *server_args[] = {"./server", NULL};
        char *client_args[] = {"./client", NULL};

        fork_process(server_pid, server_args);
        fork_process(client_pid, client_args);

        waitpid(client_pid, &client_status, 0);
        waitpid(server_pid, &server_status, 0);
}

int main (int arc, int **argv){

        // CMD line parsing...
        run();

        return EXIT_SUCCESS;
}