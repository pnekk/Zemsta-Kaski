#include "frame.h"

void Frame::Draw(SDL_Texture* spriteSheet, float x, float y){
	SDL_Rect dest; //cel gdzie chcemy rysowac klatke
	dest.x = x - offSet.x;
	dest.y = y - offSet.y;
	dest.w = clip.w;
	dest.h = clip.h;

	renderTexture(spriteSheet, Window::renderer, dest, &clip);
}


void Frame::loadFrame(ifstream &file, list<DataGroupType> &groupTypes){
	//konstruktor do przechowywania danych o klatkach
	GroupBuilder::buildGroups(groupTypes, frameData);


	string buffer;
	getline(file, buffer);
	getline(file, buffer);
	stringstream ss;
	buffer = Window::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h; 

	getline(file, buffer);
	ss.clear();
	buffer = Window::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offSet.x >> offSet.y;

	getline(file, buffer);
	ss.clear();
	buffer = Window::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;

	getline(file, buffer);
	ss.clear();
	buffer = Window::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	GroupBuilder::loadGroups(file, frameData);
}