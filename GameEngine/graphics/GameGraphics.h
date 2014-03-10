/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	This class provides a framework for technology-specific
	classes that will manage the rendering of all game textures and text.
	It provides methods for the initialization of objects necessary
	for rendering images and text.

	Rendering should be done by generating a list of RenderItem
	objects each frame and putting them into a list. These
	objects store information about the textures that have
	to be drawn to the screen.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include "GameGUI.h"
#include "GameInput.h"
#include "GameText.h"
#include "RenderList.h"
#include "StringTable.h"
#include "TextureManager.h"
#include "TextFileWriter.h"
#include "GameWorld.h"
#include <list>
#include <vector>

// OUR SCREEN SIZE PREFERENCES
const int DEFAULT_SCREEN_WIDTH = 1024;
const int DEFAULT_SCREEN_HEIGHT = 768;

class GameGraphics
{
protected:
	Game *game;

	// EACH FRAME, RENDER LISTS ARE CREATED, FILLED WITH
	// DATA, THEN USED FOR RENDERING THE FRAME, AND DESTROYED
	RenderList				*guiRenderList;
	RenderList				*worldRenderList;

	// THE TextureManagers STORE ALL THE IMAGES FOR OUR GAME
	TextureManager			*guiTextureManager;
	TextureManager			*worldTextureManager;

public:	
	// INLINED ACCESSOR METHODS
	RenderList*				getGUIRenderList()			{ return guiRenderList;			}
	TextureManager*			getGUITextureManager()		{ return guiTextureManager;		}
	RenderList*				getWorldRenderList()		{ return worldRenderList;		}
	TextureManager*			getWorldTextureManager()	{ return worldTextureManager;	}

	// VIRTUAL METHODS TO BE OVERRIDDEN BY TECHNOLOGY
	// SPECIFIC CLASSES
	virtual	TextureManager* createTextureManager()=0;
	virtual int				getScreenHeight()=0;
	virtual int				getScreenWidth()=0;
	virtual	void			initGraphics(HWND hWnd, bool isFullscreen)=0;
	virtual void			initTextFont(int fontSize)=0;
	virtual void			reloadGraphics()=0;
	virtual void			renderGame(Game *game)=0;
	virtual void			renderTextToDraw(TextToDraw *textToDraw)=0;
	virtual void			setColorKey(int r, int g, int b)=0;
	virtual void			setFontColor(int r, int g, int b)=0;
	virtual void			shutdownGraphics()=0;

	// CONCRETE METHODS DEFINED IN GameGraphics.cpp
	GameGraphics();
	~GameGraphics();
	void clearWorldTextures();
	void fillRenderLists(Game *game);
	void init();
	void renderText(GameText *text);
};