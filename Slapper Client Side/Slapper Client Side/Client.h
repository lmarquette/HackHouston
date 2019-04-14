#pragma warning (disable :4996)

#define DEFAULT_PORT "27015"


#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdio.h>	
#include <fcntl.h>
#include <Windows.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <fstream>

using namespace std;


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "advApi32.lib")

namespace Client
{
	struct Client_Data
	{
		SOCKET connect_socket;
	};

	int Client_Init(Client_Data *data)
	{
		WSADATA wsadata;
		struct addrinfo *result = NULL, *ptr = NULL, hints;
		data->connect_socket = INVALID_SOCKET;
		FILE *read_file;
		read_file = fopen("C:\\Users\\alexa\\source\\repos\\TCP_Server_SlappingStick\\TCP_Server_SlappingStick\\defaultAddress.txt", "r");
		if (read_file == NULL)
		{
			printf("error while opening file\n");
			return 1;
		}
		char * address = (char*)malloc(sizeof(char)*100);
		fscanf(read_file, "%s", address);

		printf("string is %s\n", address);

		int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
		if (iResult != 0)
		{
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		iResult = getaddrinfo(address, DEFAULT_PORT, &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{
			data->connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (data->connect_socket == INVALID_SOCKET)
			{
				printf("socket failed with error: %d\n", WSAGetLastError());
				closesocket(data->connect_socket);
				WSACleanup();
				return 1;
			}

			iResult = connect(data->connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				closesocket(data->connect_socket);
				data->connect_socket = INVALID_SOCKET;
				continue;
			}
			break;
		}



		freeaddrinfo(result);

		if (data->connect_socket == INVALID_SOCKET)
		{
			printf("unable to cocnnect to server!\n");
			WSACleanup();
			return 1;
		}


		return 0;

	}

	int Send_Data(SOCKET *connect_socket, char* buffer)
	{
		int iResult = send(*connect_socket, buffer, sizeof(buffer), 0);
		if (iResult == 0)
		{
			printf("could not send");
		}
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(*connect_socket);
			WSACleanup();
			return 1;
		}
		return 0;
	}

	int Recv_Data(SOCKET *connect_socket, char* buffer)
	{
		int iResult = recv(*connect_socket, buffer, sizeof(buffer), 0);
		if (iResult == 0) printf("Connection closed\n");
		if (iResult < 0)
		{
			printf("receive failed with the error: %d\n", WSAGetLastError());
			closesocket(*connect_socket);
			WSACleanup();
			return 1;
		}
		return 0;
	}

}