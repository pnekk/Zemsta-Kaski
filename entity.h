#ifndef ENTITY
#define ENTITY

#include <limits>
#include <algorithm>
#include "window.h"
#include "timeController.h"
#include "animationSet.h"

//klasa abstrakcyjna
class Entity{
public:
	//odwolanie sie do kierunkow poruszania sie
	static const int DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_NONE;

	//jaki stan ma entity? (porusza sie, stoi w miejscu, atakuje..)
	int state;

	float x, y;
	int direction;
	bool solid = true; //czy obiekt jest typu stalego?
	bool collideWithSolids = true; //obiekt typu solid moze oddzialywac z innym obiektem typu solid
	bool dieOnSolids = false; //uzywane gdy obiekt ma sie niszczyc na solidnych obiektach (np. kule na scianach)
	bool active = true; //czy entity jest uzywane?
	string type = "entity"; //rodzaje entity: przeciwnik, sciana, bohater
	bool moving; //czy entity porusza sie?
	float angle; //kat do poruszania entity
	float moveSpeed; //predkosc poczatkowa
	float moveSpeedMax; //maksymalna predkosc
	float slideAngle; //w ktorym kierunku bohater sie sliga?
	float slideAmount; //"ilosc" slizgania sie, czyli dystans pokany przez slizganie sie
	float totalXMove, totalYMove; //suma ruchow po osiach X i Y

	SDL_Rect collisionBox; // "skrzynka" uzywana do okrelania wielkosci entity, uzywane do kolziji z innymi entity
	SDL_Rect lastCollisionBox; //gdzie collisionbox zdajdowal sie ostatnio? uzywane do poprawnego przemieszczania sie collisionBox z entity
	int collisionBoxW, collisionBoxH; //szerokosc i wysokosc collisionBox
	float collisionBoxYOffset; //od wartosci Y gdzie powinien zaczynac sie collision box

	AnimationSet *animSet; //wszystkie animacje jakie entity posiada
	Animation *currentAnim; //cobecna animacja jaka entity uzywa
	Frame *currentFrame; //obecna klatka jaka entity uzywa
	float frameTimer; //pomaga w animacji klaki w klatke

	virtual void update();
	virtual void draw();

	virtual void move(float angle);
	virtual void updateMovement();
	virtual void updateCollisionBox();

	virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0;
	virtual void updateCollisions(); //uderzanie w obiekty
	virtual void crashOntoSolid(){ ; }//niszczenie posciskow ktore uderzaja w obiekty solid (implementacja dieOnSolid


	static float SweptAABB(SDL_Rect movingBox, float vx, float vy, SDL_Rect otherBox, float &normalX, float &normalY);
	static float distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
	static float distanceBetweenTwoEntities(Entity *e1, Entity *e2);
	static float distanceBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBetweenTwoEntities(Entity *e1, Entity *e2);
	static bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2);
	static int angleToDirection(float angle);
	static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);

	static list<Entity*> entities;
	static bool EntityCompare(const Entity* const &a, const Entity * const &b); //porownywanie 2 entity z listy zeby pomoc w sortowaniu
	static void removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities);
	static void removeAllFromList(list<Entity*> *entityList, bool deleteEntities);
};

#endif 