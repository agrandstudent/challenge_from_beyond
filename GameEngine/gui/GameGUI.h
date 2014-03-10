/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGUI.h

	This class manages all data for the game gui, the cursor
	and the ScreenGUIs, which store the buttons and overlayed
	images.

	For example, the splash screen might be one screen, the main
	menu might be another, and the game as its running might be
	another.
*/
#pragma once
#include "stdafx.h"
#include <vector>

class ButtonEventHandler;
class Button;
class Cursor;
class Game;
class GameInput;
class GameGraphics;
class RenderList;
class ScreenGUI;

class GameGUI
{
private:
	// THIS VECTOR STORES ALL OF OUR SCREENS
	vector<ScreenGUI*>	*screens;

	// THIS IS OUR CURSOR, WITH WHICH WE CAN LOAD
	// WHATEVER IMAGES WE WANT
	Cursor				*customCursor;

public:
	// INLINED ACCESSOR METHODS
	Cursor*		getCursor()					{ return customCursor;		}
	ScreenGUI*	getScreen(int gameState)	{ return screens->at(gameState);	}

	// INLINED MUTATOR METHOD
	void setCursor(Cursor *initCursor)
	{
		customCursor = initCursor;
	}

	// METHODS DEFINED IN GameGUI.cpp
	GameGUI();
	~GameGUI();
	void addRenderItemsToRenderList(Game *game);
	void addScreenGUI(ScreenGUI *screenToAdd);
	bool checkCurrentScreenForAction(Game *game);
	void registerButtonEventHandler(ButtonEventHandler *eventHandler);
	void updateGUIState(long mouseX, long mouseY, int gameState);
};