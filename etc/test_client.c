#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <winsock2.h>
#include "errorHandling.h"

#define SERVER_IP "192.168.0.10"
#define PORT      2000
#define BUF_SIZE  1024
#define KEY_SIZE  1024

int main() {

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

    if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        errorHandling("connect() error");

    printf("Connected to server\n");

    char key[KEY_SIZE];
    unsigned char buf[BUF_SIZE];
    uint16_t header, a, b, c;
    header = 0x0006;
    a =      0x0102;
    b =      0x1122;
    c =      0x0097;
    *((uint16_t*)buf) = header;
    *((uint16_t*)(buf + 2)) = a;
    *((uint16_t*)(buf + 4)) = b;
    *((uint16_t*)(buf + 6)) = c;

    while (1) {
        printf("Input message(Q to exit): ");
        fgets(key, BUF_SIZE, stdin);
        key[strlen(key) - 1] = '\0';

        if (!strcmp(key, "q") || !strcmp(key, "Q"))
            break;
        else if (!strcmp(key, "ab")) {
            printf("header = %u, a = %u, b = %u, c = %u\n", header, a, b, c);
            printf("%02x %02x ", buf[0], buf[1]);
            printf("%02x %02x ", buf[2], buf[3]);
            printf("%02x %02x ", buf[4], buf[5]);
            printf("%02x %02x \n", buf[6], buf[7]);

            send(client_socket, buf, 8, 0);
        }        
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
