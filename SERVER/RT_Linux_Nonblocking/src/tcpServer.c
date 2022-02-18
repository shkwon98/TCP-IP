#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include "errorHandling.h"
#include "packetReading.h"
#include "tcpServer.h"

extern pthread_mutex_t mutex;

void *tcpServer(void *buffer)
{
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    int server_sock, client_sock;

    if ((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        errorHandling("socket() error");

    int option = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        errorHandling("bind() error");

    if (listen(server_sock, 5) < 0)
        errorHandling("listen() error");

    printf("accept wait...\n");

    if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size)) < 0)
        errorHandling("accept() error");

    printf("client accepted!\n");

    int flag = fcntl(client_sock, F_GETFL, 0);
    fcntl(client_sock, F_SETFL, flag | O_NONBLOCK);

    int retval, length;
    while (1)
    {
        /* 길이정보 수신하여 length에 저장 */
        retval = read(client_sock, &length, sizeof(length));
        if (retval < 0)
        {
            if (errno != EAGAIN)
            {
                printf("read() error\n");
                printf("----- Socket close -----\n");
                break;
            }
        }
        else if (retval == 0)
        {
            printf("----- Socket close -----\n");
            break;
        }
        else
        {
            pthread_mutex_lock(&mutex);

            /* length만큼 문자열 수신 */
            readString(client_sock, buffer, length);

            pthread_mutex_unlock(&mutex);
        }
    }

    close(client_sock);
    strcpy(buffer, "exit");
    pthread_exit(NULL);
    return 0;
}