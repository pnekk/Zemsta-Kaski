#include "entity.h"



void Entity::update() { ; }
void Entity::draw() {
	//rysowanie obecnej klatki
	if (currentFrame != NULL && active){
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	//rysowanie collsionBox
	if (solid && Window::debugging){
		//ustawia jaki kolor na byc renderowany
		SDL_SetRenderDrawColor(Window::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Window::renderer, &collisionBox);
	}
}

void Entity::move(float angle){
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDirection = angleToDirection(angle);
	//jesli zmienil sie kierunek patrzenia postaci, zmien animacje
	if (direction != newDirection){
		direction = newDirection;
		changeAnimation(state, false);
	}
}
void Entity::updateMovement(){
	updateCollisionBox();
	//przechowuje collisiobox zanim sie ruszymy
	lastCollisionBox = collisionBox;

	//resetowanie wszystkich krokow
	totalXMove = 0;
	totalYMove = 0;

	if (moveSpeed > 0){

		float moveDist = moveSpeed*(TimeController::timeController.dT)*4; 
		if (moveDist > 0)
		{
			//xmove - dystans w radianach
			float xMove = moveDist*(cos(angle*3.14/ 180));
			//ymove - dystans w radianach
			float yMove = moveDist*(sin(angle*3.14/ 180));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if (!moving)
				moveSpeed -= moveDist;
		}
	}
	//slizganie sie po powierzchni
	if (slideAmount > 0){
		float slideDist = slideAmount*TimeController::timeController.dT*4;
		if (slideDist > 0){
			float xMove = slideDist*(cos(slideAngle*3.14 / 180));
			float yMove = slideDist*(sin(slideAngle*3.14 / 180));

			x += xMove;
			y += yMove;

			totalXMove += xMove;
			totalYMove += yMove;
			slideAmount -= slideDist;
		}
		else
		{
			slideAmount = 0;
		}
	}
	//przesuwanie collsionBox wraz z entiyy
	updateCollisionBox();
	//pomoc w wyszukiwaniu kolizji
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}
void Entity::updateCollisionBox(){
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y + collisionBoxYOffset;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
}

void Entity::updateCollisions(){
	if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0)){
		//lista potencjalnych kolizji
		list<Entity*> collisions;


		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			//jesli entity koliduje z obiektem ktory z collsionBox
			if ((*entity)->active 
				&& (*entity)->solid 
				&& Entity::checkCollision(collisionBox, (*entity)->collisionBox)){

				//dodawanie do listy
				collisions.push_back(*entity);

			}
		}
		if (collisions.size() > 0){
			updateCollisionBox();

			float collisionTime = 1; // 1 oznacza brak kolizji
			float normalX, normalY; //uzywane w okreleniu w jakim miescu "zderzylismy" sie

			//collisionBox zanim entity sie poruszy
			SDL_Rect startingCollisionBox = lastCollisionBox;

			//petla w entiy z listy
			for (auto entity = collisions.begin(); entity != collisions.end(); entity++){
				////tymczasowe zmienne dla x,y i collisionTime
				float tmpNormalX, tmpNormalY;
				float tmpCollisionTime = SweptAABB(startingCollisionBox, totalXMove, totalYMove, (*entity)->collisionBox, tmpNormalX, tmpNormalY);

				if (tmpCollisionTime < collisionTime){
					collisionTime = tmpCollisionTime;
					normalX = tmpNormalX;
					normalY = tmpNormalY;
				}
			}
			// jesli wystapila kolizja, "wyslizgnijmy" sie z niej
			if (collisionTime < 1.0f){
				if (dieOnSolids)
				{
					crashOntoSolid();
				}

				//przesuwanie collisionBox do pozycji gdzie jest kolizja
				startingCollisionBox.x += totalXMove * collisionTime;
				startingCollisionBox.y += totalYMove * collisionTime;

				float remainingTime = 1.0f - collisionTime;
				x = startingCollisionBox.x + startingCollisionBox.w / 2;
				y = startingCollisionBox.y - collisionBoxYOffset;

				float dotProd = (totalXMove * normalY + totalYMove * normalX)*remainingTime;
				totalXMove = dotProd * normalY;
				totalYMove = dotProd * normalX;
				x += totalXMove;
				y += totalYMove;
				lastCollisionBox = startingCollisionBox;
				updateCollisionBox();

				//slizgajac sie mozemy wpasc na inne obiekty z ktorymi mozemy kolidowac wiec wywolaj funkcje znowu
				updateCollisions();

			}
		}
	}
}


float Entity::SweptAABB(SDL_Rect movingBox, float vx, float vy, SDL_Rect otherBox, float &normalX, float &normalY){

	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	//znajdz dystans pomiedzy obiektami
	if (vx > 0.0f){
		xInvEntry = otherBox.x - (movingBox.x + movingBox.w);
		xInvExit = (otherBox.x + otherBox.w) - movingBox.x;
	}
	else
	{
		xInvEntry = (otherBox.x + otherBox.w) - movingBox.x;
		xInvExit = otherBox.x - (movingBox.x + movingBox.w);
	}

	if (vy > 0.0f){
		yInvEntry = otherBox.y - (movingBox.y + movingBox.h);
		yInvExit = (otherBox.y + otherBox.h) - movingBox.y;
	}
	else
	{
		yInvEntry = (otherBox.y + otherBox.h) - movingBox.y;
		yInvExit = otherBox.y - (movingBox.y + movingBox.h);
	}

	//znajdz czas kolizji i przeciecie dla kazdej osi
	float xEntry, yEntry;
	float xExit, yExit;

	if (vx == 0.0f){
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / vx;
		xExit = xInvExit / vx;
	}

	if (vy == 0.0f){
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / vy;
		yExit = yInvExit / vy;
	}

	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	//jesli nie bylo kolizji
	if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f){
		normalX = 0.0f; 
		normalY = 0.0f; 
		return 1.0f; //1 oznaczna brak kolizji
	}
	else
	{
		//w innym przypadku wysapila kozlizja
		if (xEntry > yEntry){
			if (xInvEntry < 0.0f){
				normalX = 1; //oderzenie po prawej stronie
				normalY = 0;//brak uderzenia od gory czy od dolu
			}
			else
			{
				normalX = -1; //oderzenie po lewej stronie
				normalY = 0;//brak uderzenia od gory czy od dolu
			}
		}
		else
		{
			if (yInvEntry < 0.0f){
				normalX = 0;
				normalY = 1;
			}
			else
			{
				normalX = 0;
				normalY = -1;
			}
		}

		return entryTime;
	}
}


float Entity::distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2){
	SDL_Point e1, e2;
	e1.x = r1.x + r1.w / 2;
	e1.y = r1.y + r1.h / 2;

	e2.x = r2.x + r2.w / 2;
	e2.y = r2.y + r2.h / 2;

	float d = abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2)));
	return d;
}
float Entity::distanceBetweenTwoEntities(Entity *e1, Entity *e2){
	float d = abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));
	return d;
}
float Entity::distanceBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2){
	float d = abs( sqrt( pow(cx2 - cx1, 2) + pow(cy2 - cy1, 2) ) );
	return d;
}

float Entity::angleBetweenTwoEntities(Entity *e1, Entity *e2){
	float dx, dy;
	float x1 = e1->x, y1 = e1->y, x2 = e2->x, y2 = e2->y;
	
	dx = x2 - x1;
	dy = y2 - y1;

	return atan2(dy, dx) * 180 / 3.14;
}
bool Entity::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2){
	SDL_Rect intersection;
	if (SDL_IntersectRect(&cbox1, &cbox2, &intersection))
	{
		return true;
	}


	return false;
}
int Entity::angleToDirection(float angle){
	const int DIR_UP = 0, DIR_DOWN = 1, DIR_LEFT = 2, DIR_RIGHT = 3, DIR_NONE = -1;
	if ((angle >= 0 && angle <= 45) || angle >= 315 && angle <= 360)
		return DIR_RIGHT;
	else if (angle >= 45 && angle <= 135)
		return DIR_DOWN;
	else if (angle >= 135 && angle <= 225)
		return DIR_LEFT;
	else
		return DIR_UP;
}
float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2){
	float dx = cx2 - cx1;
	float dy = cy2 - cy1;

	return atan2(dy, dx) * 180 / 3.14;
}
float Entity::angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2){
	float cx1 = r1.x + (r1.w / 2);
	float cy1 = r1.y + (r1.h / 2);

	float cx2 = r2.x + (r2.w / 2);
	float cy2 = r2.y + (r2.h / 2);

	return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);
}


list<Entity*> Entity::entities;
bool Entity::EntityCompare(const Entity* const &a, const Entity * const &b){
	if (a != 0 && b != 0)
	{
		return (a->y < b->y);
	}
	else
	{
		return false;
	}
}
void Entity::removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities){
	for (auto entity = entityList->begin(); entity != entityList->end();){
		//jezeli entity jest nieaktywne, usun je
		if (!(*entity)->active){
			if (deleteEntities)
				delete (*entity);
			entity = entityList->erase(entity);
		}
		else
		{
			entity++;
		}
	}
}
void Entity::removeAllFromList(list<Entity*> *entityList, bool deleteEntities){
	for (auto entity = entityList->begin(); entity != entityList->end();){
		if (deleteEntities)
			delete (*entity);
		entity = entityList->erase(entity);
	}
}