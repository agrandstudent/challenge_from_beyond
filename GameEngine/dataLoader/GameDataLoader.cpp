#pragma once
#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "BoundingVolume.h"
#include "CollidableObject.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameWorld.h"
#include "PhysicalProperties.h"
#include "TiledLayer.h"
#include "SparseLayer.h"
#include "SpriteManager.h"
#include "Viewport.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "Bot.h"
#include "GameAI.h"
#include "Player.h"
#include "XactSound.h"
#include "Item.h"


SpriteManager *spriteManager;
AnimatedSpriteType *ast;
int fromArea;

string spritesFile;
int currentLevel;
string fileToRead;
string lineRead;
TextureManager *worldTextureManager;
vector<Tile*> *tileVector;
vector<Item*> *itemList;
vector<OverlayImage*> *overlayImageVector;
Tile *tileToAdd;
string areaFile;
bool playerCreated=false;
bool isCollidable;
bool hurts;
int portal;
bool loaded=false;
int currentArea;
Item *selectedItem;
bool dummyLoaded;

Player *player;


/*
	loadLevelExample - This method loads the current
	level with data. It illustrates all the data
	we would need to load ourselves. You should
	load your data by reading it from a file.
*/
void GameDataLoader::clearSelectedItem()
{
	if (!dummyLoaded)
	{
		dummyItem = new Item();
		dummyItem->name="empty";
	}
	selectedItem = dummyItem;
}

void GameDataLoader::clearInventory(Game *game)
{
	wchar_t* fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR("textures/gui/overlays/itemButton.png"));
	int inventoryButtonID = game->getGraphics()->getGUITextureManager()->loadTexture(fileName);
	//initialize inventory empty images
	player = game->getPlayerObject();
	for (int i=0; i<15; i++)
	{
		player->getInventoryImages()->at(i)->imageID=inventoryButtonID;
	}
	player->resetInventoryIndex();
	player->updateInventoryBar(player->getInventoryImages());
}

Item* GameDataLoader::getItemType(string itemName)
{
	for (int i = 0; i<itemList->size();i++)
	{
		if (itemList->at(i)->name.compare(itemName)==0)
		{
			return itemList->at(i);
		}
	}
}



void loadTileTypes(Game *game, string file)
{
	using namespace std;
	GameGraphics *graphics = game->getGraphics();
	worldTextureManager = graphics->getWorldTextureManager();
	tileVector = new vector<Tile*>();
	ifstream inputFile;
	string lineRead;
	inputFile.open(file);
	wchar_t *fileName;
	if (inputFile)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(inputFile,lineRead))
		{
			ss<<lineRead;
			while (getline(ss, lineRead,','))
			{
				getline(ss,lineRead,',');
				fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
				
				tileToAdd = new Tile();
				getline(ss,lineRead,',');
				isCollidable=atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				hurts = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				portal = atoi(lineRead.c_str());
				tileToAdd->textureID=worldTextureManager->loadTexture(fileName);
				tileToAdd->collidable = isCollidable;
				tileToAdd->hurts=hurts;
				tileToAdd->portal=portal;
				tileVector->push_back(tileToAdd);
				//make tile vector of tiles with attributes imbued;
			}
			ss.clear();
		}
	}
}

AnimatedSpriteType* createSprite(Game *game, string file)
{
	using namespace std;
	GameGraphics *graphics = game->getGraphics();
	worldTextureManager = graphics->getWorldTextureManager();
	ast = new AnimatedSpriteType();
	wchar_t *animStateName;
	int spriteImageID1;
	int spriteImageID2;
	int spriteImageID3;
	int spriteImageID4;
	int spriteImageID5;
	int spriteImageID6;
	int spriteImageID7;
	int height;
	int width;
	int movementSpeed;
	int HP;
	ifstream createSpriteFile;
	string lineRead;
	createSpriteFile.open(file);
	wchar_t *fileName;
	if (createSpriteFile)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(createSpriteFile,lineRead))
		{
			ss<<lineRead;
			while (getline(ss, lineRead,','))
			{
				if (lineRead.compare("spriteID")==0)
				{
					getline(ss,lineRead,',');
					ast->setSpriteTypeID(atoi(lineRead.c_str()));
				}
				if (lineRead.compare("animationSpeed")==0)
				{
					getline(ss,lineRead,',');
					ast->setAnimationSpeed(atoi(lineRead.c_str()));
				}
				if (lineRead.compare("textureSize")==0)
				{
					getline(ss,lineRead,',');
					height = atoi(lineRead.c_str());
					getline(ss,lineRead,',');
					width = atoi(lineRead.c_str());
					ast->setTextureSize(width,height);
				}
				if (lineRead.compare("movementSpeed")==0)
				{
					getline(ss,lineRead,',');
					movementSpeed = atoi(lineRead.c_str());
					ast->setMovementSpeed(movementSpeed);
				}
				if (lineRead.compare("HP")==0)
				{
					getline(ss,lineRead,',');
					HP = atoi(lineRead.c_str());
					ast->setHP(HP);
				}
				if (lineRead.compare("down")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"Down");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(0, spriteImageID2);
					ast->addAnimationFrame(0, spriteImageID2);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID3);
					ast->addAnimationFrame(0, spriteImageID3);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID1);
					animStateName = constructEmptyWCHAR_TArray(L"StandingDown");
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(1, spriteImageID1);
					ast->addAnimationFrame(1, spriteImageID1);
				}
				if (lineRead.compare("left")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"Left");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(2, spriteImageID2);
					ast->addAnimationFrame(2, spriteImageID2);
					ast->addAnimationFrame(2, spriteImageID1);
					ast->addAnimationFrame(2, spriteImageID1);
					ast->addAnimationFrame(2, spriteImageID3);
					ast->addAnimationFrame(2, spriteImageID3);
					ast->addAnimationFrame(2, spriteImageID1);
					ast->addAnimationFrame(2, spriteImageID1);
					animStateName = constructEmptyWCHAR_TArray(L"StandingLeft");
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(3, spriteImageID1);
					ast->addAnimationFrame(3, spriteImageID1);
				}
				if (lineRead.compare("up")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"Up");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(4, spriteImageID2);
					ast->addAnimationFrame(4, spriteImageID2);
					ast->addAnimationFrame(4, spriteImageID1);
					ast->addAnimationFrame(4, spriteImageID1);
					ast->addAnimationFrame(4, spriteImageID3);
					ast->addAnimationFrame(4, spriteImageID3);
					ast->addAnimationFrame(4, spriteImageID1);
					ast->addAnimationFrame(4, spriteImageID1);
					animStateName = constructEmptyWCHAR_TArray(L"StandingUp");
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(5, spriteImageID1);
					ast->addAnimationFrame(5, spriteImageID1);
				}
				if (lineRead.compare("right")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"Right");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(6, spriteImageID2);
					ast->addAnimationFrame(6, spriteImageID2);
					ast->addAnimationFrame(6, spriteImageID1);
					ast->addAnimationFrame(6, spriteImageID1);
					ast->addAnimationFrame(6, spriteImageID3);
					ast->addAnimationFrame(6, spriteImageID3);
					ast->addAnimationFrame(6, spriteImageID1);
					ast->addAnimationFrame(6, spriteImageID1);
					animStateName = constructEmptyWCHAR_TArray(L"StandingRight");
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(7, spriteImageID1);
					ast->addAnimationFrame(7, spriteImageID1);
				}
				if (lineRead.compare("attackDown")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"attackDown");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					

					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(8, spriteImageID1);
					ast->addAnimationFrame(8, spriteImageID2);
					ast->addAnimationFrame(8, spriteImageID3);
					ast->addAnimationFrame(8, spriteImageID3);
				}
				if (lineRead.compare("attackLeft")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"attackLeft");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID6 = worldTextureManager->loadTexture(fileName);

					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(9, spriteImageID1);
					ast->addAnimationFrame(9, spriteImageID2);
					ast->addAnimationFrame(9, spriteImageID3);
					ast->addAnimationFrame(9, spriteImageID4);
					ast->addAnimationFrame(9, spriteImageID5);
					ast->addAnimationFrame(9, spriteImageID5);
					//ast->addAnimationFrame(9, spriteImageID6);
				}
				if (lineRead.compare("attackRight")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"attackRight");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);

					

					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(10, spriteImageID1);
					ast->addAnimationFrame(10, spriteImageID2);
					ast->addAnimationFrame(10, spriteImageID3);
					ast->addAnimationFrame(10, spriteImageID4);
					ast->addAnimationFrame(10, spriteImageID5);
					ast->addAnimationFrame(10, spriteImageID5);
				}
				if (lineRead.compare("attackUp")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"attackUp");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
		

					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(11, spriteImageID1);
					ast->addAnimationFrame(11, spriteImageID2);
					ast->addAnimationFrame(11, spriteImageID3);
					ast->addAnimationFrame(11, spriteImageID3);
				}


				if (lineRead.compare("death")==0)
                {
                                       animStateName = constructEmptyWCHAR_TArray(L"death");
                                       getline(ss,lineRead,',');
                                       fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
                                       spriteImageID1 = worldTextureManager->loadTexture(fileName);
                                       getline(ss,lineRead,',');
                                       fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
                                       spriteImageID2 = worldTextureManager->loadTexture(fileName);
                                       getline(ss,lineRead,',');
                                       fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
                                       spriteImageID3 = worldTextureManager->loadTexture(fileName);
                                       getline(ss,lineRead,',');
                                       fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
                                       spriteImageID4 = worldTextureManager->loadTexture(fileName);
                                       getline(ss,lineRead,',');
                                       fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
                                       spriteImageID5 = worldTextureManager->loadTexture(fileName);
                                       getline(ss,lineRead,',');
                                       fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
                                       
                                       
                                       ast->addAnimationState(animStateName);
                                       ast->addAnimationFrame(12, spriteImageID1);
                                       ast->addAnimationFrame(12, spriteImageID2);
                                       ast->addAnimationFrame(12, spriteImageID3);
                                       ast->addAnimationFrame(12, spriteImageID4);
                                       ast->addAnimationFrame(12, spriteImageID5);
                                       ast->addAnimationFrame(12, spriteImageID5);

                                       animStateName = constructEmptyWCHAR_TArray(L"death");
                                       ast->addAnimationState(animStateName);
                                       ast->addAnimationFrame(13, spriteImageID5);
                                       ast->addAnimationFrame(13, spriteImageID5);

                 }

				if (lineRead.compare("deathLeft")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"deathLeft");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID6 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID7 = worldTextureManager->loadTexture(fileName);
					
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(8, spriteImageID1);
					ast->addAnimationFrame(8, spriteImageID2);
					ast->addAnimationFrame(8, spriteImageID3);
					ast->addAnimationFrame(8, spriteImageID4);
					ast->addAnimationFrame(8, spriteImageID5);
					ast->addAnimationFrame(8, spriteImageID6);
					ast->addAnimationFrame(8, spriteImageID7);
					ast->addAnimationFrame(8, spriteImageID7);

				}
				if (lineRead.compare("deathRight")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"deathRight");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID6 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID7 = worldTextureManager->loadTexture(fileName);
					
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(8, spriteImageID1);
					ast->addAnimationFrame(8, spriteImageID2);
					ast->addAnimationFrame(8, spriteImageID3);
					ast->addAnimationFrame(8, spriteImageID4);
					ast->addAnimationFrame(8, spriteImageID5);
					ast->addAnimationFrame(8, spriteImageID6);
					ast->addAnimationFrame(8, spriteImageID7);
					ast->addAnimationFrame(8, spriteImageID7);
				}

				if (lineRead.compare("swordLeft")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"swordLeft");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					
					
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID2);
					ast->addAnimationFrame(0, spriteImageID3);
					ast->addAnimationFrame(0, spriteImageID4);
					ast->addAnimationFrame(0, spriteImageID5);
					ast->addAnimationFrame(0, spriteImageID5);
				}

				if (lineRead.compare("swordUp")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"swordUp");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					
					
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID2);
					ast->addAnimationFrame(0, spriteImageID3);
					ast->addAnimationFrame(0, spriteImageID3);
				}

				if (lineRead.compare("vdown")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"VDown");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(0, spriteImageID2);
					ast->addAnimationFrame(0, spriteImageID2);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID3);
					ast->addAnimationFrame(0, spriteImageID3);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID1);
					
				}
				if (lineRead.compare("vleft")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"VLeft");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(1, spriteImageID2);
					ast->addAnimationFrame(1, spriteImageID2);
					ast->addAnimationFrame(1, spriteImageID1);
					ast->addAnimationFrame(1, spriteImageID1);
					ast->addAnimationFrame(1, spriteImageID3);
					ast->addAnimationFrame(1, spriteImageID3);
					ast->addAnimationFrame(1, spriteImageID1);
					ast->addAnimationFrame(1, spriteImageID1);
					
					
				}
				if (lineRead.compare("vup")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"VUp");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(2, spriteImageID2);
					ast->addAnimationFrame(2, spriteImageID2);
					ast->addAnimationFrame(2, spriteImageID1);
					ast->addAnimationFrame(2, spriteImageID1);
					ast->addAnimationFrame(2, spriteImageID3);
					ast->addAnimationFrame(2, spriteImageID3);
					ast->addAnimationFrame(2, spriteImageID1);
					ast->addAnimationFrame(2, spriteImageID1);
					
				}
				if (lineRead.compare("vright")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"VRight");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);

					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(3, spriteImageID2);
					ast->addAnimationFrame(3, spriteImageID2);
					ast->addAnimationFrame(3, spriteImageID1);
					ast->addAnimationFrame(3, spriteImageID1);
					ast->addAnimationFrame(3, spriteImageID3);
					ast->addAnimationFrame(3, spriteImageID3);
					ast->addAnimationFrame(3, spriteImageID1);
					ast->addAnimationFrame(3, spriteImageID1);
					
				}
				
				if (lineRead.compare("vcloud")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"transform");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					
					
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(4, spriteImageID1);
					ast->addAnimationFrame(4, spriteImageID2);
					ast->addAnimationFrame(4,spriteImageID3);
					ast->addAnimationFrame(4,spriteImageID4);
					ast->addAnimationFrame(4,spriteImageID5);
				}
				
				if (lineRead.compare("RATK")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"lATK");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID6 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(5, spriteImageID1);
					ast->addAnimationFrame(5, spriteImageID2);
					ast->addAnimationFrame(5, spriteImageID3);
					ast->addAnimationFrame(5, spriteImageID4);
					ast->addAnimationFrame(5, spriteImageID5);
					ast->addAnimationFrame(5, spriteImageID6);
				}
				if (lineRead.compare("LATK")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"lATK");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID6 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(6, spriteImageID1);
					ast->addAnimationFrame(6, spriteImageID2);
					ast->addAnimationFrame(6, spriteImageID3);
					ast->addAnimationFrame(6, spriteImageID4);
					ast->addAnimationFrame(6, spriteImageID5);
					ast->addAnimationFrame(6, spriteImageID6);
				}
				if (lineRead.compare("UATK")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"uATK");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID6 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(7, spriteImageID1);
					ast->addAnimationFrame(7, spriteImageID2);
					ast->addAnimationFrame(7, spriteImageID3);
					ast->addAnimationFrame(7, spriteImageID4);
					ast->addAnimationFrame(7, spriteImageID5);
					ast->addAnimationFrame(7, spriteImageID6);
				}
				if (lineRead.compare("DATK")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"dATK");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID6 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(8, spriteImageID1);
					ast->addAnimationFrame(8, spriteImageID2);
					ast->addAnimationFrame(8, spriteImageID3);
					ast->addAnimationFrame(8, spriteImageID4);
					ast->addAnimationFrame(8, spriteImageID5);
					ast->addAnimationFrame(8, spriteImageID6);
				}
				if (lineRead.compare("vdeath")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"Death");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(9, spriteImageID1);
					ast->addAnimationFrame(9, spriteImageID2);
					ast->addAnimationFrame(9, spriteImageID3);
					ast->addAnimationFrame(9, spriteImageID4);
					ast->addAnimationFrame(9, spriteImageID4);
				
				}
				
				/*if (lineRead.compare("deathUp")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"deathUp");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID2 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID3 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID4 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID5 = worldTextureManager->loadTexture(fileName);
					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(10, spriteImageID1);
					ast->addAnimationFrame(10, spriteImageID2);
					ast->addAnimationFrame(10, spriteImageID3);
					ast->addAnimationFrame(10, spriteImageID4);
					ast->addAnimationFrame(10, spriteImageID5);
				}*/
			}
			ss.clear();
		}
	}

	return ast;
}


void loadItemTypes(Game *game, string file)
{
	GameGraphics *graphics = game->getGraphics();
	worldTextureManager = graphics->getWorldTextureManager();
	ifstream itemFile;
	string lineRead;
	itemFile.open(file);
	itemList = new vector<Item*>();
	if (itemFile)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(itemFile,lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				Item *itemToAdd = new Item();
				itemToAdd->name=lineRead;
				getline(ss,lineRead,',');
				wchar_t *fileName;
				fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
				itemToAdd->imageID=game->getGraphics()->getGUITextureManager()->loadTexture(fileName);
				getline(ss,lineRead,',');
				itemToAdd->damage=atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				itemToAdd->description1 = lineRead;
				getline(ss,lineRead,',');
				itemToAdd->description2 = lineRead;
				getline(ss,lineRead,',');
				itemToAdd->description3 = lineRead;
				getline(ss,lineRead,',');
				itemToAdd->description4 = lineRead;
				itemList->push_back(itemToAdd);
			}
			ss.clear();
		}
	}
}

void GameDataLoader::loadIntro(Game *game)
{
	string file = this->getIntroFile();
	AnimatedSprite *spriteToAdd;
	ifstream intro;
	intro.open(file);
	GameGraphics *graphics = game->getGraphics();
	worldTextureManager = graphics->getWorldTextureManager();
	int index;
	int initX;
	int initY;
	int accelerationX;
	int accelerationY;
	int velocityX;
	int velocityY;
	int spriteType;
	int alpha;
	int currentState;
	int height;
	int width;
	ast = new AnimatedSpriteType();
	wchar_t *animStateName;
	int spriteImageID0;
	int spriteImageID1;
	int spriteImageID2;
	int spriteImageID3;
	int spriteImageID4;
	wchar_t *fileName;

	if (intro)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(intro,lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				if (lineRead.compare("animationSpeed")==0)
				{
					getline(ss,lineRead,',');
					ast->setAnimationSpeed(atoi(lineRead.c_str()));
				}
				if (lineRead.compare("textureSize")==0)
				{
					getline(ss,lineRead,',');
					height = atoi(lineRead.c_str());
					getline(ss,lineRead,',');
					width = atoi(lineRead.c_str());
					ast->setTextureSize(width,height);
				}
				if (lineRead.compare("scenes")==0)
				{
					animStateName = constructEmptyWCHAR_TArray(L"scenes");
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID0 = worldTextureManager->loadTexture(fileName);
					getline(ss,lineRead,',');
					fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					spriteImageID1 = worldTextureManager->loadTexture(fileName);

					ast->addAnimationState(animStateName);
					ast->addAnimationFrame(0, spriteImageID1);
					ast->addAnimationFrame(0, spriteImageID1);
				}
				
				
			}
			ss.clear();
		}
		PhysicalProperties *spriteProps = new PhysicalProperties();
				spriteToAdd = new AnimatedSprite();
				spriteToAdd->setSpriteType(ast);
				accelerationX = 0;
				accelerationY = 0;
				velocityX = 0;
				velocityY = 0;
				spriteType = 10;
				alpha = 255;
				currentState = 0;
				
				spriteProps->setX(0);
				spriteProps->setY(0);
				spriteProps->setAccelerationX(accelerationX);
				spriteProps->setAccelerationY(accelerationY);
				spriteProps->setVelocityX(velocityX);
				spriteProps->setInAir(false);
				spriteProps->setVelocityY(-1);
				spriteToAdd->setPhysicalProperties(spriteProps);
				BoundingVolume *bv = new BoundingVolume();
				bv->setX(0);
				bv->setY(0);
				bv->setHeight(ast->getTextureHeight());
				bv->setWidth(ast->getTextureWidth());
				bv->setType(spriteType);
				spriteToAdd->setBoundingVolume(bv);
				spriteToAdd->setAlpha(alpha);
				spriteToAdd->setCurrentState(currentState);
				spriteToAdd->setCurrentlyCollidable(true);
				game->getWorld()->getSpriteManager()->addSprite(spriteToAdd);
	}
	game->getWorld()->getViewport()->setViewportWidth(9000);
	game->getWorld()->getViewport()->setViewportHeight(9000);
	game->getWorld()->getViewport()->setViewportOffsetX(0);
	game->getWorld()->getViewport()->setViewportOffsetY(0);

}

void loadSpriteTypes(Game *game, string file)
{
	using namespace std;
	GameGraphics *graphics = game->getGraphics();
	worldTextureManager = graphics->getWorldTextureManager();
	ifstream spriteFile;
	string lineRead;
	spriteFile.open(file);
	if (spriteFile)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(spriteFile,lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				getline(ss,lineRead,',');
				spriteManager->addSpriteType(createSprite(game, lineRead));
			}
			ss.clear();
		}
	}
}

wchar_t* GameDataLoader::stringToLCPWSTR(string s)
	{
		wchar_t *fileToLoad;
		int divLocation = s.find_first_of("-");
		string location = (s.substr(divLocation+1));
		int index = atoi((s.substr(0, divLocation).c_str()));
		int size = MultiByteToWideChar(CP_ACP, 0, location.c_str(), -1, 0, 0);
		wchar_t* buf = (wchar_t*)_malloca(size * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, location.c_str(), -1, buf, size);
		fileToLoad = constructEmptyWCHAR_TArray(buf);
		return fileToLoad;
	}

void setWorldAttributes(Game *game, string file)
{
	GameWorld *world = game->getWorld();
	Viewport *viewport = world->getViewport();
	GameDataLoader *dataloader = game->getDataLoader();	
	ifstream worldSpecsFile;
	worldSpecsFile.open(file);
	//set world attributes
	if (worldSpecsFile)
	{
		char inputLine[255];
		stringstream ss;

		while (getline(worldSpecsFile,lineRead))
		{
			ss<<lineRead;
			while(getline(ss, lineRead, ','))
			{
				if (lineRead.compare("worldHeight")==0)
				{
					getline(ss, lineRead, ',');
					world->setWorldHeight(atoi(lineRead.c_str()));
				}
				else if (lineRead.compare("worldWidth")==0)
				{
					getline(ss, lineRead, ',');
					world->setWorldWidth(atoi(lineRead.c_str()));
				}
				else if (lineRead.compare("viewportWidth")==0)
				{
					getline(ss, lineRead, ',');
					viewport->setViewportWidth(atoi(lineRead.c_str()));
				}
				else if (lineRead.compare("viewportHeight")==0)
				{
					getline(ss, lineRead, ',');
					viewport->setViewportHeight(atoi(lineRead.c_str()));
				}
				else if (lineRead.compare("viewportOffsetX")==0)
				{
					getline(ss, lineRead, ',');
					viewport->setViewportOffsetX(atoi(lineRead.c_str()));
				}
				else if (lineRead.compare("viewportOffsetY")==0)
				{
					getline(ss, lineRead, ',');
					viewport->setViewportOffsetY(atoi(lineRead.c_str()));
				}
				else if (lineRead.compare("tileSize")==0)
				{
					getline(ss, lineRead, ',');
					dataloader->setTileSize(atoi(lineRead.c_str()));
				}
				else if (lineRead.compare("viewportX")==0)
				{
					if (fromArea == 3)
					{
						getline(ss, lineRead, ',');
						getline(ss, lineRead, ',');
						viewport->setViewportX(atoi(lineRead.c_str()));
					}
					else
					{
						getline(ss, lineRead, ',');
						viewport->setViewportX(atoi(lineRead.c_str()));
					}
					
				}
				else if (lineRead.compare("viewportY")==0)
				{
					if (fromArea == 3)
					{
						getline(ss, lineRead, ',');
						getline(ss, lineRead, ',');
						viewport->setViewportY(atoi(lineRead.c_str()));
					}
					else
					{
						getline(ss, lineRead, ',');
						viewport->setViewportY(atoi(lineRead.c_str()));
					}
				}
			}
			ss.clear();
		}
	}
}

void loadLevelTiles(Game *game, string file)
{
	GameDataLoader *dataloader = game->getDataLoader();
	Viewport *viewport = game->getWorld()->getViewport();
	int tilesize = dataloader->getTileSize();
	int worldHeight = game->getWorld()->getWorldHeight();
	int worldWidth = game->getWorld()->getWorldWidth();
	TiledLayer *tiledLayer = new TiledLayer(game->getWorld()->getWorldWidth()/64, game->getWorld()->getWorldHeight()/64, tilesize, tilesize, 0, true, viewport, game->getWorld()->getWorldWidth(), game->getWorld()->getWorldHeight());
	Tile *tileToAdd;
	ifstream otherstream;
	otherstream.open(file);
	if (otherstream)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(otherstream, lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				int index = atoi(lineRead.c_str());
				tileToAdd = tileVector->at(index);
				tiledLayer->addTile(tileToAdd);
			}
			ss.clear();
		}
	}
	otherstream.close();
	game->getWorld()->addLayer(tiledLayer);
	game->getWorld()->setTiledLayer(tiledLayer);
}

void loadLevelSparse(Game *game, string file)
{
	SparseLayer *sparseLayer = new SparseLayer();
	OverlayImage *overlayImage;
	ifstream sparseStream;
	sparseStream.open(file);
	wchar_t *imageName;
	int alpha;
	int height;
	int width;
	int x;
	int y;
	int z;
	bool collidable;
	if (sparseStream)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(sparseStream, lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				overlayImage = new OverlayImage;
				overlayImage->name = lineRead;
				getline(ss,lineRead,',');
				imageName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
				getline(ss,lineRead,',');
				alpha=atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				height = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				width = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				x = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				y = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				z = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				collidable=atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				if (lineRead!="")
				{
					overlayImage->itemContained=lineRead;
				}
				else
				{
					overlayImage->itemContained="empty";
				}
				getline(ss,lineRead,',');
				overlayImage->descriptor1=lineRead;
				getline(ss,lineRead,',');
				overlayImage->descriptor2=lineRead;
				getline(ss,lineRead,',');
				overlayImage->descriptor3=lineRead;
				getline(ss,lineRead,',');
				overlayImage->descriptor4=lineRead;

				

				overlayImage->alpha=alpha;
				overlayImage->height = height;
				overlayImage->width = width;
				overlayImage->x = x;
				overlayImage->y = y;
				overlayImage->z = z;
				overlayImage->imageID = worldTextureManager->loadTexture(imageName);
				overlayImage->isCollidable=collidable;
				sparseLayer->addTile(overlayImage);
			}
			ss.clear();
		}
	}
	sparseStream.close();
	game->getWorld()->addLayer(sparseLayer);
	game->getWorld()->setSparseLayer(sparseLayer);
}

void createBot(Game *game, string file, AnimatedSprite *sprite)
{
	Bot* bot = new Bot();
	vector<int> *instructions = new vector<int>;
	vector<int> *cycles = new vector<int>;
	ifstream spriteAI;
	spriteAI.open(file);
	if (spriteAI)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(spriteAI,lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				if (lineRead.compare("instructions")==0)
				{
					while(getline(ss,lineRead,','))
					{
						instructions->push_back(atoi(lineRead.c_str()));
					}
				}
				else if (lineRead.compare("cycles")==0)
				{
					while(getline(ss,lineRead,','))
					{
						cycles->push_back(atoi(lineRead.c_str()));
					}
				}
			}
			ss.clear();
		}
	}
	bot->setInstructions(instructions);
	bot->setCycles(cycles);
	bot->resetCount();
	bot->resetInstructionNumber();
	bot->setAnimatedSprite(sprite);
	GameAI* gameAI= game->getAI();
	gameAI->addBot(bot);
}

void loadLevelSprites(Game *game, string file)
{
	AnimatedSprite *spriteToAdd;
	ifstream levelSpritesFile;
	levelSpritesFile.open(file);
	int index;
	int initX;
	int initY;
	int accelerationX;
	int accelerationY;
	int velocityX;
	int velocityY;
	int spriteType;
	int alpha;
	int currentState;



	//create sword up
				PhysicalProperties *spriteProps = new PhysicalProperties();
				spriteToAdd = new AnimatedSprite();
				index = 6;
				ast = spriteManager->getSpriteType(index);
				spriteToAdd->setSpriteType(ast);
				accelerationX = 0;
				accelerationY = 0;
				velocityX = 0;
				velocityY = 0;
				spriteType = 6;
				alpha = 0;
				spriteProps->setX(0);
				spriteProps->setY(0);
				spriteProps->setAccelerationX(0);
				spriteProps->setAccelerationY(0);
				spriteProps->setVelocityX(0);
				spriteProps->setInAir(false);
				spriteProps->setVelocityY(0);
				spriteToAdd->setPhysicalProperties(spriteProps);
				BoundingVolume *bv = new BoundingVolume();
				bv->setX(0);
				bv->setY(0);
				bv->setHeight(ast->getTextureHeight());
				bv->setWidth(ast->getTextureWidth());
				bv->setType(spriteType);
				spriteToAdd->setBoundingVolume(bv);
				spriteToAdd->setAlpha(alpha);
				spriteToAdd->setCurrentState(0);
				spriteToAdd->setCurrentlyCollidable(false);
				spriteManager->setSwordUp(spriteToAdd);
				spriteManager->addSprite(spriteToAdd);




	if (levelSpritesFile)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(levelSpritesFile,lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				PhysicalProperties *spriteProps = new PhysicalProperties();
				spriteToAdd = new AnimatedSprite();
				getline(ss,lineRead,',');
				index = atoi(lineRead.c_str());
				ast = spriteManager->getSpriteType(index);
				spriteToAdd->setHP(ast->getHP());
				spriteToAdd->setSpriteType(ast);
				getline(ss,lineRead,',');
				accelerationX = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				accelerationY = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				velocityX = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				velocityY = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				spriteType = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				alpha = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				currentState = atoi(lineRead.c_str());
				if (index>0)
				{
					getline(ss,lineRead,',');
					createBot(game, lineRead, spriteToAdd);
				}
				else
				{
					getline(ss,lineRead,',');
				}
				if (index==0 && fromArea==2)
				{
					getline(ss,lineRead,',');
					getline(ss,lineRead,',');
					getline(ss,lineRead,',');
					initX = atoi(lineRead.c_str());
					getline(ss,lineRead,',');
					initY = atoi(lineRead.c_str());
					getline(ss,lineRead,'\n');
				}
				else if (index==0 && fromArea==3)
				{
					getline(ss,lineRead,',');
					getline(ss,lineRead,',');
					getline(ss,lineRead,',');
					getline(ss,lineRead,',');
					getline(ss,lineRead,',');
					initX = atoi(lineRead.c_str());
					getline(ss,lineRead,',');
					initY = atoi(lineRead.c_str());
				}
				else 
				{
					getline(ss,lineRead,',');
					initX = atoi(lineRead.c_str());
					getline(ss,lineRead,',');
					initY = atoi(lineRead.c_str());
					getline(ss,lineRead,'\n');
				}
				spriteProps->setX(initX);
				spriteProps->setY(initY);
				spriteProps->setAccelerationX(accelerationX);
				spriteProps->setAccelerationY(accelerationY);
				spriteProps->setVelocityX(velocityX);
				spriteProps->setInAir(false);
				spriteProps->setVelocityY(velocityY);
				spriteToAdd->setPhysicalProperties(spriteProps);
				BoundingVolume *bv = new BoundingVolume();
				bv->setX(initX);
				bv->setY(initY);
				bv->setHeight(ast->getTextureHeight());
				bv->setWidth(ast->getTextureWidth());
				bv->setType(spriteType);
				spriteToAdd->setBoundingVolume(bv);
				spriteToAdd->setAlpha(alpha);
				spriteToAdd->setCurrentState(currentState);
				spriteToAdd->setCurrentlyCollidable(true);
				spriteManager->addSprite(spriteToAdd);
				if (index==0)
				{
					spriteManager->setPlayer(spriteToAdd);
					spriteManager->getPlayer()->setPlayerObject(game->getPlayerObject());
					game->getPlayerObject()->setAtkEquipped(game->getPlayerObject()->getDummyItem());
					game->getPlayerObject()->setMscEquipped(game->getPlayerObject()->getDummyItem());
					
					/*if(game->getWorld()->getCurrentArea() == 2)
					{
						getline(ss,lineRead,',');
						initX = atoi(lineRead.c_str());
						getline(ss,lineRead,',');
						initY = atoi(lineRead.c_str());
					}
					if(game->getWorld()->getCurrentArea() == 3)
					{
						getline(ss,lineRead,',');
						getline(ss,lineRead,',');
						getline(ss,lineRead,',');
						initX = atoi(lineRead.c_str());
						getline(ss,lineRead,',');
						initY = atoi(lineRead.c_str());
					}*/
				}
				
			}
			ss.clear();
		}
	}
	
			//create sword left
				spriteProps = new PhysicalProperties();
				spriteToAdd = new AnimatedSprite();
				index = 5;
				ast = spriteManager->getSpriteType(index);
				spriteToAdd->setSpriteType(ast);
				accelerationX = 0;
				accelerationY = 0;
				velocityX = 0;
				velocityY = 0;
				spriteType = 5;
				alpha = 0;
				spriteProps->setX(game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getX() - 64);
				spriteProps->setY(game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getY());
				spriteProps->setAccelerationX(0);
				spriteProps->setAccelerationY(0);
				spriteProps->setVelocityX(0);
				spriteProps->setInAir(false);
				spriteProps->setVelocityY(0);
				spriteToAdd->setPhysicalProperties(spriteProps);
				bv = new BoundingVolume();
				bv->setX(game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getX() - 64);
				bv->setY(game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getY());
				bv->setHeight(ast->getTextureHeight());
				bv->setWidth(ast->getTextureWidth());
				bv->setType(spriteType);
				spriteToAdd->setBoundingVolume(bv);
				spriteToAdd->setAlpha(alpha);
				spriteToAdd->setCurrentState(0);
				spriteToAdd->setCurrentlyCollidable(false);
				spriteManager->setSwordLeft(spriteToAdd);
				spriteManager->addSprite(spriteToAdd);

}

void loadLevelExample(Game *game)
{
	// FIRST SETUP THE GAME WORLD DIMENSIONS
	GameWorld *world = game->getWorld();
	GameGraphics *graphics = game->getGraphics();
	GameDataLoader *dataloader = game->getDataLoader();
	spriteManager = world->getSpriteManager();
	 char *cue;
	 char *stop="";
	 XactSound *sound= game->getSound();
	 player=game->getPlayerObject();
	 Item *itemToAdd;
	
	if(!loaded)
	{
		
		loadTileTypes(game, dataloader->getTileFile());
		loadItemTypes(game, dataloader->getItemFile());

		itemToAdd = dataloader->getItemType("Teddy Bear");
		int inventoryIndex = player->getInventoryIndex();
		player->getInventoryImages()->at(inventoryIndex) = itemToAdd;
		player->increaseInventoryIndex();
		//
		player->updateInventoryBar(player->getInventoryImages());

		loaded =true;
	}
	
	loadSpriteTypes(game, dataloader->getSpritesFile());
	currentArea = game->getWorld()->getCurrentArea();
	fromArea = game->getWorld()->getFromArea();
	int textureWidth;
	int textureHeight;
	wchar_t *animStateName;
	bool area1=false;
	bool area2=false;
	bool area3=false;
	currentLevel = world->getCurrentLevel();
	string fileLocation;
	string levelnumber;
	stringstream out;
	out << currentLevel;
	levelnumber = out.str();
	ifstream getAreaStream;
	fileLocation = "Design/Act" + levelnumber + ".csv";
	getAreaStream.open(fileLocation);
	if (getAreaStream)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(getAreaStream,lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				if (lineRead.compare("1")==0 && currentArea ==1)
				{
					getline(ss,lineRead,',');
					areaFile = lineRead;
					 area1=true;
	
				}
				if (lineRead.compare("2")==0 && currentArea ==2)
				{
					getline(ss,lineRead,',');
					areaFile = lineRead;
					 area2=true;
				}
				if (lineRead.compare("3")==0 && currentArea ==3)
				{
					getline(ss,lineRead,',');
					areaFile = lineRead;
					 area3=true;
				}
			}
			ss.clear();
			
			
		}
	}

	ifstream somestream;
	somestream.open(areaFile);
	if(somestream)
	{
		char inputLine[255];
		stringstream ss;

		while (getline(somestream,lineRead))
		{
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				if (lineRead.compare("worldSpecs")==0)
				{
					getline(ss,lineRead,',');
					setWorldAttributes(game, lineRead);
				}
				if (lineRead.compare("tiles")==0)
				{
					getline(ss,lineRead,',');
					loadLevelTiles(game, lineRead);
				}
				if (lineRead.compare("sprites")==0)
				{
					getline(ss,lineRead,',');
					loadLevelSprites(game, lineRead);
				}
				if (lineRead.compare("sparse")==0)
				{
					getline(ss,lineRead,',');
					loadLevelSparse(game, lineRead);
				}
			}
			ss.clear();
		}
	}

	   string itemToSet = game->getMscEquipped();
       if(itemToSet!="empty" && itemToSet!="")
       {
               itemToAdd = dataloader->getItemType(itemToSet);
               game->getPlayerObject()->setMscEquipped(itemToAdd);
       }
       itemToSet = game->getAttackEquipped();
       if(itemToSet!="empty"  && itemToSet!="")
       {
               itemToAdd = dataloader->getItemType(itemToSet);
               game->getPlayerObject()->setAtkEquipped(itemToAdd);
       }


		sound->stopSound(stop);
		if(area1)
			{
				sound->playSound("realbgmusic");
				stop="realbgmusic";
				area2=false;
				area3=false;
			}
			else if(area2)
			{
				sound->playSound("outside");
				stop="outside";
				area1=false;
				area3=false;

			}
			else if(area3)
			{
				sound->playSound("basement");
				stop="basement";
				area2=false;
				area1=false;
			}
			game->setBGSound(stop);
			
}

/*
	loadWorld - This method should load the data
	for the GameWorld's current level. The GameWorld
	stores a vector of all level file names. Such
	a file would describe how a level would be
	constructed.
*/
void GameDataLoader::loadWorld(Game *game)	
{
	// NOTE:	I AM DEMONSTRATING HOW TO LOAD A LEVEL
	//			PROGRAMICALLY. YOU SHOULD DO THIS
	//			USING CSV FILES.
	
	loadLevelExample(game);

}

/*
	loadGUI - One could use this method to build
	the GUI based on the contents of a GUI file.
	That way we could change the GUI and add to
	it without having to rebuild the project.
*/
void GameDataLoader::loadGUI(Game *game)
{

}




void GameDataLoader:: loadASprite(Game *game, int spriteIndex)
{
	AnimatedSprite *spriteToAdd;
	GameWorld *world=game->getWorld();
	currentLevel = world->getCurrentLevel();
	string fileLocation;
	string levelnumber;
	stringstream out;
	out << currentLevel;
	levelnumber = out.str();
	
	stringstream next;
	ifstream levelSpritesFile;
	int areaNumber=world->getCurrentArea();
	
	string areaNum;
	next << areaNumber;
	areaNum = next.str();
	fileLocation="Design/Act"+levelnumber+"/Area"+areaNum+"Sprites.csv";
	levelSpritesFile.open(fileLocation);

	int index;
	int initX;
	int initY;
	int accelerationX;
	int accelerationY;
	int velocityX;
	int velocityY;
	int spriteType;
	int alpha;
	int currentState;
	if (levelSpritesFile)
	{
		char inputLine[255];
		stringstream ss;
		for (int i=0;i<spriteIndex;i++)
		{
			getline(levelSpritesFile,lineRead);
		}
		
			ss<<lineRead;
			while (getline(ss,lineRead,','))
			{
				PhysicalProperties *spriteProps = new PhysicalProperties();
				spriteToAdd = new AnimatedSprite();
				getline(ss,lineRead,',');
				index = atoi(lineRead.c_str());
				ast = spriteManager->getSpriteType(index);
				
				spriteToAdd->setSpriteType(ast);
				spriteToAdd->setHP(ast->getHP());
				getline(ss,lineRead,',');
				accelerationX = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				accelerationY = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				velocityX = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				velocityY = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				spriteType = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				alpha = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				currentState = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				createBot(game, lineRead, spriteToAdd);
				getline(ss,lineRead,',');
				initX = atoi(lineRead.c_str());
				getline(ss,lineRead,',');
				initY = atoi(lineRead.c_str());
				getline(ss,lineRead,'\n');
				spriteProps->setX(initX);
				spriteProps->setY(initY);
				spriteProps->setAccelerationX(accelerationX);
				spriteProps->setAccelerationY(accelerationY);
				spriteProps->setVelocityX(velocityX);
				spriteProps->setInAir(false);
				spriteProps->setVelocityY(velocityY);
				spriteToAdd->setPhysicalProperties(spriteProps);
				BoundingVolume *bv = new BoundingVolume();
				bv->setX(initX);
				bv->setY(initY);
				bv->setHeight(ast->getTextureHeight());
				bv->setWidth(ast->getTextureWidth());
				bv->setType(spriteType);
				spriteToAdd->setBoundingVolume(bv);
				spriteToAdd->setAlpha(alpha);
				spriteToAdd->setCurrentState(currentState);
				spriteToAdd->setCurrentlyCollidable(true);
				spriteManager->addSprite(spriteToAdd);
				
				}
			ss.clear();
		
	}
	

}
