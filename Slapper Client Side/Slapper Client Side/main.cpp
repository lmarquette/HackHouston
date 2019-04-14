

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
#include <time.h>
#include <sapi.h>
#include "Client.h"
enum gamestate {initializing, match_start, results};
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "advApi32.lib")

void initialize_tts(ISpVoice** pVoice)
{
	CoInitialize(NULL);
	CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)pVoice);
}

void slapfest()
{
	//getslappednerd
	//run ardiuno slap
}
	
int main()
{

	unsigned int start;
	unsigned int end;

	ISpVoice *pVoice = NULL;
	initialize_tts(&pVoice);

	Package receive_data;
	const char *port_str = "\\\\.\\COM6";
	HANDLE h_serial = CreateFile(port_str, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	DCB device;
	assert(GetCommState(h_serial, &device));
	device.BaudRate = 9600;
	SetCommState(h_serial, &device);

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 3;
	timeouts.ReadTotalTimeoutMultiplier = 3;
	timeouts.ReadTotalTimeoutConstant = 2;
	timeouts.WriteTotalTimeoutMultiplier = 3;
	timeouts.WriteTotalTimeoutConstant = 2;
	SetCommTimeouts(h_serial, &timeouts);

	int buffer_size = 256;
	char* buffer = new char[buffer_size];
	char user_input[50];

	unsigned long n_bytes = 0;

	int input = 0; //stores arduino button input here

	printf("%d\n", input);

	//Receive Data From Server
	Client::Client_Data client_data;
	Client::Client_Init(&client_data);
	Client::Recv_Data(&client_data.connect_socket, &receive_data);


	//play game
	if (receive_data.result == 1)
	{
		const wchar_t *countdown = L"3 2 1 GO";
		pVoice->Speak(countdown, SPF_IS_XML, NULL);
		start = clock(); //starts timer
		for (int i = 0; i < receive_data.n_data;)
		{
			int r = ReadFile(h_serial, buffer, buffer_size, &n_bytes, NULL);
			input = buffer[0];
			if (input == '1' || input == '2' || input == '3')
			{
				user_input[i++] = input;
			}
		}
		end = clock();
		receive_data.d_time = end - start;
		for (int i = 0; i < receive_data.n_data; i++)
		{
			receive_data.data[i] = user_input[i];
		}
		//send data back with send function
		Client::Send_Data(&client_data.connect_socket, &receive_data);

	}

	//Get SLAPPED
	if (receive_data.result == 2)
	{
		slapfest();
		const wchar_t *slap = L"<pitch middle = '+30'>Get SLAPPED nerd";
		const wchar_t *new_game = L"Would you like to play another game?";
		const wchar_t *blue = L"<pitch middle = '+30'>What is wrong with you, you stupid ass, <pitch middle = '-30'>papa smurf dick sucking fuck. Who picks the blue button. <pitch middle = '+30'>Honestly";

		pVoice->Speak(blue, SPF_IS_XML, NULL);
		pVoice->Speak(slap, SPF_IS_XML, NULL);
		pVoice->Speak(new_game, SPF_IS_XML, NULL);

		if (user_input[0] == 0) receive_data.result = 1;
		else
		{
			return 0;
		}

	}
	//system("pause");
	return 0;
}