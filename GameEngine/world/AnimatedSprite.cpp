/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.cpp

	See AnimatedSprite.h for a class description.
*/

#include "stdafx.h"
#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "PhysicalProperties.h"

/*
	AnimatedSprite - Default constructor, just sets everything to 0.
*/
AnimatedSprite::AnimatedSprite()  
{
	spriteType = 0;
	currentState = 0;
	currentFrame = 0;
	frameIndex = 0;
	animationCounter = 0;
	offCooldown=true;
	cooldown=0;
}

/*
	Destructor - Nothing to clean up. We don't want to clean up the
	sprite type because it is a shared variable.
*/
AnimatedSprite::~AnimatedSprite() 
{

}

void AnimatedSprite::updateCooldown()
{
	cooldown++;
	if (cooldown > 60)
	{
		cooldown=0;
		offCooldown=true;
	}
}

/*
	changeFrame - This method allows for the changing of an image in an 
	animation sequence for a given animation state.
*/
void AnimatedSprite::changeFrame()
{
	if (currentState == RUNNING_RIGHT)
	{
		if (frameIndex == 1)
		{
		int blah = 5;
		int blahblah = blah + 5;
		}
	}

	currentFrame = spriteType->getAnimationFrameID(currentState, frameIndex);
	frameIndex++;
	frameIndex = frameIndex % spriteType->getSequenceSize(currentState);
}

void AnimatedSprite::setCurrentState(int newState) 
{
	currentState = newState;
	frameIndex = 0;
}

/*
	updateSprite - To be called every frame of animation. This
    method advances the animation counter appropriately per
	the animation speed. It also updates the sprite location
	per the current velocity.
*/
void AnimatedSprite::updateSprite()
{
	animationCounter++;
	animationCounter = animationCounter%spriteType->getAnimationSpeed();

	// WE ONLY CHANGE THE FRAME OF ANIMATION ONCE EVERY animationSpeed FRAMES
	if (animationCounter == 0)
	{
		changeFrame();
	}

	// UPDATE THE SPRITE LOCATION
	pp->incX(pp->getVelocityX());
	pp->incY(pp->getVelocityY());
}
