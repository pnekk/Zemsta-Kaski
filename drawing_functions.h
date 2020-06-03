#ifndef DRAWINGFUNCS_H
#define DRAWINGFUNCS_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "cleanup.h"

using namespace std;

//ladowanie obrazka 
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);


//ladowanie plaszczyzny
SDL_Surface* loadSurface(const std::string &file, SDL_Renderer *ren);

//kopiowanie palety
void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette);


//tworzenie plaszczyzny lub jej usuwanie
SDL_Texture *convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer *ren, bool cleanSurface = false);

//renderowanie tekstury w okreslonej sciezce
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr);

//renderowanie tekstury w okrelonym polozeniu (x,y)
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip = nullptr)
	;



//renderowanie tekstu przez SDL, uzywany do wyswietlania liczby ECTSow
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer);






#endif