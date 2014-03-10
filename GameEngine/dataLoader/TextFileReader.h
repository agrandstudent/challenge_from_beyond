/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileReader.h

	This is a wrapper class for reading text from a file. This
	can be useful for reading game design files for building a
	game world.
*/

#pragma once
#include "stdafx.h"
#include "GameDataLoader.h"
#include "Game.h"
#include "WorldScrollingGame.h"

class TextFileReader
{
protected:
	// FILE NAME TO READ FROM
	wchar_t *fileName;

	// STREAM TO WRITE TO
	ifstream *inFile;

	//handle to worldscrollingame
	WorldScrollingGame *worldScrollingGame;

	class GameDataLoader;

public:
	// METHODS DEFINED INSIDE TextFileReader
	TextFileReader();
	~TextFileReader();
	void closeReader();
	void initFile(wchar_t *initFileName);
	void loadSpritesAndTiles(Game *game);
	void loadDimensions(Game *game, string file);
	void loadLevel(Game *game, string file);
	void loadBackground(Game *game, string file);
	void loadForeground(Game *game, string file);
	void loadSplashScreen(Game *game);
	void loadFiles(Game *game);
	wchar_t* readLineOfText();	
}
;