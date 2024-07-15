/**
 * @file     client.c
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

#define IP ("127.0.0.1")
#define PORT (8080)
#define SEND_BUFFER_BYTE_SIZE (1024)
#define RECV_BUFFER_BYTE_SIZE (1024)

/**
 * @brief    Text
 *
 */
void run_client()
{
        int client_fd;
        struct sockaddr_in address;
        unsigned int addrlen = sizeof(address);
        char send_buff[SEND_BUFFER_BYTE_SIZE];
        char recv_buff[RECV_BUFFER_BYTE_SIZE];
        int bytes_recived = 0;

        printf("Client process created\n");

        if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                perror("Failed to create socket");
                exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_port = htons(PORT);

        if (inet_pton(AF_INET, IP, &address.sin_addr) <= 0)
        {
                perror("Invalid address/ Address not supported");
                close(client_fd);
                exit(EXIT_FAILURE);
        }

        if (connect(client_fd, (struct sockaddr *)&address, addrlen) < 0)
        {
                perror("Failed to connect socket");
                close(client_fd);
                exit(EXIT_FAILURE);
        }

        strcpy(send_buff, "Hi, I'm a client");

        if (send(client_fd, send_buff, strlen(send_buff) + 1, 0) < 0)
        {
                perror("Failed to send data");
                close(client_fd);
                exit(EXIT_FAILURE);
        }

        if ((bytes_recived = recv(client_fd, recv_buff, RECV_BUFFER_BYTE_SIZE, 0)) > 0)
        {
                printf("Received: %s\n", recv_buff);
        }
        else
        {
                perror("Failed to receive data");
        }

        close(client_fd);
}

/**
 * @brief    Text
 *
 * @param    argc                My Param doc
 * @param    argv                My Param doc
 * @return   int
 */
int main(int argc, char **argv)
{
        run_client();

        exit(EXIT_SUCCESS);
}