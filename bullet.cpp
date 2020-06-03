#include "bullet.h"


Bullet::Bullet(AnimationSet* animSet, int x, int y){
	const int DIR_UP = 0, DIR_DOWN = 1, DIR_LEFT = 2, DIR_RIGHT = 3, DIR_NONE = -1;
	const string BULLET_ANIM_BULLET = "bullet";
	this->animSet = animSet;

	solid = false;
	collideWithSolids = true;
	dieOnSolids = true;
	moving = true;
	type = "enemy";

	this->x = x;
	this->y = y;
	moveSpeed = 40;
	moveSpeedMax = 40;
	hp = hpMax = 1;
	damage = 5;
	collisionBoxW = 5;
	collisionBoxH = 5;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
	collisionBoxYOffset = 0;

	direction = DIR_DOWN;

	changeAnimation(0, true);

	updateCollisionBox();

	invincibleTimer = 0;
}
void Bullet::update(){
	updateCollisionBox();

	move(angle);
	updateMovement();

	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();

	
}
void Bullet::changeAnimation(int newState, bool resetFrameToBeginning){
	const string BULLET_ANIM_BULLET = "bullet";
	state = newState;
	currentAnim = animSet->getAnimation(BULLET_ANIM_BULLET);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}
void Bullet::updateAnimation(){
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	//jesli sa klatki, aktualizuj frameTimer i obecna animacje
	frameTimer += TimeController::timeController.dT;

	if (frameTimer >= currentFrame->duration){
		//jesli koniec klatki, zrestartuj ja
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
			currentFrame = currentAnim->getFrame(0);
		else
			currentFrame = currentAnim->getNextFrame(currentFrame);

		frameTimer = 0;
	}
}
void Bullet::hitLanded(LivingEntity* entity){
	//dostalismy kula, kula znika
	active = false;

}
void Bullet::crashOntoSolid(){
	//kula uderza w obiekt typu solid, znisz ja
	active = false;

}