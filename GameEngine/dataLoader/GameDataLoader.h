/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameDataLoader.h

	This class should read data from CSV (Comma Separated Values)
	files and uses them to load game data for a game
	application. Game data for rendering the game GUI is
	loaded when the game starts, data for displaying each
	game level would be loaded as needed. Note that we are not
	using this in the first benchmark.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Item.h"
#include <vector>
#include <list>

// FORWARD DECLARATIONS OF CLASSES USED BY GAME
class Game;

class GameDataLoader
{
public:
	// SIMPLE INLINED CONSTRUCTOR/DESTRUCTOR

	GameDataLoader()			{} 
	~GameDataLoader()			{}
	string gameMenuFile;
	string spritesFile;
	string worldSpecsFile;
	string actsFile;
	string tileFile;
	string itemFile;
	string introFile;
	Item *selectedItem;
	Item *dummyItem;

	void setIntroFile(string file)		{ introFile = file;}
	string getIntroFile()				{return introFile;}
	void setSelectedItem(Item *item)	{selectedItem = item;}
	Item* getSelectedItem()				{return selectedItem;}
	void setTileFile(string file)		{tileFile = file;}
	string getTileFile()				{return tileFile;}
	void setItemFile(string file)		{itemFile = file;}
	string getItemFile()				{return itemFile;}
	int tileSize;
	void setTileSize(int i)				{tileSize = i;}
	int getTileSize()					{return tileSize;}
	void setActsFile(string file)		{actsFile = file;}
	string getActsFile()				{return actsFile;}
	void setWorldSpecsFile(string file)	{worldSpecsFile = file;}
	string getWorldSpecsFile()			{return worldSpecsFile;}
	void setGameMenuFile(string file)	{gameMenuFile = file;}
	string	getGameMenuFile()			{return gameMenuFile;}
	void setSpritesFile(string file)	{spritesFile = file;}
	string getSpritesFile()				{ return spritesFile;}
	int viewportXdifferenceLeft;
	int viewportXdifferenceRight;
	int viewportYdifferenceTop;
	int viewportYdifferenceBottom;
	void setViewportXdifferenceLeft(int i)	{viewportXdifferenceLeft = i;}
	void setViewportXdifferenceRight(int i)	{viewportXdifferenceRight = i;}
	void setViewportYdifferenceTop(int i)	{viewportYdifferenceTop = i;}
	void setViewportYdifferenceBottom(int i)	{viewportYdifferenceBottom = i;}
	int getViewportXdifferenceLeft()		{return viewportXdifferenceLeft;}
	int getViewportXdifferenceRight()		{return viewportXdifferenceRight;}
	int getViewportYdifferenceTop()		{return viewportYdifferenceTop;}
	int getViewportYdifferenceBottom()		{return viewportYdifferenceBottom;}


	// METHODS TO BE DEFINED INSIDE GameDataLoader.cpp
	void loadGUI(Game *game);
	void loadWorld(Game *game);
	wchar_t* stringToLCPWSTR(string s);
	Item* getItemType(string s);
	void loadIntro(Game *game);
	void clearInventory(Game *game);
	void clearSelectedItem();
	void loadASprite(Game *game, int spriteIndex);
	void loadBoss(Game *game);

};