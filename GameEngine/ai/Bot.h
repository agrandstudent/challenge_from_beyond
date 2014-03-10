
#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include <vector>


class Bot
{
private:
	AnimatedSprite *animatedSprite;
	float density;
	int frames;
	int instructionNumber;
	vector<int> *instructions;
	vector<int> *cycles;


public:
	// INLINED METHODS
	void setAnimatedSprite(AnimatedSprite* sprite) {animatedSprite=sprite;}
	AnimatedSprite* getAnimatedSprite() { return animatedSprite; }
	float getDensity() { return density; }
	void setDensity(float initDensity) { density = initDensity; }
	void setInstructions(vector<int> *vector)	{instructions=vector;}
	void setCycles(vector<int> *vector)	{cycles=vector;}
	int	 getInstruction(int i)		{return instructions->at(i);}
	int  getCycle(int i)			{return cycles->at(i);}
	int  getCount()					{return frames;	}
	void resetCount()				{frames=0;}
	void incrementCount()			{frames++;}
	void incrementInstructionNumber()	{instructionNumber++;}
	void resetInstructionNumber()	{instructionNumber=0;}
	int getInstructionSize()		{return instructions->size();}
	int getInstructionNumber()	{return instructionNumber;}

	Bot(){};
	~Bot();
	 void update(Game *game);
};