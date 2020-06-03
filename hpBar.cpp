#include "hpBar.h"

void HPBar::draw(){
	if (entity != NULL){
		SDL_SetRenderDrawColor(Window::renderer, 240, 51, 159, SDL_ALPHA_OPAQUE);
		//rysowanie paska hp
		SDL_Rect barContainer = { x, y, barWidth, barHeight };
		SDL_RenderDrawRect(Window::renderer, &barContainer);

		//ile hp zostalo
		float hpPercent = entity->hp / (entity->hpMax*1.0f);


		SDL_Rect hpRect = { x + 2, y + 2, (barWidth - 4)*hpPercent, barHeight - 4 };

		SDL_RenderFillRect(Window::renderer, &hpRect);

	}
}