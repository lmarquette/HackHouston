#pragma warning (disable :4996)
#define DEFAULT_PORT "27015" 

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <Windows.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include "Package.h"
using namespace std;

#pragma comment(linker, "/subsystem:console")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "advApi32")


namespace Server
{
	struct Server_Data
	{
		SOCKET listen_socket;
		int num_clients;
		SOCKET *client_sockets;
	};

	int Listen_For_Client(SOCKET *client_socket, SOCKET * listen_socket)
	{
		int iResult = listen(*listen_socket, SOMAXCONN);
		if (iResult == SOCKET_ERROR)
		{
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(*listen_socket);
			WSACleanup();
			return 1;
		}

		*client_socket = accept(*listen_socket, NULL, NULL);
		if (*client_socket == INVALID_SOCKET)
		{
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(*listen_socket);
			WSACleanup();
			return 1;
		}
		printf("client connected\n");
		return 0;
	}

	int Server_Init(Server_Data *data)
	{
		WSADATA wsadata;
		struct addrinfo *result = NULL;
		struct addrinfo hints;
		int iResult;
		data->listen_socket = INVALID_SOCKET;
		data->num_clients = 2;
		data->client_sockets = (SOCKET*)malloc(sizeof(SOCKET)*data->num_clients);
		unsigned long mode = 1;

		for (int i = 0; i < data->num_clients; i++)
		{
			data->client_sockets[i] = INVALID_SOCKET;
		}

		printf("server Initialising\n");

		iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
		if (iResult != 0)
		{
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		data->listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (data->listen_socket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		iResult = bind(data->listen_socket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			printf("bind failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(data->listen_socket);
			WSACleanup();
			return 1;
		}

		freeaddrinfo(result);

		printf("looking for players\n");


		for (int i = 0; i < data->num_clients; i++)
		{
			iResult = Listen_For_Client(&data->client_sockets[i], &data->listen_socket);
		}



		return 0;

	}

	int Send_Data(SOCKET *client_socket, Package *buffer)
	{
		printf("%s\n", buffer);
		int iResult = send(*client_socket, (char*)buffer, sizeof(Package), 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(*client_socket);
			WSACleanup();
			return 1;
		}
		return 0;
	}

	int Recv_Data(SOCKET *client_socket, Package* buffer)
	{
		int iResult = recv(*client_socket, (char*)buffer, sizeof(Package), 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(*client_socket);
			WSACleanup();
			return 1;
		}
		return 0;
	}

	void End_Server(Server_Data *data)
	{
		for (int i = 0; i < data->num_clients; i++)
		{
			closesocket(data->client_sockets[i]);
		}
		WSACleanup();
		printf("server Closed");
	}
}