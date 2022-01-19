#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024
#define PORT 2000

void errorHandling(char *errmsg);

void another_task();

int main() {
    char buf[BUF_SIZE];
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    int server_sock, client_sock;

    if( (server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
        errorHandling("socket() error");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if( bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
        errorHandling("bind() error");

    if( listen(server_sock, 5) < 0 )
        errorHandling("listen() error");

    printf("----- accept wait\n");

    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);
    if( client_sock < 0 )
        errorHandling("accept() error");

    printf("----- accept client\n");

    int read_rtn;

    while(1) {
        printf("----- read wait\n");

        memset(buf, 0x00, sizeof(buf));
        read_rtn = read(client_sock, buf, sizeof(buf));

        if( read_rtn < 0 ) {
            printf("----- read error\n");
            printf("----- Socket close\n");
            close(client_sock);
            break;
        }
        else if( read_rtn == 0 ) {
            printf("----- Socket close\n");
            close(client_sock);
            break;
        }
        else {
            printf("----- read : %s\n", buf);
        }

        another_task();
    }

    return 0;
}

void another_task() {
    printf("----- Another task processing\n");
}

void errorHandling(char* errmsg) {
    fputs(errmsg, stderr);
    fputc('\n', stderr);
    exit(1);
}   
