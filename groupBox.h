#ifndef GROUPBOX_H
#define GROUPBOX_H

#include "group.h"

class GroupBox : public Group{
public:
	list<SDL_Rect> data;

	GroupBox(DataGroupType type){
		this->type = type;
	}

	int numberOfDataInGroup(){
		return data.size();
	}
	
	void addToGroup(string str){
		stringstream ss;
		ss << str;
		SDL_Rect box;
		ss >> box.x >> box.y >> box.w >> box.h;

		data.push_back(box);
	}

	void draw(){
	}

};


#endif