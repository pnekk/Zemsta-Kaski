#include "animationSet.h"

AnimationSet::~AnimationSet(){
	cleanup(spriteSheet);
	if (whiteSpriteSheet != NULL)
		cleanup(whiteSpriteSheet);
}

Animation* AnimationSet::getAnimation(string name){
	for (list<Animation>::iterator animation = animations.begin(); animation != animations.end(); animation++){
		Animation* anim = &(*animation);


		if (name == anim->name)
		{
			return anim;
		}
	}

	return NULL;
}

//zaladowywanie parametrow animacji z pliku
void AnimationSet::loadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture){

	ifstream file;
	file.open(fileName);
	if (file.good())
	{
		getline(file, imageName);
		if (setColourKey)
		{
			SDL_Surface* spriteSurface = loadSurface(imageName, Window::renderer);

			

			spriteSheet = convertSurfaceToTexture(spriteSurface, Window::renderer, false);

			if (createWhiteTexture)
			{
				SDL_Surface* whiteSurface = loadSurface("allwhite.png", Window::renderer);
				surfacePaletteSwap(spriteSurface, whiteSurface);
			
				whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Window::renderer, false); //tworzenie tekstury podzas niszczenie powierzchni

				cleanup(whiteSurface);
			}
			else{
				whiteSpriteSheet = NULL;
			}

			cleanup(spriteSurface);
		}
		else
			spriteSheet = loadTexture(imageName, Window::renderer);

		string buffer;
		getline(file, buffer);
		stringstream ss;
		buffer = Window::clipOffDataHeader(buffer);
		ss << buffer;
		int numberOfAnimations;
		ss >> numberOfAnimations;

		for (int i = 0; i < numberOfAnimations; i++)
		{
			Animation newAnimation;
			newAnimation.loadAnimation(file, groupTypes);
			animations.push_back(newAnimation);
		}

	}
	file.close();


}