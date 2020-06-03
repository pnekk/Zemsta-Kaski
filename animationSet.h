#ifndef ANIMATIONSET
#define ANIMATIONSET

#include "animation.h"

using namespace std;

class AnimationSet{
public:
	string imageName;
	SDL_Texture* spriteSheet; //przechowywanie pliku z animacjami
	SDL_Texture* whiteSpriteSheet = NULL; //plik do wyswietlania zadanych obrazen (na bosie)
	list<Animation> animations;

	~AnimationSet(); //czyszczenie pamieci z niepotrzebnych tekstur

	Animation* getAnimation(string name);

	void loadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
};

#endif