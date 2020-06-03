#ifndef TIMECONTROLLER
#define TIMECONTROLLER

#include <iostream>
#include "SDL.h"

class TimeController{
public:

	int timeState;
	Uint32 lastUpdate; //ile sekund minelo od ostatniego updateu
	float dT; //czas od kiedy ostatnia klatka zostala pokazana na ekranie

	TimeController();
	void updateTime();//nadpisuwanie lastUpdate i dT
	void reset();

	static TimeController timeController;
};


#endif