#include "window.h"


bool Window::debugging = false; 


int Window::ScreenWidth = 1256, Window::ScreenHeight = 880, Window::ScreenScale = 1;
SDL_Renderer* Window::renderer = NULL;

string Window::clipOffDataHeader(string data){
	int pos = data.find(":", 0);
	if (pos != -1){
		data = data.substr(pos + 1, data.length() - pos + 2);
	}
	return data;
}
