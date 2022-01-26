#pragma once
#define SERVER_IP  "192.168.0.10"
#define PORT        2000
#define BUF_SIZE    1024

#pragma pack(push, 1)
typedef struct packet_str {
    uint32_t    length;
    char        str[512];
} PACKET_STR;

typedef struct packet_mod1 {
    uint32_t    length;
    uint16_t    value1;
} PACKET_MOD1;

typedef struct packet_mod2 {
    uint32_t    length;
    uint8_t     value2;
} PACKET_MOD2;
#pragma pack(pop)


void sendString(SOCKET socketFD, char* buffer);

// void sendValue1(SOCKET socketFD, char* buffer);

// void sendValue2(SOCKET socketFD, char* buffer);