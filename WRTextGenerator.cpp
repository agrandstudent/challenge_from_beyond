/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRTextGenerator.cpp

	See WRTextGenerator.h for a class description.
*/

#include "stdafx.h"
#include "WRTextGenerator.h"
#include "Game.h"
#include "GameGraphics.h"
#include "GameText.h"
#include "GameTimer.h"
#include "WindowsGameTimer.h"

wchar_t welcomeMessage[40];
size_t counter = 0;

void WRTextGenerator::initText(Game *game)
{
	GameText *text = game->getText();
	WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();
	welcomeMessage[0] = '\0';
	swprintf(welcomeMessage, L"TARGET FPS: %d", timer->getTargetFPS());
	text->addText(welcomeMessage, 350, 50, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
}

void WRTextGenerator::updateText(Game *game)
{
	// GET THE text OBJECT IF YOU LIKE
	GameText *text = game->getText();
	WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();

	// UPDATE AND DISPLAY ANY TEXT WE WANT, WE KNOW THAT
	// GameText IS RENDERING THIS ONE, SINCE WE GAVE IT THE
	// MEMORY ADDRESS, SO WE CAN JUST CHANGE THIS ONE DIRECTLY
	welcomeMessage[0] = '\0';
	swprintf(welcomeMessage, L"TARGET FPS: %d %d", timer->getTargetFPS(), timer->getLoopCounter());
	counter++;
}