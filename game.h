#ifndef GAME
#define GAME

#include "window.h"
#include "hero.h"
#include "wall.h"
#include "kaska.h"
#include "keyboardInput.h"
#include "drawing_functions.h"
#include "multiBoss.h"
#include "hpBar.h"

class Game{
public:

	AnimationSet* heroAnimSet;
	AnimationSet* kaskaAnimSet;
	AnimationSet* wallAnimSet;
	AnimationSet* multiAnimSet;
	AnimationSet* bulletAnimSet;

	SDL_Texture* backgroundImage;
	SDL_Texture* splashImage;
	SDL_Texture* diedTexture;
	SDL_Texture* endTexture;

	SDL_Texture* scoreTexture = NULL; //uzywane aby wyswietlic obecna liczbe etcs na ekran

	Hero *hero;
	KeyboardInput heroInput;

	list<Entity*> enemies;
	list<Entity*> walls;

	int enemyWavesTillBoss = 3;
	bool buildBossNext = false;
	bool bossActive = false;

	HPBar hpBar;

	bool splashShowing;
	float overlayTimer;


	Game();
	~Game();

	void update();
	void draw();

};


#endif