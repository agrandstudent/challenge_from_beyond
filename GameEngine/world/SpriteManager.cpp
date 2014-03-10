/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "GameGraphics.h"
#include "PhysicalProperties.h"
#include "SpriteManager.h"
#include "GameWorld.h"
#include <vector>

/*
	SpriteManager - Default constructor, this method initializes all of the data
	structures for managing sprites.
*/
SpriteManager::SpriteManager()
{
	spriteTypes = new vector<AnimatedSpriteType*>();
	sprites = new vector<AnimatedSprite*>();
	player = NULL;
}

/*
	~SpriteManager - Destructor, it recovers all pointer-allocated memory.
*/
SpriteManager::~SpriteManager()
{
	delete spriteTypes;
	delete sprites;
	delete player;
}

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									pp->getX(),
									pp->getY(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getCurrentFrame();
		renderList->addRenderItem(	sprite->getCurrentFrame(),
									pp->getX()-viewport->getViewportX(),
									pp->getY()-viewport->getViewportY(),
									pp->getZ(),
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight());
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	RenderList *renderList,
												Viewport *viewport)
{
	// ADD THE PLAYER SPRITE
	if (player != NULL)
		addSpriteToRenderList(player, renderList, viewport);

	// NOW ADD THE REST OF THE SPRITES
	vector<AnimatedSprite*>::iterator spriteIterator;
	spriteIterator = sprites->begin();
	while (spriteIterator != sprites->end())
	{			
		AnimatedSprite *sprite = (*spriteIterator);
		addSpriteToRenderList(sprite, renderList, viewport);
		spriteIterator++;
	}
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addSprite(AnimatedSprite *spriteToAdd)
{
	sprites->push_back(spriteToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
void SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes->push_back(spriteTypeToAdd);
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes->clear();
	sprites->clear();
	if (player != NULL)
	{
		delete player;
		player = NULL;
	}
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(int typeIndex)
{
	if (typeIndex < spriteTypes->size())
		return spriteTypes->at(typeIndex);
	else
		return NULL;
}

/*
	getSprite - This method gets the sprite object that
	corresponds to the id argument.
*/
AnimatedSprite* SpriteManager::getSprite(int spriteID)
{
	if (spriteID < sprites->size())
		return sprites->at(spriteID);
	else
		return NULL;
}

/*
	updateAllSprites - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::updateAllSprites()
{
	AnimatedSpriteType *spriteType;

	// UPDATE THE PLAYER SPRITE
	if (player != NULL)
	{
		player->updateSprite();
	}

	// NOW UPDATE THE REST OF THE SPRITES
	vector<AnimatedSprite*>::iterator spriteIterator;
	spriteIterator = sprites->begin();
	while (spriteIterator != sprites->end())
	{
		AnimatedSprite *sprite = (*spriteIterator);
		sprite->updateSprite();
		spriteIterator++;
	}
} 