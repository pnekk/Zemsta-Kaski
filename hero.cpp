#include "hero.h"



Hero::Hero(AnimationSet *animSet){
	const string HERO_ANIM_UP = "Up";
	const string HERO_ANIM_DOWN = "Down";
	const string HERO_ANIM_LEFT = "Left";
	const string HERO_ANIM_RIGHT = "Right";
	const string HERO_ANIM_IDLE_UP = "idleUp";
	const string HERO_ANIM_IDLE_DOWN = "idleDown";
	const string HERO_ANIM_IDLE_LEFT = "idleLeft";
	const string HERO_ANIM_IDLE_RIGHT = "idleRight";
	const string HERO_SLASH_ANIM_UP = "slashUp";
	const string HERO_SLASH_ANIM_DOWN = "slashDown";
	const string HERO_SLASH_ANIM_LEFT = "slashLeft";
	const string HERO_SLASH_ANIM_RIGHT = "slashRight";
	const string HERO_ANIM_DIE = "die";

	const int HERO_STATE_IDLE = 0;
	const int HERO_STATE_MOVE = 1;
	const int HERO_STATE_SLASH = 2;
	const int HERO_STATE_DEAD = 4;
	const int DIR_UP = 0, DIR_DOWN = 1, DIR_LEFT = 2, DIR_RIGHT = 3, DIR_NONE = -1;
	this->animSet = animSet;
	type = "hero";

	//ustawianie
	x = Window::ScreenWidth / 2;
	y = Window::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 80;
	hp = hpMax = 20;
	damage = 0;
	collisionBoxW = 20;
	collisionBoxH = 24;
	collisionBoxYOffset = -20;

	direction = DIR_DOWN;

	changeAnimation(HERO_STATE_IDLE, true);

	updateCollisionBox();
}
void Hero::update(){
	const int HERO_STATE_IDLE = 0;
	const int HERO_STATE_MOVE = 1;
	const int HERO_STATE_SLASH = 2;
	const int HERO_STATE_DEAD = 4;
	if (hp < 1 && state != HERO_STATE_DEAD){
		changeAnimation(HERO_STATE_DEAD, true);
		moving = false;
		die();
	}
	
	updateCollisionBox(); 
	updateMovement();
	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}
void Hero::slash(){
	const int HERO_STATE_IDLE = 0;
	const int HERO_STATE_MOVE = 1;
	const int HERO_STATE_SLASH = 2;
	const int HERO_STATE_DEAD = 4;
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE))
	{
		moving = false;
		frameTimer = 0;
		changeAnimation(HERO_STATE_SLASH, true);
	}
}
void Hero::die(){
	const int HERO_STATE_IDLE = 0;
	const int HERO_STATE_MOVE = 1;
	const int HERO_STATE_SLASH = 2;
	const int HERO_STATE_DEAD = 4;
	moving = false;
	changeAnimation(HERO_STATE_DEAD, true);
}
void Hero::revive(){
	const int HERO_STATE_IDLE = 0;
	const int HERO_STATE_MOVE = 1;
	const int HERO_STATE_SLASH = 2;
	const int HERO_STATE_DEAD = 4;
	hp = hpMax;
	changeAnimation(HERO_STATE_IDLE, true);
	moving = false;
	x = Window::ScreenWidth / 2;
	y = Window::ScreenHeight / 2;
	slideAmount = 0;
}
void Hero::changeAnimation(int newState, bool resetFrameToBeginning){
	const string HERO_ANIM_UP = "Up";
	const string HERO_ANIM_DOWN = "Down";
	const string HERO_ANIM_LEFT = "Left";
	const string HERO_ANIM_RIGHT = "Right";
	const string HERO_ANIM_IDLE_UP = "idleUp";
	const string HERO_ANIM_IDLE_DOWN = "idleDown";
	const string HERO_ANIM_IDLE_LEFT = "idleLeft";
	const string HERO_ANIM_IDLE_RIGHT = "idleRight";
	const string HERO_SLASH_ANIM_UP = "slashUp";
	const string HERO_SLASH_ANIM_DOWN = "slashDown";
	const string HERO_SLASH_ANIM_LEFT = "slashLeft";
	const string HERO_SLASH_ANIM_RIGHT = "slashRight";
	const string HERO_ANIM_DIE = "die";

	const int HERO_STATE_IDLE = 0;
	const int HERO_STATE_MOVE = 1;
	const int HERO_STATE_SLASH = 2;
	const int HERO_STATE_DEAD = 4;
	state = newState;
	const int DIR_UP = 0, DIR_DOWN = 1, DIR_LEFT = 2, DIR_RIGHT = 3, DIR_NONE = -1;
	if (state == HERO_STATE_IDLE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_DOWN);
		else if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_UP);
		else if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_RIGHT);
	}
	else if (state == HERO_STATE_MOVE){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(HERO_ANIM_DOWN);
		else if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(HERO_ANIM_UP);
		else if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(HERO_ANIM_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(HERO_ANIM_RIGHT);
	}
	else if (state == HERO_STATE_SLASH){
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_DOWN);
		else if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_UP);
		else if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_LEFT);
		else if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_RIGHT);
	}
	else if (state == HERO_STATE_DEAD){
		currentAnim = animSet->getAnimation(HERO_ANIM_DIE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}
void Hero::updateAnimation(){
	const int HERO_STATE_IDLE = 0;
	const int HERO_STATE_MOVE = 1;
	const int HERO_STATE_SLASH = 2;
	const int HERO_STATE_DEAD = 4;
	if (currentFrame == NULL || currentAnim == NULL)
		return; 


	if (state == HERO_STATE_MOVE && !moving){
		changeAnimation(HERO_STATE_IDLE, true);
	}

	if (state != HERO_STATE_MOVE && moving){
		changeAnimation(HERO_STATE_MOVE, true);
	}
	

	frameTimer += TimeController::timeController.dT;

	if (frameTimer >= currentFrame->duration)
	{

		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()){
			if (state == HERO_STATE_SLASH){

				changeAnimation(HERO_STATE_MOVE, true);
			}
			else if (state == HERO_STATE_DEAD && hp > 0){

				changeAnimation(HERO_STATE_MOVE, true);
			}
			else{

				currentFrame = currentAnim->getFrame(0);
			}
		}
		else
		{

			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}
void Hero::updateDamages(){
	if (active && hp > 0 && invincibleTimer <= 0){
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			if ((*entity)->active && (*entity)->type == "enemy"){

				LivingEntity* enemy = (LivingEntity*)(*entity);

				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)){
					enemy->hitLanded(this);
					hp -= enemy->damage;


					if (hp > 0){
						invincibleTimer = 0.3;
					}

					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 200;
				}
			}
		}
	}
}