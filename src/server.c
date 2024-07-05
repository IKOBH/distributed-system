#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT (8080)
#define SEND_BUFFER_BYTE_SIZE (1024)
#define RECV_BUFFER_BYTE_SIZE (1024)

void run_server()
{
        int server_fd, request_fd;
        struct sockaddr_in address;
        int opt = 1;
        unsigned int addrlen = sizeof(address);
        char buffer[1024] = {0};
        char send_buff[SEND_BUFFER_BYTE_SIZE];
        char recv_buff[RECV_BUFFER_BYTE_SIZE];
        int bytes_recived = 0;

        printf("Server process created\n");

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                perror("Failed to create socket");
                exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
                perror("Failed to set socket options");
                close(server_fd);
                exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0)
        {
                perror("Failed to bind socket to address");
                close(server_fd);
                exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 10) < 0)
        {
                perror("Failed to listen");
                close(server_fd);
                exit(EXIT_FAILURE);
        }

        printf("Server is listening on port %d\n", PORT);

        if ((request_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
                perror("Failed to accept connection");
                close(server_fd);
                exit(EXIT_FAILURE);
        }

        printf("Connection accepted\n");

        strcpy(send_buff, "Hi client, I'm a server");
        if ((bytes_recived = recv(request_fd, recv_buff, RECV_BUFFER_BYTE_SIZE, 0)) > 0)
        {
                send(request_fd, send_buff, strlen(send_buff) + 1, 0);
                printf("Received: %s\n", recv_buff);
        }
        else
        {
                perror("Failed to receive data");
        }

        close(request_fd);
        close(server_fd);

        printf("Request arrived\n");
}

int main(int argc, int **argv)
{
        run_server();

        return EXIT_SUCCESS;
}