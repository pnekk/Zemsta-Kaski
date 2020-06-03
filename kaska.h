#ifndef GLOB
#define GLOB

#include <cstdlib>
#include "livingEntity.h"

class Kaska : public LivingEntity{
public:

	//animacje
	static const string KASKA_ANIM_UP;
	static const string KASKA_ANIM_DOWN;
	static const string KASKA_ANIM_LEFT;
	static const string KASKA_ANIM_RIGHT;
	static const string KASKA_ANIM_IDLE_UP;
	static const string KASKA_ANIM_IDLE_DOWN;
	static const string KASKA_ANIM_IDLE_LEFT;
	static const string KASKA_ANIM_IDLE_RIGHT;
	static const string KASKA_ANIM_ATTACK_UP;
	static const string KASKA_ANIM_ATTACK_DOWN;
	static const string KASKA_ANIM_ATTACK_LEFT;
	static const string KASKA_ANIM_ATTACK_RIGHT;
	static const string KASKA_ANIM_TELEGRAPH_UP;
	static const string KASKA_ANIM_TELEGRAPH_DOWN;
	static const string KASKA_ANIM_TELEGRAPH_LEFT;
	static const string KASKA_ANIM_TELEGRAPH_RIGHT;
	static const string KASKA_ANIM_DIE;

	//stany
	static const int KASKA_STATE_IDLE;
	static const int KASKA_STATE_MOVE;
	static const int KASKA_STATE_ATTACK;
	static const int KASKA_STATE_TELEGRAPH;
	static const int KASKA_STATE_DEAD;


	static int kaskaKilled;

	//jesli thinkTimer = 0, "mysl" nad akcja
	float thinkTimer = 0;
	//gonienie bohatera
	LivingEntity* target = NULL;

	int aiState = 0;

	Kaska(AnimationSet *animSet);
	void update();
	void think();
	void telegraph();
	void attack();
	void die();
	void findTarget();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages();
};

#endif