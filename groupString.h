#ifndef GROUPSTRING_H
#define GROUPSTRING_H

#include "group.h"

class GroupString : public Group{
public:
	list<string> data;

	GroupString(DataGroupType type){
		this->type = type;
	}
	int numberOfDataInGroup(){
		return data.size();
	}
	void addToGroup(string str){
		if (str[0] == ' '){
			str.erase(0, 1);
		}

		data.push_back(str);
	}

	void draw(){

	}

};


#endif