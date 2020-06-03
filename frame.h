#ifndef FRAME
#define FRAME

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "window.h"
#include "groupBuilder.h"

using namespace std;

class Frame{
public:
	int frameNumber; //numer klatki
	SDL_Rect clip; //region na "spritesheetcie" gdzie znajduje sie dana klatka
	float duration; //jak dlugo klatka ma byc na ekranie
	SDL_Point offSet; //os uzywana do poprawnego wyswietlania wszystkich animacji

	list<Group*> frameData;//przechowywanie dodatkowych danych

	void Draw(SDL_Texture* spriteSheet, float x, float y);

	void loadFrame(ifstream &file, list<DataGroupType> &groupTypes);
};

#endif