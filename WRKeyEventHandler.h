/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRKeyEventHandler.h

	This is a custom key event handler. Each game can choose
	to use different keyboard keys and respond differently to when
	they are pressed. This class will handle key presses for
	this particular game following the KeyEventHandler pattern.
*/

#pragma once
#include "stdafx.h"
#include "KeyEventHandler.h"

class Game;

class WRKeyEventHandler : public KeyEventHandler
{
public:
	WRKeyEventHandler()		{}
	~WRKeyEventHandler()	{}
	void handleKeyEvents(Game *game);
};