/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSpriteType.cpp

	See AnimatedSpriteType.h for a class description.
*/

#include "stdafx.h"
#include "AnimatedSpriteType.h"

/*
	AnimatedSpriteType - Default constructor, it constructs the data
	structures necessary for storing the sprite images and animation
	sequences.
*/
AnimatedSpriteType::AnimatedSpriteType()  
{
	textureIDs = new vector<int>();
	animationSequences = new vector<vector<int>*>();
	animationSequencesNames = new vector<wchar_t*>();
	animationSequences->reserve(10);
}

/*
	~AnimationSpriteType - Destructor, it cleans up our pointers.
*/
AnimatedSpriteType::~AnimatedSpriteType() 
{
	delete textureIDs;
	delete animationSequences;
	delete animationSequencesNames;
}

/*
	addAnimationState - This is a placeholder method, it has to be called before
	setting an animation sequence using setAnimationSequence such that the vector
	has room for it.
*/
void AnimatedSpriteType::addAnimationState(wchar_t *stateName)
{
	animationSequences->push_back(new vector<int>);
	animationSequencesNames->push_back(stateName);

}

int AnimatedSpriteType::getAnimationFrameID(int state, int index)
{
	return animationSequences->at(state)->at(index);
}
