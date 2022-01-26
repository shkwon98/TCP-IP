#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "errorHandling.h"

#define PORT 2000
#define BUF_SIZE 1024
#define HEAD_SIZE 2

void another_task();

int main() {
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    int server_sock, client_sock;

    if( (server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
        errorHandling("socket() error");
    
    memset(&server_addr, 0x00, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if( bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
        errorHandling("bind() error");

    if( listen(server_sock, 5) < 0 )
        errorHandling("listen() error");

    printf("----- accept wait\n");

    if( (client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size)) < 0 )
        errorHandling("accept() error");

    printf("----- accept client\n");

    char buf[BUF_SIZE];
    char head[HEAD_SIZE];
    uint16 a, b, c;
    int body_len, i;
    while(1) {
        printf("----- read wait\n");
        Sleep(3000);

        memset(&head, 0, HEAD_SIZE);
        memset(&buf, 0, BUF_SIZE);
        read(client_sock, head, HEAD_SIZE);           // HEAD_SIZE(2byte)만큼 우선 head[]로 읽어들임
        body_len = *((uint16*)(head));                // body_len : 헤더에 담긴 숫자(읽어올 본문길이)
        body_len = read(client_sock, buf, body_len);  // body_len : 실제 읽어온 본문의 길이

        if (body_len < 0) {
            printf("----- read error\n");
            printf("----- Socket close\n");
            close(client_sock);
            break;
        }
        else if (body_len == 0) {
            printf("----- Socket close\n");
            close(client_sock);
            break;
        }
        else {
            printf("Received: %d bytes\n", body_len);
            for (i = 0; i < body_len; i++) {
                printf("%02x ", buf[i]);
            }
            a = *((uint16*)(buf));
            b = *((uint16*)(buf + 2));
            c = *((uint16*)(buf + 4));
            printf("\na=%u, b=%u, c=%u\n", a, b, c);
    }
        
    close(server_sock);
    return 0;
}

void another_task() {
    printf("----- Another task processing...\n");
}
