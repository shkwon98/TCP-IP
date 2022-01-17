#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // 파일 관리 함수 헤더
#include <arpa/inet.h>
#include <sys/socket.h> // 소켓프로그래밍 함수선언

#define BUFSIZE 1024
void errorHandling(char *errmsg);
 
int main() {
    // ip, port 정의
    char ip[] = "192.168.0.10";
    int port = 2000;
    char message[BUFSIZE];

	int server_socket;
    int client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;
    
    // 서버 소켓 생성
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
        errorHandling("socket() error");

    // 주소 구조체 정의
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    
    // 서버 소켓에 주소정보 할당
    int bindret = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(bindret == -1)
        errorHandling("bind() error");

    // 서버 소켓으로 오는 클라이언트 요청 대기
    int listenret = listen(server_socket, 5);
        if(listenret == -1) errorHandling("listen() error");

    // 클라이언트 구조체 크기 밑작업 및 함수호출
    client_addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
    if(client_socket == -1)
        errorHandling("accept() error");
    printf("Connected\n");

    // 클라이언트의 메시지 수신
    int str_len;
    // memset(&message, 0, sizeof(message));
    while(1) {
        str_len = 0;
        while(str_len <= 0) {
            str_len = read(client_socket, message, BUFSIZE-1);
            if(str_len == -1)
                errorHandling("read() error");
        }
        message[str_len] = '\0';

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

        printf("received message: %s", message);
    }

    close(client_socket);
    close(server_socket);
    return 0;
}


void errorHandling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}   