/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGUI.cpp

	See GameGUI.h for a class description.
*/

#include "stdafx.h"
#include "ButtonEventHandler.h"
#include "Button.h"
#include "Cursor.h"
#include "Game.h"
#include "GameGUI.h"
#include "GameGraphics.h"
#include "GameInput.h"
#include "IllegalArgumentException.h"
#include "RenderList.h"
#include "ScreenGUI.h"
#include "TextFileReader.h"
#include "TextureManager.h"

/*
	GameGUI - Default Constructor, it sets up the data structure for storing
	all ScreenGUIs. These should subsequently be added using addScreenGUI
*/
GameGUI::GameGUI()
{
	screens = new vector<ScreenGUI*>();
	customCursor = NULL;
}

/*
	~GameGUI - Destructor, this would be called when the application is closing,
	it will clean up all memory for the GUI, including the cursor and all
	ScreenGUIs.
*/
GameGUI::~GameGUI()
{
	delete screens;
	delete customCursor;
}

/*
	addRenderItemsToRenderList - This method cascades this call to the
	current ScreenGUI. That object will add the appropriate items to
	the render list. This method should be called once per frame to make
	sure the current GUI is always rendered.

	Make sure the mode you provide matches the index of this GUI's screen.
	If an illegal mode value is provided, an exception is thrown.
*/
void GameGUI::addRenderItemsToRenderList(Game *game)
{
	int gameState = game->getGameState();
	GameGraphics *graphics = game->getGraphics();
	RenderList *guiRenderList = graphics->getGUIRenderList();

	if ((gameState >= 0) && (gameState < screens->size()))
	{
		// CASCADE THIS CALL TO THE APPROPRIATE SCREEN
		screens->at(gameState)->addRenderItemsToRenderList(guiRenderList);
	}

	// CASCADE THIS CALL TO THE CURSOR, IF THERE IS ONE
	if (customCursor != NULL)
		customCursor->addRenderItemToRenderList(guiRenderList);

}

/*
	addScreenGUI - This method adds a constructed ScreenGUI to the
	game's gui.
*/
void GameGUI::addScreenGUI(ScreenGUI *screenToAdd)
{
	screens->push_back(screenToAdd);
}

/*
	checkCurrentScreenForAction - This method should be called
	once per frame. It checks with the current screen to see if
	a button event needs to be fired, and fires it if necessary.

	If a mode is provided that does not match a ScreenGUI index,
	an exception is thrown.
*/
bool GameGUI::checkCurrentScreenForAction(Game *game)
{
	// IF THE mode DOESN'T EXIST, THROW AN EXCEPTION
	if ((game->getGameState() >= 0) && (game->getGameState() < screens->size()))
	{
		// CASCADE THE CHECK TO THE CURRENT SCREEN
		return screens->at(game->getGameState())->fireButtonCommand(game);
	}
}

/*
	registerButtonEventHandler - This method tells all of the screens in
	this gui the class that will handle button click events.
*/
void GameGUI::registerButtonEventHandler(ButtonEventHandler *eventHandler)
{
	// INFORM ALL THE SCREENS
	for (int i = 0; i < screens->size(); i++)
		screens->at(i)->registerButtonEventHandler(eventHandler);
}

/*
	updateGUIState - This method sends mousePoint, the current cursor
	location, to the current GUI screen such that all of its button
	states can be updated. This method should be called once per frame
	after mouse position has been updated but before we check for fired
	events.

	If an illegal mode is provided, meaning a screen index that does not
	exist, an exception is thrown.
*/
void GameGUI::updateGUIState(long mouseX, long mouseY, int gameState)
{
	if ((gameState >= 0) && (gameState < screens->size()))
	{
		// LET THE CURRENT SCREEN UPDATE IT'S BUTTON STATES
		screens->at(gameState)->updateAllButtons(mouseX, mouseY);
	}
}