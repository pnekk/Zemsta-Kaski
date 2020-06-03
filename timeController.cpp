#include "timeController.h"



TimeController::TimeController(){
	dT = 0;
	lastUpdate = 0;
}
void TimeController::updateTime(){
	if (timeState){
		dT = 0;
	}
	else{
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		dT = timeDiff / 1000.0;
	}
	lastUpdate = SDL_GetTicks();
}

void TimeController::reset(){
	dT = 0;
	lastUpdate = SDL_GetTicks();
}

TimeController TimeController::timeController;