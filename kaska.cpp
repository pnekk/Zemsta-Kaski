#include "kaska.h"



int Kaska::kaskaKilled = 0;

Kaska::Kaska(AnimationSet *animSet){
	const string KASKA_ANIM_UP = "Up";
	const string KASKA_ANIM_DOWN = "Down";
	const string KASKA_ANIM_LEFT = "Left";
	const string KASKA_ANIM_RIGHT = "Right";
	const string KASKA_ANIM_IDLE_UP = "idle";
	const string KASKA_ANIM_IDLE_DOWN = "idle";
	const string KASKA_ANIM_IDLE_LEFT = "idle";
	const string KASKA_ANIM_IDLE_RIGHT = "idle";
	const string KASKA_ANIM_ATTACK_UP = "attackUp";
	const string KASKA_ANIM_ATTACK_DOWN = "attackDown";
	const string KASKA_ANIM_ATTACK_LEFT = "attackLeft";
	const string KASKA_ANIM_ATTACK_RIGHT = "attackRight";
	const string KASKA_ANIM_TELEGRAPH_UP = "telegraphUp";
	const string KASKA_ANIM_TELEGRAPH_DOWN = "telegraphDown";
	const string KASKA_ANIM_TELEGRAPH_LEFT = "telegraphLeft";
	const string KASKA_ANIM_TELEGRAPH_RIGHT = "telegraphRight";
	const string KASKA_ANIM_DIE = "die";

	const int KASKA_STATE_IDLE = 0;
	const int KASKA_STATE_MOVE = 1;
	const int KASKA_STATE_ATTACK = 2;
	const int KASKA_STATE_TELEGRAPH = 3;
	const int KASKA_STATE_DEAD = 4;

	const int DIR_UP = 0, DIR_DOWN = 1, DIR_LEFT = 2, DIR_RIGHT = 3, DIR_NONE = -1;
	this->animSet = animSet;

	type = "enemy";

	//domysle ustawienia
	x = Window::ScreenWidth / 2;
	y = Window::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 1;
	damage = 0;
	collisionBoxW = 18;
	collisionBoxH = 20;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -14;

	direction = DIR_DOWN;
	changeAnimation(KASKA_STATE_IDLE, true);

	updateCollisionBox();
}
void Kaska::update(){
	const int KASKA_STATE_IDLE = 0;
	const int KASKA_STATE_MOVE = 1;
	const int KASKA_STATE_ATTACK = 2;
	const int KASKA_STATE_TELEGRAPH = 3;
	const int KASKA_STATE_DEAD = 4;


	if (hp < 1 && state != KASKA_STATE_DEAD){
		changeAnimation(KASKA_STATE_DEAD, true);
		moving = false;
		die();
	}

	think();

	updateCollisionBox();
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}
void Kaska::think(){
	const int KASKA_STATE_IDLE = 0;
	const int KASKA_STATE_MOVE = 1;
	const int KASKA_STATE_ATTACK = 2;
	const int KASKA_STATE_TELEGRAPH = 3;
	const int KASKA_STATE_DEAD = 4;

	if (state == KASKA_STATE_IDLE || state == KASKA_STATE_MOVE){
		thinkTimer -= TimeController::timeController.dT;
		//wybieranie akcji
		if (thinkTimer <= 0){

			thinkTimer = rand() % 5;
			int action = rand() % 10;

			if (action < 3){
				moving = false;
				aiState = 0;
				changeAnimation(KASKA_STATE_IDLE, true);
			}
			else
			{
				findTarget();
				//cel znaleziony
				if (target != NULL && target->hp > 0){
					float dist = Entity::distanceBetweenTwoEntities(this, target);
					//jesli w zasiegu, atakuj
					if (dist < 100){
						telegraph();
						aiState = 0;

					}
					else
					{
						//jesli nie w zasiegu podejdz do gracza
						aiState = 1;
						moving = true;
						changeAnimation(KASKA_STATE_MOVE, state != KASKA_STATE_MOVE);
					}
				}
				else
				{
					moving = false;
					aiState = 0;
					changeAnimation(KASKA_STATE_IDLE, true);
				}
			}
			
		}
	}
	if (aiState == 1 && hp > 0 && active){
		angle = Entity::angleBetweenTwoEntities(this, target);
		move(angle);
	}
}
void Kaska::telegraph(){
	const int KASKA_STATE_TELEGRAPH = 3;
	moving = false;
	frameTimer = 0;
	changeAnimation(KASKA_STATE_TELEGRAPH, true);
}
void Kaska::attack(){

	const int KASKA_STATE_ATTACK = 2;
	const int KASKA_STATE_TELEGRAPH = 3;
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(KASKA_STATE_ATTACK, true);
}
void Kaska::die(){
	const int  KASKA_STATE_DEAD = 4;
	moving = false;
	state = KASKA_STATE_DEAD;
	changeAnimation(state, true);

	Kaska::kaskaKilled++;
}
void Kaska::findTarget(){
	target = NULL;
	for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
		if ((*entity)->type == "hero" && (*entity)->active){
			if (target == NULL){
				target = (LivingEntity*)(*entity);
			}
			else
			{

				if (Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target))
				{
					target = (LivingEntity*)(*entity);
				}
			}
		}
	}

}
void Kaska::changeAnimation(int newState, bool resetFrameToBeginning){
	const string KASKA_ANIM_UP = "Up";
	const string KASKA_ANIM_DOWN = "Down";
	const string KASKA_ANIM_LEFT = "Left";
	const string KASKA_ANIM_RIGHT = "Right";
	const string KASKA_ANIM_IDLE_UP = "idle";
	const string KASKA_ANIM_IDLE_DOWN = "idle";
	const string KASKA_ANIM_IDLE_LEFT = "idle";
	const string KASKA_ANIM_IDLE_RIGHT = "idle";
	const string KASKA_ANIM_ATTACK_UP = "attackUp";
	const string KASKA_ANIM_ATTACK_DOWN = "attackDown";
	const string KASKA_ANIM_ATTACK_LEFT = "attackLeft";
	const string KASKA_ANIM_ATTACK_RIGHT = "attackRight";
	const string KASKA_ANIM_TELEGRAPH_UP = "telegraphUp";
	const string KASKA_ANIM_TELEGRAPH_DOWN = "telegraphDown";
	const string KASKA_ANIM_TELEGRAPH_LEFT = "telegraphLeft";
	const string KASKA_ANIM_TELEGRAPH_RIGHT = "telegraphRight";
	const string KASKA_ANIM_DIE = "die";

	const int KASKA_STATE_IDLE = 0;
	const int KASKA_STATE_MOVE = 1;
	const int KASKA_STATE_ATTACK = 2;
	const int KASKA_STATE_TELEGRAPH = 3;
	const int KASKA_STATE_DEAD = 4;

	state = newState;
	const int DIR_UP = 0, DIR_DOWN = 1, DIR_LEFT = 2, DIR_RIGHT = 3, DIR_NONE = -1;
	if (state == KASKA_STATE_IDLE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(KASKA_ANIM_IDLE_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(KASKA_ANIM_IDLE_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_IDLE_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_IDLE_RIGHT);
	}
	else if (state == KASKA_STATE_MOVE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(KASKA_ANIM_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(KASKA_ANIM_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_RIGHT);
	}
	else if(state == KASKA_STATE_ATTACK){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(KASKA_ANIM_ATTACK_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(KASKA_ANIM_ATTACK_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_ATTACK_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_ATTACK_RIGHT);
	}
	else if (state == KASKA_STATE_TELEGRAPH){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(KASKA_ANIM_TELEGRAPH_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(KASKA_ANIM_TELEGRAPH_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_TELEGRAPH_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(KASKA_ANIM_TELEGRAPH_RIGHT);
	}
	else if (state == KASKA_STATE_DEAD){
			currentAnim = animSet->getAnimation(KASKA_ANIM_DIE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}
void Kaska::updateAnimation(){
		const int KASKA_STATE_IDLE = 0;
	const int KASKA_STATE_MOVE = 1;
	const int KASKA_STATE_ATTACK = 2;
	const int KASKA_STATE_TELEGRAPH = 3;
	const int KASKA_STATE_DEAD = 4;

	if (currentFrame == NULL || currentAnim == NULL)
		return;

	if (state == KASKA_STATE_MOVE && !moving){
		changeAnimation(KASKA_STATE_IDLE, true);
	}
	if (state != KASKA_STATE_MOVE && moving){
		changeAnimation(KASKA_STATE_MOVE, true);
	}

	frameTimer += TimeController::timeController.dT;
	if (frameTimer >= currentFrame->duration){
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()){
			if (state == KASKA_STATE_TELEGRAPH){
				attack();
			}
			else if (state == KASKA_STATE_ATTACK){
				changeAnimation(KASKA_STATE_MOVE, true);
			}
			else if (state == KASKA_STATE_DEAD){
				frameTimer = 0;

				if (hp > 0)
					changeAnimation(KASKA_STATE_MOVE, true);
				else
					active = false; 
			}
			else
			{

				currentFrame = currentAnim->getFrame(0);
			}
		}
		else{

			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}
void Kaska::updateDamages(){
	if (active && hp > 0 && invincibleTimer <= 0){
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			if ((*entity)->active && (*entity)->type == "hero"){

				LivingEntity* enemy = (LivingEntity*)(*entity);

				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)){
					enemy->hitLanded(this); 
					hp -= enemy->damage;

					if (hp > 0){
						invincibleTimer = 0.1;
					}

					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 300;
				}
			}
		}
	}
}