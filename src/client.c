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
#include <pthread.h>
#include <arpa/inet.h>

#define IP ("127.0.0.1")
#define PORT (8080)
#define SEND_BUFFER_BYTE_SIZE (1024)
#define RECV_BUFFER_BYTE_SIZE (1024)

/**
 * @brief    Text
 *
 * @param    args                My Param doc
 * @return   void*
 */
void *spin_recv(void *args)
{
        int client_fd = *(int *)(args);
        char recv_buff[RECV_BUFFER_BYTE_SIZE];
        int bytes_recived = 0;

        printf("Spin Recieve\n");
        if ((bytes_recived = recv(client_fd, recv_buff, RECV_BUFFER_BYTE_SIZE, 0)) > 0)
        {
                printf("Received: %s\n", recv_buff);
        }
        else
        {
                perror("Failed to receive data");
        }
}

/**
 * @brief    Text
 *
 * @param    args                My Param doc
 * @return   void*
 */
void *spin_send(void *args)
{
        int client_fd = *(int *)(args);
        char send_buff[SEND_BUFFER_BYTE_SIZE];

        printf("Spin Send\n");
        strcpy(send_buff, "Hi, I'm a client");

        if (send(client_fd, send_buff, strlen(send_buff) + 1, 0) < 0)
        {
                perror("Failed to send data");
                close(client_fd);
                exit(EXIT_FAILURE);
        }
}

/**
 * @brief    Text
 *
 */
void run_client()
{
        int client_fd;
        struct sockaddr_in address;
        unsigned int addrlen = sizeof(address);

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

        pthread_t send_thread, recv_thread;

        // TODO: Check return values.
        pthread_create(&send_thread, NULL, spin_send, &client_fd);
        pthread_create(&recv_thread, NULL, spin_recv, &client_fd);
        pthread_join(send_thread, NULL);
        pthread_join(recv_thread, NULL);

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