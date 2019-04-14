#pragma once
#include <time.h>
#include <iostream>
#include <assert.h>
#include "TCP_Server.h"
using namespace std;

enum lights { RED, GREEN, BLUE, OFF };
enum game_state { INITIALIZING, PLAYING, COMPARE, GAME_OVER };
enum player_number { PLAYER_ONE, PLAYER_TWO, SAME, NONE, BOTH };
enum slap_progress { SLAP_PLAYER_ONE, PLAYER_ONE_SLAPPED, SLAP_PLAYER_TWO, PLAYER_TWO_SLAPPED, SLAP_BOTH_PLAYERS, BOTH_PLAYERS_SLAPPED, NO_SLAPS };



void initialize_package(Package *package, unsigned int sequence_size)
{
	
	//INITIALIZE PACKAGES TO ORIGINAL SEQUENCE SIZE.
	//SETS THE ORIGINAL ENTRIES IN SEQUENCES TO OFF.

	//SEQUENCE_SIZE = NUMBER OF MOVES IN THE SEQUENCE REQUIRED.
	//PACKAGE = THE PACKAGE WHICH THE SEQUENCES ARE STORED.
	
	
	package->sequence_size = sequence_size;
	package->result = NONE;

	package->completion_time = 0;

	memset(package->sequence, OFF, sizeof(unsigned int) * 50);
}

void create_key_sequence(Package *key, unsigned int sequence_size, unsigned int number_of_buttons)
{
	/*
	
	CREATES A RANDOM SEQUENCE THAT THE PLAYERS WILL COPY.

	KEY = THE PACKAGE WHICH THE ORIGINAL SEQUENCE WILL BE STORED.
	SEQUENCE_SIZE = THE NUMBER OF MOVES THE SEQUENCE REQUIRES.
	NUMBER_OF_BUTTONS = SETS THE NUMBER OF POSSIBLE BUTTON CHOICES EACH ENTRY CAN BE.

	*/
	key->sequence_size = sequence_size; assert(key->sequence_size);

	for (unsigned int i = 0; i < key->sequence_size; i++)
	{
		key->sequence[i] = rand() % number_of_buttons;
	}
}

void populate_player_sequence(unsigned int player_input, Package *player_package, Package *key_sequence)
{
	/*
	
	FILLS IN EACH PLAYER'S SEQUENCES.

	PLAYER_INPUT = THE BUTTON PRESSED BY THE PLAYER. USED TO FILL IN EACH ENTRY OF THE SEQUENCE.
	PLAYER_PACKAGES = DETERMINES THE PACKAGE WHICH THE SEQUENCE WILL BE FILLED.
	KEY_SEQUENCE = DETERMINES THE REQUIRED SIZE OF THE ARRAY THAT NEEDS TO BE FILLED.
	
	*/
	for (unsigned int i = 0; i < key_sequence->sequence_size; i++)
	{
		player_package->sequence[i] = player_input;
	}
}

int compare_sequences(Package *key_sequence, Package *player_one, Package *player_two)
{
	/*
	
	COMPARES THE SEQUENCES OF THE PLAYERS TO THE KEY SEQUENCE. DETERMINES WHO MESSED UP ON INPUTING THE CORRECT SEQUENCE.

	KEY_SEQUENCE = CALLED TO HAVE THE CORRECT SEQUENCE THAT NEEDS TO BE COPIED.
	PLAYER_ONE = CALLED TO PULL UP THEIR INPUTTED SEQUENCE.
	PLAYER_TWO = CALLED TO PULL UP THEIR INPUTTED SEQUENCE.

	*/
	for (unsigned int i = 0; i < key_sequence->sequence_size; i++)
	{
		if (player_one->sequence[i] != key_sequence->sequence[i])
		{
			if (player_two->sequence[i] != key_sequence->sequence[i]) return BOTH;
			else return PLAYER_ONE;
		}
		else if (player_two->sequence[i] != key_sequence->sequence[i]) return PLAYER_TWO;
	}

	return NONE;
}

int compare_player_times(Package *player_one, Package *player_two)
{
	/*

	COMPARES COMPLETION TIMES. DETERMINES WHO FINISHED THEIR SEQUENCE FASTER.

	PLAYER_ONE = CALLED TO USE THEIR COMPLETION TIME.
	PLAYER_TWO = CALLED TO USE THEIR COMPLETION TIME.

	*/
	if (player_one->completion_time > player_two->completion_time) return PLAYER_ONE;
	else if (player_one->completion_time < player_two->completion_time) return PLAYER_TWO;
	else return SAME;
}

int compare_players(Package *key_sequence, Package *player_one, Package *player_two, int &slap_request)
{
	/*
	
	COMPARES PLAYERS BASED OFF COMPLETING THE SEQUENCE CORRECTLY. IF SEQUENCE IS COMPLETED CORRECTLY BY EVERY PLAYER, THEY ARE THEN COMPARED BY THEIR TIME REQUIRED TO FINISH THE SEQUENCE.

	KEY_SEQUENCE = CALLED TO PULL UP CORRECT SEQUENCE.
	PLAYER_ONE = CALLED TO PULL UP THEIR INPUTTED SEQUENCE AND COMPLETION TIME.
	PLAYER_TWO = CALLED TO PULL UP THEIR INPUTTED SEQUENCE AND COMPLETION TIME.
	*/
	int compare = compare_sequences(key_sequence, player_one, player_two);
	int compare_times = compare_player_times(player_one, player_two);

	if (compare == PLAYER_ONE)
	{
		//SEND SLAP COMMAND
		slap_request = SLAP_PLAYER_ONE;

	}
	else if (compare == PLAYER_TWO)
	{
		//SLAP PLAYER TWO
		slap_request = SLAP_PLAYER_TWO;
	}
	else if (compare == BOTH)
	{
		//SLAP BOTH PLAYERS
		slap_request = SLAP_BOTH_PLAYERS;
	}
	else if (compare == NONE)
	{
		if (compare_times == PLAYER_ONE)
		{
			//SLAP PLAYER ONE
			slap_request = SLAP_PLAYER_ONE;
		}
		else if (compare_times == PLAYER_TWO)
		{
			//SLAP PLAYER TWO
			slap_request = SLAP_PLAYER_TWO;
		}
		else if (compare_times == SAME)
		{
			//SLAP BOTH PLAYERS
			slap_request = SLAP_BOTH_PLAYERS;
		}
	}

	return slap_request;
}

void give_slap_results(Package *key, int slap_request, Server::Server_Data server_data)
{
	if (slap_request == SLAP_PLAYER_ONE)
	{
		key->result = 2;
		Server::Send_Data(&server_data.client_sockets[0], key);
		key->result = 3;
		Server::Send_Data(&server_data.client_sockets[1], key);
	}
	else if (slap_request == SLAP_PLAYER_TWO)
	{
		key->result = SLAP_PLAYER_TWO;
		Server::Send_Data(&server_data.client_sockets[1], key);
		key->result = 3;
		Server::Send_Data(&server_data.client_sockets[0], key);
	}
}

void game_over_event(bool *running, int &game_state)
{
	//SEND OUT GAME OVER SCREEN COMMAND TO CLIENTS
}