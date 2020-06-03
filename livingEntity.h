#ifndef LIVINGENTITY
#define LIVINGENTITY

#include "entity.h"


class LivingEntity : public Entity{
public:
	int hp, hpMax;
	int damage = 0;
	SDL_Rect hitBox; //podobna struktura do collisinoBox, ale opisuje zadane przestrzen w ktorej zadawne sa (obiektowi) obrazenia

	float invincibleTimer = 0;

	virtual void updateHitBox();
	virtual void updateDamages() = 0; //jak otrzymujemy obrazenia
	virtual void die() = 0;
	virtual void updateInvincibleTimer();
	virtual void hitLanded(LivingEntity* entity){ ; }

	void draw(); //nadpisywanie danych z entity
};

#endif