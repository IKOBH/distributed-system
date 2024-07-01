#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void server(){
        printf("Server is running\n");
}

void client(){
        printf("Client is running\n");
}

int fork_process(pid_t pid, void (*func_ptr)(void)){
        pid = fork();

        if (pid < 0){
                perror("Failed to fork process");
                exit(EXIT_FAILURE);
        } else if (pid == 0){
                (*func_ptr)();
                 exit(EXIT_SUCCESS);
        }

        return EXIT_SUCCESS;
}

int run(){
        pid_t server_pid;
        pid_t client_pid;
        int server_status;
        int client_status;

        fork_process(server_pid, server);
        fork_process(client_pid, client);

        waitpid(client_pid, &client_status, 0);
        waitpid(server_pid, &server_status, 0);
}

int main (int arc, int **argv){

        // CMD line parsing...
        run();
}