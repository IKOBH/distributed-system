/**
 * @file     server.c
 * @author   Shahar Avidar  (ikobh7@gmail.com)
 * @brief    Text
 * @version  0.1
 * @date     2024-07-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server_api.h"

#define PORT (8080)

/**
 * @brief    Text
 *
 */
void run_server()
{
        int server_fd, request_fd;
        struct sockaddr_in address;
        int opt = 1;
        unsigned int addrlen = sizeof(address);
        char send_buff[OUTPUT_BUFFER_BYTE_SIZE];
        char recv_buff[INPUT_BUFFER_BYTE_SIZE];
        int bytes_recived = 0;

        printf("Server process created\n");

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
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

        if (bind(server_fd, (struct sockaddr *)&address, addrlen) == -1)
        {
                perror("Failed to bind socket to address");
                close(server_fd);
                exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 10) == -1)
        {
                perror("Failed to listen");
                close(server_fd);
                exit(EXIT_FAILURE);
        }

        printf("Server is listening on port %d\n", PORT);

        // TODO: Support more than one client by accepting more connections.(loop accept.)
        if ((request_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) == -1)
        {
                perror("Failed to accept connection");
                close(server_fd);
                exit(EXIT_FAILURE);
        }

        printf("Connection accepted\n");

        if ((bytes_recived = recv(request_fd, recv_buff, INPUT_BUFFER_BYTE_SIZE, 0)) == -1)
        {
                perror("Failed to receive data");
        }
        else
        {
                printf("Received: %s\n", recv_buff);
                sprintf(send_buff, "%s", recv_buff);
                send(request_fd, send_buff, strlen(send_buff) + 1, 0);
        }

        close(request_fd);
        close(server_fd);

        printf("Request arrived\n");
}

/**
 * @brief    Text
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 * @return   int
 */
int main(int argc, int **argv)
{
        run_server();

        return EXIT_SUCCESS;
}