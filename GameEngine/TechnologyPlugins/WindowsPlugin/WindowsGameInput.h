/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WindowsGameInput.h

	This class manages all game input. This is currently done only
	via a keyboard and mouse. Windows method are used to get input
	information each frame.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "KeyEventHandler.h"

#define NUM_MOUSE_BUTTONS 2

// FORWARD DECLARATIONS OF CLASSES USED BY WindowsGameInput.h
class Cursor;
class Game;
class GameGUI;

class WindowsGameInput : public GameInput
{
private:
	// THIS IS THE LOCATION OF THE CURSOR
	POINT *mousePoint;

public:
	// INLINED METHODS

	// GETS THE MOUSE LOCATION
	POINT*	getMousePoint()						
	{	return mousePoint;								}

	// METHODS DEFINED IN GameInput.cpp
	WindowsGameInput();
	~WindowsGameInput();
	void initInputState();
	void updateInputState();
	void updateCursorPosition(WINDOWINFO wi, Cursor *cursor);
	void processInput(Game *game);
	void respondToMouseInput(Game *game);
};