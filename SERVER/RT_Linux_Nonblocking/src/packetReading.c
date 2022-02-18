#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "packetReading.h"

void readString(int socketFD, char *buffer, int length)
{
    memset(buffer, 0, BUF_SIZE);
    read(socketFD, (void *)buffer, length);
}

// void readValue1(int socketFD, char* buffer) {}

// void readValue2(int socketFD, char* buffer) {}
