#ifndef ROUND_KING
#define ROUND_KING

#include <cstdlib>
#include "livingEntity.h"
#include "bullet.h"

class MultiBoss : public LivingEntity{
public:
	//animacje
	static const string MULTI_ANIM_IDLE;
	static const string MULTI_ANIM_SHOOT;
	static const string MULTI_ANIM_SLAM;
	static const string MULTI_ANIM_JUMP;
	static const string MULTI_ANIM_DIE;

	//stany
	static const int MULTI_STATE_IDLE;
	static const int MULTI_STATE_SHOOT;
	static const int MULTI_STATE_SLAM;
	static const int MULTI_STATE_JUMP;
	static const int MULTI_STATE_DEAD;

	//fazy


	//analogicznie jak w przypadku inego ai - jesli thinkTimer = 0, podejmi nowa akcje
	float thinkTimer = 0;
	//jak dlugo ma strzelac boss?
	float shootTimer = 0;
	//ile czasu ma byc pomiedzy wystrzelonymi pociskami
	float shotTimer = 0;
	LivingEntity* target = NULL;

	//obenca "faza" bossa
	int aiState = 1;

	//odnosnik do animacji strzelajacych kul
	AnimationSet* bulletAnimSet;

	static int multiKilled;

	MultiBoss(AnimationSet* animSet, AnimationSet* bulletAnimSet);
	void update();
	void updateShoot();
	void think();
	void shoot();
	void slam();
	void jump();
	void die();
	void findTarget();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages();


};

#endif