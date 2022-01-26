#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "errorHandling.h"
#include "packetReading.h"

void another_task();

int main() {
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

    printf("accept wait...\n");

    if( (client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size)) < 0 )
        errorHandling("accept() error");

    printf("client accepted!\n");

    int flag = fcntl(client_sock, F_GETFL, 0);
    fcntl(client_sock, F_SETFL, flag | O_NONBLOCK);
    
    char buf[BUF_SIZE];
    int retval, len;
    // memset(&len, 0x00, sizeof(len));
    while(1) {
        sleep(2);

        // 길이정보 수신하여 len에 저장
        retval = read(client_sock, &len, sizeof(len));
        if( retval < 0 ) {
            if(errno != EAGAIN) {
                printf("read() error\n");
                printf("----- Socket close -----\n");
                close(client_sock);
                break;
            }
        }
        else if( retval == 0 ) {
            printf("----- Socket close -----\n");
            close(client_sock);
            break;
        }
        else {
            // len만큼 문자열 수신
            readString(client_sock, buf, len);
        }

        another_task();
    }

    return 0;
}

void another_task() {
    printf("----- Another task processing...\n");
}
