// #define _WINSOCK_DEPRECATED_NO_WARNINGS
// #define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <winsock2.h>
#include "packetSending.h"

void sendString(SOCKET socketFD, char *buffer)
{
    PACKET_STR *pMsg = (PACKET_STR *)&buffer;

    // strcpy(pMsg->str, "hello world1 hello world2 hello world3 hello world4 hello world5");
    strcpy(pMsg->str, buffer);
    pMsg->length = strlen(pMsg->str);
    int total_length = sizeof(pMsg->length) + pMsg->length;

    send(socketFD, (void *)&buffer, total_length, 0);
}

// void sendValue1(SOCKET socketFD, char* buffer) {}

// void sendValue2(SOCKET socketFD, char* buffer) {}
