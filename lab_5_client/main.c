#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_MSSG_LEN 50

DWORD WINAPI send_function(SOCKET* s);
DWORD WINAPI receive_function(SOCKET* s);
void close(SOCKET s);
void printClientMessage(char* buffer);
DWORD WINAPI thread_function();
int main() {
	WSADATA wsas;
	WORD version = MAKEWORD(1, 1);
	int result = WSAStartup(version, &wsas);
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		printf("Socket creation failed!\n");
		exit(0);
	}
	struct sockaddr_in sa;
	memset((void*)(&sa), 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(8080);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(s, (struct sockaddr*)&sa, sizeof(sa)) != 0) {
		printf("Connection error occured!");
		return 0;
	}

	DWORD id[2];
	HANDLE client_threads[2];

	client_threads[0] = CreateThread(NULL, 0, send_function, &s, 0, &id[0]);
	client_threads[1] = CreateThread(NULL, 0, receive_function, &s, 0, &id[1]);
	WaitForMultipleObjects(2, client_threads, 1, INFINITE);
	close(s);
	return 0;
}

DWORD WINAPI send_function(SOCKET* s) {
	char buffer[MAX_MSSG_LEN];
	int buff_size;
	for (;;) {
		fgets(buffer, MAX_MSSG_LEN, stdin);
		buff_size = strlen(buffer);
		buffer[buff_size - 1] = '\0';
		send(*s, buffer, buff_size, 0);
		if (strcmp(buffer, "exit") == 0) 
			break;
	}
	return 0;
}

DWORD WINAPI receive_function(SOCKET* s) {
	char buffer[MAX_MSSG_LEN];
	for (;;) {
		recv(*s, buffer, MAX_MSSG_LEN, 0);
		if (strcmp(buffer, "exit") == 0) {
			close(*(s));
			return 0;
		}
		printClientMessage(buffer);
	}
	printf("Leaving the server...\n");
	return 0;
}

void close(SOCKET s) {
	closesocket(s);
	WSACleanup();
}

void printClientMessage(char* buffer) {
	int i = 0;
	while (buffer[i] != '\0') {
		printf("%c", buffer[i]);
		i++;
	}
	printf("\n");
}