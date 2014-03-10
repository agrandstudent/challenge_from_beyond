/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSpriteType.h

	This class stores data to be shared between similar types
	of sprites. Individual sprites will be in different locations
	and have different state variables, but they may share things
	like animation sequences.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Viewport.h"
#include <vector>


class AnimatedSpriteType
{
private:
	int spriteTypeID;
	int movementSpeed;
	int HP;

	// animationSequences WILL BE A vector OF JAGGED vectors.
	// THE INDEX OF THIS ARRAY IS THE ANIMATION STATE, AND EACH
	// VECTOR STORES THE IMAGE ID TO BE USED IN A GIVEN
	// ANIMATION SEQUENCE
	vector<vector<int>*> *animationSequences;

	// THE NAMES OF THE ANIMATION SEQUENCES (i.e. RUNNING)
	vector<wchar_t*> *animationSequencesNames;

	// RATE AT WHICH FRAME CHANGES ARE MADE (MORE MEANS SLOWER)
	int animationSpeed;

	// ANIMATION IMAGES, THESE ARE THE INDICES AS LOADED INTO
	// THE LEVEL'S TEXTURE MANAGER
	vector<int> *textureIDs;

	// WIDTH & HEIGHT OF TEXTURES
	int textureHeight, textureWidth;

public:
	// INLINED ACCESSOR METHODS
	int	getAnimationSpeed()			{ return animationSpeed;						}
	int getSequenceSize(int state)	{ return animationSequences->at(state)->size();	}
	int	getSpriteTypeID()			{ return spriteTypeID;							}
	int	getTextureHeight()			{ return textureHeight;							}
	int	getTextureWidth()			{ return textureWidth;							}
	int getMovementSpeed()			{ return movementSpeed;}
	int getHP()						{ return HP;}

	// INLINED MUTATOR METHODS
	void setAnimationSpeed(int updatedSpeed)
	{	animationSpeed = updatedSpeed;								}
	void addAnimationFrame(int sequenceNum, int initFrame)
	{	animationSequences->at(sequenceNum)->push_back(initFrame);	}
	void addAnimationSequence(vector<int> *initSequence)
	{	animationSequences->push_back(initSequence);				}
	void addImageID(int initImageID)
	{	textureIDs->push_back(initImageID);							}
	void setSpriteTypeID(int initSpriteTypeID)
	{	spriteTypeID = initSpriteTypeID;							}
	void setTextureSize(int initTextureWidth,	int initTextureHeight)
	{	textureWidth = initTextureWidth;
		textureHeight = initTextureHeight;							}
	  void setMovementSpeed(int i)	{ movementSpeed = i;}
	void incMovementSpeed()			{movementSpeed++;}
	void decMovementSpeed()			{movementSpeed--;}
	void setHP(int initHP)			{HP = initHP;	}
	void decreaseHP()				{	HP--;		}
	// METHODS DEFINED IN AnimatedSpriteType.cpp
	AnimatedSpriteType();
	~AnimatedSpriteType();
	void addAnimationState(wchar_t *stateName);
	int getAnimationFrameID(int state, int index);
};