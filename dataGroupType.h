#ifndef DATAGROUPTYPE_H
#define DATAGROUPTYPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

//klatki przechowuja swoje dane, ktore (dane) pomagaja okreslic jakie dane sa w grupie
class DataGroupType{
public:
	string groupName; //nazwa grupy danych, np hi
	int dataType;
	bool singleItem;

	//zapisywanie i ladowanie grup
	static void saveRSDataGroupType(ofstream &file, DataGroupType dataGroupType);
	static DataGroupType loadRSDataGroupType(ifstream &file);

	static const int DATATYPE_STRING = 0, DATATYPE_POSITION = 1, DATATYPE_BOX = 2, DATATYPE_NUMBER = 3;

};

#endif