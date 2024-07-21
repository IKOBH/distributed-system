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

#include "client_api.h"

void connection_ctx_init()
{
        // TODO: Implement
}

void connect_retry_ctx_init(connect_retry_ctx_t *retry_ctx)
{
        // TODO: Implement
}

void client_ctx_init(client_ctx_t *client_ctx)
{
        connection_ctx_init(client_ctx->connection_ctx);
        connect_retry_ctx_init(client_ctx->retry_ctx);
        client_ctx->user_args = NULL;
}

/**
 * @brief    Retry connecting server.
 *
 * @param    retries             retry attempt count (retries >= 0)
 * @param    client_fd           socket file descriptor to connect.
 * @param    address             server address.
 * @param    addrlen             sizeof address struct.
 */
static void client_retry_connect(int retries, int client_fd, struct sockaddr_in *address, unsigned int addrlen)
{
        int attempt_cnt = 0;
        do
        {
                if (connect(client_fd, (struct sockaddr *)address, addrlen) == 0)
                        return;

                usleep(CONNECT_SLEEP_GAP_US);
                fprintf(stdout, "Failed to connect to server. (Retry attempt %d/%d)\n", attempt_cnt, retries);
        } while (++attempt_cnt <= retries);

        perror("Failed to connect to server, no more retries");
        close(client_fd);
        exit(EXIT_FAILURE);
}

/**
 * @brief    Text
 *
 * @param    args                My Param doc
 * @return   void*
 */
static void *client_recv(void *args)
{
        int client_fd = *(int *)(args);
        char recv_buff[RECV_BUFFER_BYTE_SIZE];
        int bytes_recived = 0;

        printf("Spin Recieve thread\n");
        if ((bytes_recived = recv(client_fd, recv_buff, RECV_BUFFER_BYTE_SIZE, 0)) == -1)
        {
                perror("Failed to receive data");
                close(client_fd);
                exit(EXIT_FAILURE);
        }

        printf("Received: %s\n", recv_buff);
}

/**
 * @brief    Text
 *
 * @param    args                My Param doc
 * @return   void*
 */
static void *client_send(void *args)
{
        int client_fd = *(int *)(args);
        char send_buff[SEND_BUFFER_BYTE_SIZE];

        printf("Spin Send thread\n");
        if (send(client_fd, send_buff, strlen(send_buff) + 1, 0) == -1)
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
static void client_run()
{
        int client_fd;
        struct sockaddr_in address;
        unsigned int addrlen = sizeof(address);

        printf("Client process created\n");

        if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
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

        client_retry_connect(CONNECT_RETRIES, client_fd, &address, addrlen);

        pthread_t send_thread, recv_thread;

        // TODO: Check return values.
        pthread_create(&send_thread, NULL, client_send, &client_fd);
        pthread_create(&recv_thread, NULL, client_recv, &client_fd);
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
        client_run();

        exit(EXIT_SUCCESS);
}