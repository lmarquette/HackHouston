#include <iostream>
#include <time.h>
#include <assert.h>
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

	unsigned int start;
	unsigned int end;

	ISpVoice *pVoice = NULL;
	initialize_tts(&pVoice);

	Package client_data;
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

	unsigned long n_bytes = 0;

	int input = 0; //stores arduino button input here

	printf("%d\n", input);

	//play game
	if (client_data.result == 1)
	{
		const wchar_t *countdown = L"3 2 1 GO";
		pVoice->Speak(countdown, SPF_IS_XML, NULL);
		start = clock(); //starts timer
		for (int i = 0; i < client_data.n_data;)
		{
			int r = ReadFile(h_serial, buffer, buffer_size, &n_bytes, NULL);
			input = buffer[0];
			if (input == '1' || input == '2' || input == '3')
			{
				client_data.user_input[i++] = input;
			}
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