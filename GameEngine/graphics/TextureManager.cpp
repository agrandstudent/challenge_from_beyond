/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextureManager.cpp

	See TextureManager.h for a class description.
*/

#include "stdafx.h"
#include "Game.h"
#include "GameGraphics.h"
#include "GameOS.h"
#include "StringTable.h"
#include "TextFileWriter.h"
#include "TextureManager.h"
#include <map>
#include <vector>

/*
	TextureManager - Default Constructor, it creates the StringTable.
*/
TextureManager::TextureManager()  
{
	stringTable = new StringTable();
}

/*
	~TextureManager - Destructor, it cleans up the StringTable pointer.
*/
TextureManager::~TextureManager() 
{
	delete stringTable;
}