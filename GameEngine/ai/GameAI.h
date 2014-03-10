#pragma once

/*	
	Author: _______________

	GameAI.h

	This class will be used to manage all game bots, and the
	Artificial Intelligence associated with them. Each frame
	we will update the AI state, which should update all bots
	accordingly.

	Students will implement this as part of their projects.
*/

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include "Bot.h"
#include <vector>

#define GO_LEFT		            1
#define GO_Right	            2
#define STAND					3


class GameAI
{
private:
	vector<Bot*> *bots;
public:
	// WE'LL DEFINE THESE METHODS LATER
	void				addBot(Bot *bot);
	GameAI();
	~GameAI()	{}

	void				clearBots();
	void update(Game *game);
	double GameAI::guessSpriteLocation(AnimatedSprite* sprite,int numOfFramesIntoFuture,bool Xlocation);
	double GameAI::calculateProjectileXMoveSpeed(int targetX,int targetY,int shooterX, int shooterY, double projectileMoveSpeed);
};