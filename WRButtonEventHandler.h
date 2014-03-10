/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRButtonEventHandler.h

	This is a custom button event handler. Each game can choose
	to use different buttons and respond differently to when
	they are pressed. This class will handle button presses for
	this particular game following the ButtonEventHandler pattern.
*/

#pragma once
#include "stdafx.h"
#include "ButtonEventHandler.h"

class Game;

class WRButtonEventHandler: public ButtonEventHandler
{
public:
	// METHODS DEFINED INSIDE WRButtonEventHandler.cpp
	WRButtonEventHandler()	{}
	~WRButtonEventHandler()	{}
	void handleButtonEvents(Game *game, 
							wchar_t *command);
};