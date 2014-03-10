/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WindowsGameInput.cpp

	See WindowsGameInput.h for a class description.
*/

#include "stdafx.h"
#include "Button.h"
#include "Cursor.h"
#include "Game.h"
#include "GameGUI.h"
#include "GameInput.h"
#include "GameOS.h"
#include "KeyEventHandler.h"
#include "ScreenGUI.h"
#include "WindowsGameOS.h"
#include "WindowsGameInput.h"

/*
	WindowsGameInput - Default constructor, it will initialize the input state variables.
*/
WindowsGameInput::WindowsGameInput()
{
	mousePoint = new POINT();
	mousePoint->x = 0;
	mousePoint->y = 0;
}

/*
	~WindowsGameInput - Destructor, it will destroy the mousePoint pointer.
*/
WindowsGameInput::~WindowsGameInput()
{
	delete mousePoint;
}

/*
	processInput - This method updates first gets all input from Windows
	for the mouse and the keyboard. It then asks the event handlers
	to respond to the input.
*/
void WindowsGameInput::processInput(Game *game)
{
	WindowsGameOS *os = (WindowsGameOS*)game->getOS();
	WINDOWINFO wi = os->getWindowInfo();
	updateCursorPosition(wi, game->getGUI()->getCursor());
	updateInputState();
	respondToKeyboardInput(game);
	respondToMouseInput(game);
}

/*
	respondToMouseInput - This method sends the updated cursor position
	to the GameGUI so that it can update the Button and Cursor states.
	It then checks to see if the left mouse button is pressed, and if
	so, it asks the gui to check to see if it needs to fire an event.
	This should be called once per frame, after input is retrieved.
*/
void WindowsGameInput::respondToMouseInput(Game *game)
{
	GameGUI *gui = game->getGUI();

	gui->updateGUIState(mousePoint->x, mousePoint->y, game->getGameState());
	
	if ( (GetAsyncKeyState(VK_LBUTTON) & 0X8000)
		&& (inputState[VK_LBUTTON].isFirstPress))
	{
		gui->checkCurrentScreenForAction(game);
	}
}

/*
	updateCursorPosition - This method asks Windows for the position
	of the cursor in screen coordinates. The cursor position is fixed
	to account for windows borders. The values are recorded for use
	and the cursor is updated.
*/
void WindowsGameInput::updateCursorPosition(WINDOWINFO wi, Cursor *cursor)
{
	GetCursorPos(mousePoint);

	// Fix the mouse location
	mousePoint->x = mousePoint->x - wi.rcWindow.left - wi.rcClient.left;
	mousePoint->y = mousePoint->y - wi.rcWindow.top - wi.rcClient.top;
	if (mousePoint->x < 0)
	{
		mousePoint->x = 0;
	}
	if (mousePoint->x >= DEFAULT_SCREEN_WIDTH)
	{
		mousePoint->x = DEFAULT_SCREEN_WIDTH - 1;
	}
	if (mousePoint->y < 0)
	{
		mousePoint->y = 0;
	}
	if (mousePoint->y >= DEFAULT_SCREEN_HEIGHT)
	{
		mousePoint->y = DEFAULT_SCREEN_HEIGHT - 1;
	}

	cursor->setX(mousePoint->x);
	cursor->setY(mousePoint->y);
}

/*
	updateInputState - This method checks all keys and updates
	their states. This should be called once per frame.
*/
void WindowsGameInput::updateInputState()
{
	// RESET isPressed FOR ALL KEYS
	for (int i = 0; i < 256; i++)
		inputState[i].isPressed = false;

	// FILL IN isPressed FOR THOSE PRESSED
	for (int j = 0; j < 256; j++)
	{
		if (GetAsyncKeyState(j) & 0X8000)
			inputState[j].isPressed = true;
	}

	// UPDATE wasHeldDown & isFirstPress
	for (int k = 0; k < 256; k++) 
	{
		if (inputState[k].isPressed) 
		{
			if (inputState[k].wasHeldDown)
				inputState[k].isFirstPress = false;
			else 
			{
				inputState[k].wasHeldDown = true;
				inputState[k].isFirstPress = true;
			}
		}
		else 
		{
			inputState[k].wasHeldDown = false;
			inputState[k].isFirstPress = false;
		}
	}
}
