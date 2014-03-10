/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "GameGraphics.h"
#include <vector>

class SpriteManager
{
private:
	vector<AnimatedSpriteType*> *spriteTypes;
	vector<AnimatedSprite*> *sprites;
	AnimatedSprite *player;
	AnimatedSprite *swordLeft;
	AnimatedSprite *swordUp;

public:
	// INLINED ACCESSOR METHODS
	int				getNumberOfSprites()	{ return sprites->size();	}
	AnimatedSprite* getPlayer()				{ return player;			}
	AnimatedSprite* getSwordLeft()			{ return swordLeft;			}
	AnimatedSprite* getSwordUp()			{ return swordUp;			}

	// INLINE ACCESSOR METHODS
	void setPlayer(AnimatedSprite *initPlayer)
	{	player = initPlayer;				}
	void setSprite(AnimatedSprite *initSprite, int spriteID)
	{	sprites->at(spriteID) = initSprite;	}
	void setSwordLeft(AnimatedSprite *initSwordLeft)
	{	swordLeft = initSwordLeft;		}
	void setSwordUp(AnimatedSprite *initSwordUp)
	{	swordUp = initSwordUp;		}

	// METHODS DEFINED IN SpriteManager.cpp
	SpriteManager();
	~SpriteManager();
	void				addSpriteItemsToRenderList(RenderList *renderList, Viewport *viewport);
	void				addSprite(AnimatedSprite *spriteToAdd);
	void				addSpriteType(AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	AnimatedSprite*		getSprite(int spriteID);
	AnimatedSpriteType* getSpriteType(int typeIndex);
	void				updateAllSprites();
};