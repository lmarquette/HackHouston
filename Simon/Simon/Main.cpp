#include "Server.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <Windows.h>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "advApi32")

void main()
{
	srand(time(0));

	Package player_one;
	Package player_two;
	Package key;
	Server::Server_Data server_data;


	bool running = true;
	int game_state = INITIALIZING;
	int sequence_size = 3;
	int number_of_buttons = 3;
	while (running)
	{
		if (game_state == INITIALIZING)
		{
			/*
			ESTABLISH CONNECTION
			
			GET USER CONFIRMATION
			*/
			Server::Server_Init(&server_data);

			initialize_package(&player_one, sequence_size);
			initialize_package(&player_two, sequence_size);
			initialize_package(&key, sequence_size);

			game_state = PLAYING;
		}
		else if (game_state == PLAYING)
		{
			//generate solution 
			//push to players
			//populate player input 
			//if player solution = solution set loser
			
			create_key_sequence(&key, sequence_size, number_of_buttons);
			key.result = game_state;

			for (int i = 0; i < server_data.num_clients; i++)
			{
				Server::Send_Data(&server_data.client_sockets[i], &key);
			}

			//RECIEVE PLAYER SEQUENCES
			Server::Recv_Data(&server_data.client_sockets[0], &player_one);

			Server::Recv_Data(&server_data.client_sockets[1], &player_two);

			game_state = COMPARE;
			
		}
		else if (game_state == COMPARE)
		{
			int slap_request = NONE;
			slap_request = compare_players(&key, &player_one, &player_two, &slap_request);

			give_slap_results(&key, slap_request, server_data);



			key.sequence_size++;
			if (key.sequence_size > 50) game_state = GAME_OVER;
		}
		else if (game_state == GAME_OVER)
		{
			game_over_event(&running, game_state);

			//IF PLAYERS WANT TO PLAY AGAIN
			game_state = INITIALIZING;

			//IF PLAYERS DON'T WANT TO PLAY AGAIN
			running = false;
		}
	}
}
