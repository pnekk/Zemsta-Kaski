#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <SDL.h>
#include "dataGroupType.h"

using namespace std;
#ifndef GROUP_H
#define GROUP_H


//klasa do zarzadania danymi klatek
class Group{
public:
	DataGroupType type; //opisuje nazwe, typ
	Group(){
	}
	Group(DataGroupType type){
		this->type = type;
	}
	//ile bitow zawiera klatka
	virtual int numberOfDataInGroup() = 0;
	//dodwanie do grupy
	virtual void addToGroup(string str) = 0;


	virtual void draw() = 0;
};

#endif