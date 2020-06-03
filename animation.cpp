#include "animation.h"

Animation::Animation(string name)
{
	this->name = name;
}

int Animation::getNextFrameNumber(int frameNumber){
	if (frameNumber + 1 < frames.size())
		return frameNumber + 1;
	else
		return 0;
}
Frame* Animation::getNextFrame(Frame* frame){
	return getFrame(getNextFrameNumber(frame->frameNumber));
}
int Animation::getEndFrameNumber(){
	return frames.size() - 1;
}
Frame* Animation::getFrame(int frameNumber){
	if (frames.size() == 0)
		return NULL;

	list<Frame>::iterator i = frames.begin(); //wskazywanie iteratorowi pierwszej klatki z listy

	int counter = 0;

	for (counter = 0; counter < frameNumber && counter < frames.size() - 1; counter++){
		i++; //interator wskazuje nastepna klatke z listy
	}

	Frame *frame = &(*i); //klatka wskazuje na klatke wewnatrz listy na ktora wskazuje iterator


	return frame;
}

void Animation::loadAnimation(ifstream &file, list<DataGroupType> &groupTypes){
	getline(file, name);
	string buffer;
	getline(file, buffer);
	stringstream ss;
	buffer = Window::clipOffDataHeader(buffer);
	ss << buffer;
	int numberOfFrames;
	ss >> numberOfFrames;
	for (int i = 0; i < numberOfFrames; i++){
		Frame newFrame;
		newFrame.loadFrame(file, groupTypes);
		frames.push_back(newFrame);
	}


}