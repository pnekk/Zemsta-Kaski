#ifndef GROUPBUILDER
#define GROUPBUILDER

#include "window.h"
#include "group.h"
#include "groupBox.h"
#include "groupPosition.h"
#include "GroupNumber.h"
#include "groupString.h"

class GroupBuilder
{
public:

	static const bool savedInGroups;


	//tworzy grupy danych bazowan¹ na dateType, 
	static Group* buildGroup(DataGroupType dataType);

	//tworzy grupe danych bazowana na dataaGroupTypes
	static void buildGroups(list<DataGroupType> groupTypes, list<Group*> &groups);


	//dodawanie strumienia znakow do grupy
	static Group* addGroupStringToGroup(string name, list<Group*> &groups);

	//loadowanie grup
	static void loadGroups(ifstream &file, list<Group*> &groups);

	//szuka grupy po imieniu
	static Group* findGroupByName(string str, list<Group*> &groups);

};

#endif