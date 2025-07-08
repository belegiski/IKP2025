#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFF_SIZE 4096
#define MAX_TOPIC_LEN 128
#define MAX_MSG_LEN 2048
#define SERVER_ADDR "127.0.0.1"
#define PUB_PORT_S "7777"
#define SUB_PORT_S "8888"
#define PUB_PORT 7777
#define SUB_PORT 8888

static BOOL cancelationToken = FALSE;

typedef struct _PUB_INFO {
	char topic[MAX_TOPIC_LEN];
	char msg[MAX_MSG_LEN];
	SOCKET sock;
} PUB_INFO;

typedef struct _STORED_MSG {
	char topic[MAX_TOPIC_LEN];
	char msg[MAX_MSG_LEN];
	time_t timestamp;
} STORED_MSG;

SOCKET connect(short port);
bool InitializeWinsock();
int sendMessage(SOCKET sock, const char* buffer, int length);
int recvMessage(SOCKET sock, char* buffer, int length);
void closeSocket(SOCKET sock);
