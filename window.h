#ifndef GLOBALS
#define GLOBALS

#include <string>
#include <iostream>
#include <SDL.h>
#include "randomNumber.h"

using namespace std;

class Window{
public:

	static bool debugging; //wyswietlanie lub niewyswietlanie hitboxow

	static int ScreenWidth, ScreenHeight, ScreenScale;
	static SDL_Renderer* renderer;

	//odcinanie danych z hedera
	static string clipOffDataHeader(string data);

};

#endif