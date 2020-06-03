#include "drawing_functions.h"



SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		cout<< "error przy loadTexture"<<endl;
	}
	return texture;
}


SDL_Surface* loadSurface(const std::string &file, SDL_Renderer *ren){
	SDL_Surface *surface = IMG_Load(file.c_str());
	if (surface == nullptr)
	{
		cout << "error przy loadSurface" << endl;
	}
	return surface;
}

void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette)
{
	SDL_SetPaletteColors(surface->format->palette, palette->format->palette->colors, 0, palette->format->palette->ncolors);
}

SDL_Texture *convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer *ren, bool cleanSurface)
{
	SDL_Texture*texture = SDL_CreateTextureFromSurface(ren, surface);
	if (cleanSurface)
		cleanup(surface);

	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{

	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		cout<<"error przy tft"<<endl;
		return nullptr;
	}
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		cout << "error przy tft render" << endl;
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		cout<< "stworz teksture"<<endl;
	}

	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

SDL_Texture* renderText(const std::string &message, TTF_Font *font,
	SDL_Color color, SDL_Renderer *renderer)
{

	//renderowanie powierzchni na ktorej ma sie pojawic napis
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	//czyszczenie
	SDL_FreeSurface(surf);
	return texture;
}


