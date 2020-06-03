#include "cleanup.h"
#include "drawing_functions.h"
#include "SDL_mixer.h"
#include "window.h"
#include "game.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char **agv){
	srand(time(0)); 



	SDL_Window *window = SDL_CreateWindow("Zemsta Kaski", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		Window::ScreenWidth* Window::ScreenScale, Window::ScreenHeight* Window::ScreenScale, SDL_WINDOW_SHOWN);



	Window::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	SDL_RenderSetLogicalSize(Window::renderer, Window::ScreenWidth, Window::ScreenHeight);


	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_Quit();
		cout << "sdl image nie zainicjaliuzowany" << endl;
		return 1;
	}

	Game game;
	game.update();

	cleanup(Window::renderer);
	cleanup(window);
	//cleanup(texture);

	SDL_Quit();
	return 0;


}