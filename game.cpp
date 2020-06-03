#include "game.h"

Game::Game(){
	backgroundImage = loadTexture("mapa.png", Window::renderer);
	splashImage = loadTexture( "menu.png", Window::renderer);
	diedTexture = loadTexture("nieZdales.png", Window::renderer);
	endTexture = loadTexture("Zdales.png", Window::renderer);
	splashShowing = true;
	overlayTimer = 2;


	//lista typow grup
	list<DataGroupType> dataGroupTypes;

	//jakie dane klatka moze miec
	DataGroupType colBoxType;
	colBoxType.groupName = "collisionBox";
	colBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//hitboxy
	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//damage
	DataGroupType dmgType;
	dmgType.groupName = "damage";
	dmgType.dataType = DataGroupType::DATATYPE_NUMBER;

	//dodawanie tych danych do listy
	dataGroupTypes.push_back(colBoxType);
	dataGroupTypes.push_back(hitBoxType);
	dataGroupTypes.push_back(dmgType);

	heroAnimSet = new AnimationSet();
	heroAnimSet->loadAnimationSet("szylka.fdset", dataGroupTypes, true, 0, true);

	kaskaAnimSet = new AnimationSet();
	kaskaAnimSet->loadAnimationSet("testk.fdset", dataGroupTypes, true, 0, true);

	multiAnimSet = new AnimationSet();
	multiAnimSet->loadAnimationSet("mltiBoss.fdset", dataGroupTypes, true, 0, true);

	bulletAnimSet = new AnimationSet();
	bulletAnimSet->loadAnimationSet("bullet.fdset", dataGroupTypes, true, 0, true);

	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	//tworzenie entity bohatera
	hero = new Hero(heroAnimSet);
	hero->invincibleTimer = 0;
	hero->x = Window::ScreenWidth / 2;
	hero->y = Window::ScreenHeight / 2;
	//sterowanie klawiatura
	heroInput.hero = hero;
	//dodawanie bohatera do listy entity
	Entity::entities.push_back(hero);



	int tileSize = 32;
	//tworzenie scian
	for (int i = 0; i < Window::ScreenWidth / tileSize; i++){
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize/2;
		newWall->y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = Window::ScreenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

	for (int i = 1; i < Window::ScreenHeight / tileSize - 1; i++){

		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		newWall = new Wall(wallAnimSet);
		newWall->x = Window::ScreenWidth - tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

	buildBossNext = false;
	bossActive = false;

	//ustawienia pozycji hpBar
	hpBar.x = Window::ScreenWidth / 2.0f - (hpBar.barWidth / 2.0f); 
	hpBar.y = Window::ScreenHeight - hpBar.barHeight - 20;
}

Game::~Game(){
	cleanup(backgroundImage);
	cleanup(splashImage);



	if (scoreTexture != NULL)
		cleanup(scoreTexture);

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete kaskaAnimSet;
	delete wallAnimSet;
	delete multiAnimSet;
	delete bulletAnimSet;

	delete hero;

	//usuwanie entity z listy
	Entity::removeAllFromList(&walls, true);
	Entity::removeAllFromList(&enemies, true);
}

void Game::update(){

	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;

	bool quit = false;

	SDL_Event e;
	//ustawianie kontroli czasu
	TimeController::timeController.reset();
	//petla gry
	while (!quit){
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
		//usuwanie martwych przeciwnikow z listy
		Entity::removeInactiveEntitiesFromList(&enemies, true);

		//sprawdzanie czy zostal wcisniety przycisk
		while (SDL_PollEvent(&e)){
			//zamykanie okna
			if (e.type == SDL_QUIT)
				quit = true;
			//eventy z naciskaniem przyciskow
			if (e.type == SDL_KEYDOWN){
				switch (e.key.keysym.scancode){
				case SDL_SCANCODE_ESCAPE:
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (splashShowing)
						splashShowing = false;
					if (overlayTimer <= 0 && hero->hp < 1){
						//czyszczeie i restartowanie
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						overlayTimer = 2;
						enemyWavesTillBoss = 3;
						bossActive = false;
						buildBossNext = false;
						
						hpBar.entity = NULL;

						MultiBoss::multiKilled = 0;
						Kaska::kaskaKilled = 0;

						if (scoreTexture != NULL){
							cleanup(scoreTexture);
							scoreTexture = NULL;
						}

						//usuwanie WSZYSTKICH istniejacych przeciwnikow
						for (list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++){
							(*enemy)->active = false;
						}
						hero->revive();
					}
					
					break;

				}
			}
			heroInput.update(&e);
		}

		if (hero->hp < 1 && overlayTimer > 0){
			overlayTimer -= TimeController::timeController.dT;
		}

		//nadipsywanie entity
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){

			(*entity)->update();
		}

		//spawn przeciwnikow
		if (hero->hp > 0 && !splashShowing){
			if (enemiesToBuild == enemiesBuilt && enemies.size() <= 0){
				enemiesToBuild = enemiesToBuild + 2;
				enemiesBuilt = 0;
				enemyBuildTimer = 4;
				enemyWavesTillBoss--;

				if (enemyWavesTillBoss <= 0){
					buildBossNext = true;
				}
			}
			enemyBuildTimer -= TimeController::timeController.dT;

			if (!buildBossNext && !bossActive && enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10){
				Kaska*enemy = new Kaska(kaskaAnimSet);
				//ustawianie randomowych dancyh dla spawnowania przeciwnikow
				enemy->x = getRandomNumber(Window::ScreenWidth - (2 * 32) - 32) + 32 + 16;
				enemy->y = getRandomNumber(Window::ScreenHeight - (2 * 32) - 32) + 32 + 16;
				enemy->invincibleTimer = 0.1;

				enemies.push_back(enemy);
				Entity::entities.push_back(enemy);

				enemiesBuilt++;
				enemyBuildTimer = 1;
			}


			//BOSS
			if (buildBossNext && enemyBuildTimer <= 0 && enemies.size() == 0){
				MultiBoss*round = new MultiBoss(multiAnimSet, bulletAnimSet);
				round->invincibleTimer = 0.1;
				enemies.push_back(round);
				Entity::entities.push_back(round);


				hpBar.entity = round;

				bossActive = true;
				buildBossNext = false;
				enemyWavesTillBoss = 3;
			}

			if (bossActive && enemies.size() == 0){
				bossActive = false;
				buildBossNext = false;
				enemiesBuilt = 0;
				enemiesToBuild = 2;


				hpBar.entity = NULL;
			}
		}

		


		//rysowanie wszystkich entity
		draw();
	}

}
void Game::draw(){
	//czyszczenie okreanu
	SDL_SetRenderDrawColor(Window::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Window::renderer);

	if (splashShowing){
		renderTexture(splashImage, Window::renderer, 0, 0);
	}
	else{
		//rysowanie mapy
		renderTexture(backgroundImage, Window::renderer, 0, 0);


		Entity::entities.sort(Entity::EntityCompare);

		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			(*entity)->draw();
		}

		//rysowanie hpbar
		hpBar.draw();

		if (overlayTimer <= 0 && hero->hp < 1){


		}
		if (hero->hp > 0){
				//generaowanie wyniku
				SDL_Color color = { 255, 0, 0, 0 };//kolor czerowy wyswietlanego tekstu

				stringstream ss;
				ss << "ECTS:" << Kaska::kaskaKilled + MultiBoss::multiKilled;

				scoreTexture = renderText(ss.str(), "SEGOEUI.ttf", color, 30, Window::renderer);
	
			renderTexture(scoreTexture, Window::renderer, 20, 50);
		}
		if (hero->hp < 1) {
			renderTexture(diedTexture, Window::renderer, 0, 0);
		}
		if (hero->hp > 0 && MultiBoss::multiKilled)
		{
			renderTexture(endTexture, Window::renderer, 0, 0);
		}
	}
	//renderowanie na ekran
	SDL_RenderPresent(Window::renderer);
}