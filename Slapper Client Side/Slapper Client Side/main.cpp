#include <iostream>
#include <time.h>
#include <sapi.h>
enum gamestate {initializing, match_start, results};
using namespace std;

struct Package
{
	char result = 2;
	char n_data;
	char data[50];
	char user_input[50];
	unsigned int d_time;
};

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
	Package client_data;

	unsigned int start;
	unsigned int end;

	ISpVoice *pVoice = NULL;

	initialize_tts(&pVoice);
	//red = 1, 
	//play game
	if (client_data.result == 1)
	{
		const wchar_t *countdown = L"3 2 1 GO";
		pVoice->Speak(countdown, SPF_IS_XML, NULL);
		start = clock(); //starts timer
		for (int i = 0; i < client_data.n_data; i++)
		{
			client_data.user_input[i] = 0; //replace 0 with user arduino input
		}
		end = clock();
		client_data.d_time = end - start;
		//send data back with send function

	}

	//Get SLAPPED
	if (client_data.result == 2)
	{
		slapfest();
		const wchar_t *slap = L"<pitch middle = '+30'>Get SLAPPED nerd";
		const wchar_t *new_game = L"Would you like to play another game?";
		const wchar_t *blue = L"<pitch middle = '+30'>What is wrong with you, you stupid ass, <pitch middle = '-30'>papa smurf dick sucking fuck. Who picks the blue button. <pitch middle = '+30'>Honestly";

		pVoice->Speak(blue, SPF_IS_XML, NULL);
		pVoice->Speak(slap, SPF_IS_XML, NULL);
		pVoice->Speak(new_game, SPF_IS_XML, NULL);

		if (client_data.user_input[0] == 0) client_data.result = 1;
		else
		{
			return 0;
		}

	}
	//system("pause");
	return 0;
}