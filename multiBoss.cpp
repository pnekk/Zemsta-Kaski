#include "multiBoss.h"
#include "game.h"


MultiBoss::MultiBoss(AnimationSet* animSet, AnimationSet* bulletAnimSet){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	this->animSet = animSet;
	this->bulletAnimSet = bulletAnimSet;

	type = "enemy";

	x = Window::ScreenWidth / 2;
	y = Window::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 4;
	damage = 0;
	collisionBoxW = 50;
	collisionBoxH = 30;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -14;

	changeAnimation(MULTI_STATE_IDLE, true);

	updateCollisionBox();
}
void MultiBoss::update(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	//sprawdzenie czy martwy

	if (hp < 1 && state != MULTI_STATE_DEAD){
		changeAnimation(MULTI_STATE_DEAD, true);
		moving = false;
		die();
	}


	//myslenie co robic
	think();

	updateShoot();

	updateCollisionBox();

	updateMovement();

	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();

	updateInvincibleTimer();
}

void MultiBoss::updateShoot(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	if (state == MULTI_STATE_SHOOT){
		shootTimer -= TimeController::timeController.dT;
		shotTimer -= TimeController::timeController.dT;

		if (shootTimer <= 0)
		{
			changeAnimation(MULTI_STATE_IDLE, true);
		}
		else if (shotTimer <= 0)
		{
			shotTimer = 2.5;

			Bullet* bullet = new Bullet(bulletAnimSet, x, y);
			bullet->angle = angle;
			Entity::entities.push_back(bullet);
		}
	}
}
void MultiBoss::think(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	findTarget(); 

	if (target != NULL){
		if (state == MULTI_STATE_IDLE)
			thinkTimer -= TimeController::timeController.dT;

		angle = Entity::angleBetweenTwoEntities(this, target);

		//sprawdzanie w ktorej fazie jestesmy
		if (hp > 3)
			aiState = 1;
		else if (hp > 1)
			aiState = 2;
		else
			aiState = 3;

		//myslenie co robic dalej
		if (thinkTimer <= 0 && state == MULTI_STATE_IDLE){
			//restartowanie animacji
			frameTimer = 0;

			//normalna faza bosa
			if (aiState == 1){
				thinkTimer = 2;

				//wybeirania ataku
				int action = getRandomNumber(4);
				if (action % 2 == 0){
					slam();
				}
			}
			else if (aiState == 2)//faza damaged
			{
				thinkTimer = 1.5;

				//wybeirania ataku
				int action = getRandomNumber(6);
				if (action < 2)
					slam();
			}
			else //faza frantic 
			{

				thinkTimer = 1;

				int action = getRandomNumber(4);
			}
		}
	}
	else
	{
		//brak celu
		moving = 0;
		changeAnimation(MULTI_STATE_IDLE, (state != MULTI_STATE_IDLE));
	}
}

void MultiBoss::shoot(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	moving = false;

	//5 sekund strzelania w gracza
	shootTimer = 5;
	//ustawienie szybkosci strzalu
	shotTimer = 0;
	//szybsze strzelanie w fazie dmged i frantic
	if (aiState != 1){
		shootTimer = 3;
	}

	changeAnimation(MULTI_STATE_SHOOT, true);
}
void MultiBoss::slam(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	moving = false;
	changeAnimation(MULTI_STATE_SLAM, true);
}

void MultiBoss::jump(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(MULTI_STATE_JUMP, true);
}
void MultiBoss::die(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;

	moving = false;
	changeAnimation(MULTI_STATE_DEAD, true);

	multiKilled++;
}
void MultiBoss::findTarget(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;


	target = NULL;
	//znajdywanie obiektu
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
void MultiBoss::changeAnimation(int newState, bool resetFrameToBeginning){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	state = newState;

	if (state == MULTI_STATE_IDLE)
		currentAnim = animSet->getAnimation(MULTI_ANIM_IDLE);
	if (state == MULTI_STATE_SHOOT)
		currentAnim = animSet->getAnimation(MULTI_ANIM_SHOOT);
	if (state == MULTI_STATE_SLAM)
		currentAnim = animSet->getAnimation(MULTI_ANIM_SLAM);

	if (state == MULTI_STATE_JUMP)
		currentAnim = animSet->getAnimation(MULTI_ANIM_JUMP);
	if (state == MULTI_STATE_DEAD)
		currentAnim = animSet->getAnimation(MULTI_ANIM_DIE);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}
void MultiBoss::updateAnimation(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	frameTimer += TimeController::timeController.dT;
	if (frameTimer >= currentFrame->duration)
	{
		//jesli koniec animacji
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()){
			if (state == MULTI_STATE_IDLE)
				shoot();
			else if (state == MULTI_STATE_SLAM || state == MULTI_STATE_JUMP)
				changeAnimation(MULTI_STATE_IDLE, true);
			else if (state == MULTI_STATE_DEAD){
				frameTimer = 0;
				if (hp > 0)
					changeAnimation(MULTI_STATE_IDLE, true);
				else
					active = false; 
			}
		}
		else
		{ 
			currentFrame = currentAnim->getNextFrame(currentFrame);

			GroupNumber* damages = (GroupNumber*)GroupBuilder::findGroupByName("damage", currentFrame->frameData);
			if (damages != NULL && damages->numberOfDataInGroup() > 0)
			{
			}

		}
		frameTimer = 0;
	}
}
void MultiBoss::updateDamages(){
	const string MULTI_ANIM_IDLE = "idle";
	const string MULTI_ANIM_SHOOT = "shoot";
	const string MULTI_ANIM_SLAM = "slam";
	const string MULTI_ANIM_JUMP = "jump";
	const string MULTI_ANIM_DIE = "die";


	const int MULTI_STATE_IDLE = 1;
	const int MULTI_STATE_SHOOT = 3;
	const int MULTI_STATE_SLAM = 4;
	const int MULTI_STATE_JUMP = 6;
	const int MULTI_STATE_DEAD = 7;
	if (active && hp > 0 && invincibleTimer <= 0){
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			if ((*entity)->active && (*entity)->type == "hero")
			{
				LivingEntity* enemy = (LivingEntity*)(*entity);
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)){
					enemy->hitLanded(this);
					hp -= enemy->damage;

					if (hp > 0){

						invincibleTimer = 0.1;
					}
				}

			}
		}
	}
}

int MultiBoss::multiKilled = 0;