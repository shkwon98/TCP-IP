#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "packetReading.h"

void readString(int socketFD, char* pBuffer, int length) {

    memset(pBuffer, 0, BUF_SIZE);

    read(socketFD, (void*)pBuffer, length);
    printf("message string: %s\n", pBuffer);
}

// void readValue1(int socketFD, char* pBuffer) {}

// void readValue2(int socketFD, char* pBuffer) {}
