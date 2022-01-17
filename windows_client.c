#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

// #pragma comment(lib,"Ws2_32.lib")

#define BUFSIZE 1024
void errorHandling(char* message);

int main()
{
    // ip, port 정의
	char ip[] = "192.168.0.10";
    int port = 2000;
    char message[BUFSIZE];


    WSADATA wsaData;
    SOCKET client_socket;
    SOCKADDR_IN server_addr;

    // 소켓 라이브러리 초기화
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        errorHandling("WSAStartup() error");

    // 클라이언트 소켓 생성
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET)
        errorHandling("socket() error");

    // 주소 구조체 정의
    memset(&server_addr, 0 ,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // 생성된 클라이언트 소켓과 서버 연결
    if(connect(client_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        errorHandling("connect() error");
    printf("Connected\n");

    // 서버로 메시지 입력 및 전송
    while(1) {
        printf("Input message(Q to exit): ");
        fgets(message, BUFSIZE, stdin);
        send(client_socket, message, strlen(message), 0);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}

void errorHandling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}