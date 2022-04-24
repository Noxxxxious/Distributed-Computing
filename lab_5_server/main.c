#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_MSSG_LEN 50
#define MAX_CLIENTS 3

SOCKET sockets[MAX_CLIENTS];
int client_count = 0;

DWORD WINAPI HandleClient(int id);
void printClientMessage(char* buffer);
void close(SOCKET si);
int main() {
	WSADATA wsas;
	WORD version = MAKEWORD(1, 1);
	int result = WSAStartup(version, &wsas);
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);	if (s == -1) {
		printf("Socket creation failed!\n");
		exit(0);
	}	struct sockaddr_in sa;
	memset((void*)(&sa), 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(8080);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(s, (struct sockaddr*)&sa, sizeof(sa));
	result = listen(s, 5);

	HANDLE threads[MAX_CLIENTS];
	struct sockaddr_in sc;
	int lenc;
	for(;;) {
		lenc = sizeof(sc);
		sockets[client_count] = accept(s, (struct sockaddr FAR*)&sc, &lenc);

		DWORD id;
		int client_id = client_count;
		threads[client_count] = CreateThread(NULL, 0, HandleClient, client_id, 0, &id);
		client_count++;	}	WaitForMultipleObjects(client_count, threads, 1, INFINITE);	return 0;
}

DWORD WINAPI HandleClient(int id) {
	printf("Client %d joined the server.\n", id);
	
	char buffer[MAX_MSSG_LEN];
	while (recv(sockets[id], buffer, MAX_MSSG_LEN, 0) > 0) {
		if (strcmp(buffer, "exit") == 0) {
			close(sockets[id]);
			return;
		}
		char* mssg;
		mssg = (char*)malloc(11 + MAX_MSSG_LEN);
		char tag[] = "[Client  ]: ";
		tag[8] = id + 48;
		strcpy(mssg, tag);
		if (buffer[0] == '/') {
			int dest = buffer[1] - 48;
			char* buffer_pointer = buffer;
			buffer_pointer += 3;
			strcat(mssg, buffer_pointer);
			send(sockets[dest], mssg, MAX_MSSG_LEN, 0);
		}
		else {
			strcat(mssg, buffer);
			int i;
			for (i = 0; i < client_count; i++) {
				send(sockets[i], mssg, MAX_MSSG_LEN, 0);
			}
		}
		printClientMessage(mssg);
	}
	printf("Client %d left the server.\n", id);
	return;
}

void close(SOCKET s) {
	closesocket(s);
	WSACleanup();
}

void printClientMessage(char* mssg) {
	int i = 0;
	while (mssg[i] != '\0') {
		printf("%c", mssg[i]);
		i++;
	}
	printf("\n");
}