#ifndef ANIMATION
#define ANIMATION

#include "frame.h"

using namespace std;

class Animation{
public:
	string name; //nazwa animacji
	list<Frame> frames; //lista dostepnych klatek przez animacje

	Animation(string name = "");

	int getNextFrameNumber(int frameNumber);//zwraca kolejna klatke z listy
	Frame *getNextFrame(Frame* frame);
	int getEndFrameNumber(); //zwraca ostatnia klatke
	Frame* getFrame(int frameNumber); //wybierz klatke, numer klatki

	void loadAnimation(ifstream &file, list<DataGroupType> &groupTypes); //ladowanie animacji z pliku
};

#endif