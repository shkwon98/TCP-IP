// #define _WINSOCK_DEPRECATED_NO_WARNINGS
// #define _CRT_SECURE_NO_WARNINGS
// #pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <winsock2.h>
#include "errorHandling.h"
#include "packetSending.h"

int main()
{
    WSADATA wsaData;
    SOCKET client_socket;
    SOCKADDR_IN server_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        errorHandling("WSAStartup() error");

    if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
        errorHandling("socket() error");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        errorHandling("connect() error");

    printf("Connected to server\n");

    char buf[BUF_SIZE];
    while (1)
    {
        memset(buf, 0, BUF_SIZE);

        printf("Input message(\"exit\" to shutdown): ");
        fgets(buf, BUF_SIZE, stdin);
        buf[strlen(buf) - 1] = '\0';
        if (!strcmp(buf, "exit"))
            break;

        sendString(client_socket, buf);
        Sleep(1);
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
