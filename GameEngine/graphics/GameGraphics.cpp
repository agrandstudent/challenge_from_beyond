/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGraphics.cpp

	See GameGraphics.h for a class description.
*/

#include "stdafx.h"
#include "Game.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameText.h"
#include "GameOS.h"
#include "GameWorld.h"
#include "IllegalArgumentException.h"
#include "TextureManager.h"
#include "TextFileWriter.h"
#include <list>
#include <vector>

/*
	GameGraphics - Default constructor, nothing to initialize.
*/
GameGraphics::GameGraphics()
{
}

/*
	~GameGraphics - Destructor, it cleans up the render lists and texture
	managers. This should only be called when the application is closing.
*/
GameGraphics::~GameGraphics()
{
	delete guiRenderList;
	delete guiTextureManager;
	delete worldRenderList;
	delete worldTextureManager;
}

/*
	clearWorldTextures - When the game leaves a level we have to clear
	out these data structures. Calling clear on these will delete
	all the objects inside.
*/
void GameGraphics::clearWorldTextures()
{	
	// CLEAR LEVEL DATA STRUCURES
	worldTextureManager->clear();
	worldRenderList->clear();
}

/*
	fillRenderLists - This method causes the render lists to be 
	filled with the things that have to be drawn this frame.
*/
void GameGraphics::fillRenderLists(Game *game)
{
	// GENERATE RENDER LISTS FOR GAME WORLD AND GUI
	GameWorld *world = game->getWorld();
	world->addWorldRenderItemsToRenderList(game);
	GameGUI *gui = game->getGUI();
	gui->addRenderItemsToRenderList(game);
}

/*
	init - This method constructs the data structures for managing textures
	and render lists. It calls the createTextureManager, which is technology
	specific, and so is implemented only by child classes.
*/
void GameGraphics::init()
{
	// GUI TEXTURES (like buttons, cursor, etc.)
	guiRenderList = new RenderList(100);
	guiTextureManager = createTextureManager();

	// LEVEL TEXTURES (like sprites, tiles, particles, etc.)
	worldRenderList = new RenderList(1000);
	worldTextureManager = createTextureManager();
}

/*
	renderText - This method will go through the GameText argument,
	pull out each TextToDraw object, and use a technology-specific
	method in a child class, renderTextToDraw, to render each
	piece of text.
*/
void GameGraphics::renderText(GameText *text)
{
	int numTextObjects = text->getNumTextObjectsToDraw();
	for (int i = 0; i < numTextObjects; i++)
	{
		TextToDraw *textToDraw = text->getTextToDrawAtIndex(i);
		renderTextToDraw(textToDraw);
	}
}