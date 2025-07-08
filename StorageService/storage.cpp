#include "storage.h"
#include <time.h>

#define STORAGE_PORT 9999

void log_message(const PUB_INFO* info) {
	FILE* file = NULL;
	errno_t err = fopen_s(&file, "C:\\Users\\beleg\\Desktop\\IKP Projekat\\log.txt", "a");
	if (err != 0 || file == NULL) {
		printf("Failed to open log file\n");
		return;
	}

	time_t now = time(NULL);
	struct tm tstruct;
	localtime_s(&tstruct, &now);
	struct tm* t = &tstruct;

	fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d] Topic: %s | Message: %s\n",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec,
		info->topic, info->msg);

	fclose(file);
}

void StartStorageServer() {
	if (!InitializeWinsock()) {
		printf("Failed to initialize Winsock.\n");
		return;
	}

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET) {
		printf("Socket creation failed.\n");
		return;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(STORAGE_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listenSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("Bind failed with error: %d\n", WSAGetLastError());
		closesocket(listenSock);
		return;
	}

	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSock);
		return;
	}

	printf("StorageService is running on port %d...\n", STORAGE_PORT);

	while (!cancelationToken) {
		SOCKET clientSock = accept(listenSock, NULL, NULL);
		if (clientSock == INVALID_SOCKET) {
			printf("Accept failed: %d\n", WSAGetLastError());
			continue;
		}

		PUB_INFO info;
		int bytesReceived = recv(clientSock, (char*)&info, sizeof(PUB_INFO), 0);
		if (bytesReceived == sizeof(PUB_INFO)) {
			log_message(&info);
			printf("Logged: [%s] %s\n", info.topic, info.msg);
		}

		closesocket(clientSock);
	}

	closesocket(listenSock);
	WSACleanup();
}
