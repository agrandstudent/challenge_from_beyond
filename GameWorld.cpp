/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameWorld.cpp

	See GameWorld.h for a class description.
*/

#include "stdafx.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameWorld.h"
#include "RenderList.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Viewport.h"
#include <vector>
#include "GameAI.h"

/*
	GameWorld - Default Constructor, it constructs the layers
	vector, allowing new layers to be added.
*/

int zombieSpawnTime=1;
int zombieSpawnTimer=10;
int spawnLocation=2;
int area1Zombies=0;
int area2Zombies=0;
int area3Zombies=0;
bool switchingAct=false;

GameWorld::GameWorld()	
{
	layers = new vector<WorldLayer*>();
	worldWidth = 0;
	worldHeight = 0;
	currentLevel = 0;
	currentArea = 1;
	fromArea=1;
	spriteManager = new SpriteManager();
	viewport = new Viewport();
	levelFileNames = new vector<wchar_t*>();
}

/*
	~GameWorld - This destructor will remove the memory allocated
	for the layer vector.
*/
GameWorld::~GameWorld()	
{
	delete layers;
	delete spriteManager;
	delete viewport;
	delete levelFileNames;
}

/*
	addLayer - This method is how layers are added to the World.
	These layers might be TiledLayers, SparseLayers, or 
	IsometricLayers, all of which are child classes of WorldLayer.
*/
void GameWorld::addLayer(WorldLayer *layerToAdd)
{
	layers->push_back(layerToAdd);
}

/*
	addWorldRenderItemsToRenderList - This method sends the render
	list and viewport to each of the layers such that they
	may fill it with RenderItems to draw.
*/
void GameWorld::addWorldRenderItemsToRenderList(Game *game)
{
	if (game->getGameState() == GAME_PLAYING_GAME_STATE || (game->getGameState() == CHECK_GAME_STATE) || (game->getGameState() == INTRO_GAME_STATE) || (game->getGameState() == PAUSE_GAME_STATE) || (game->getGameState() == INVENTORY_GAME_STATE))
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		for (int i = 0; i < layers->size(); i++)
		{
			layers->at(i)->addRenderItemsToRenderList(	renderList,
													viewport);
		}
		spriteManager->addSpriteItemsToRenderList(renderList, viewport);
	}
}

/*
	addLevelFileName - This method adds a level file name to the vector
	of all the level file names. Storing these file names allows us to
	easily load a desired level at any time.
*/
void GameWorld::addLevelFileName(wchar_t *levelFileName)
{
	levelFileNames->push_back(levelFileName);
}

/*
	clear - This method removes all data from the World. It should
	be called first when a level is unloaded or changed. If it
	is not called, an application runs the risk of having lots
	of extra data sitting around that may slow the progam down.
	Or, if the world thinks a level is still active, it might add
	items to the render list using image ids that have already been
	cleared from the GameGraphics' texture manager for the world.
	That would likely result in an exception.
*/
void GameWorld::unloadCurrentLevel(Game *game)
{
	spriteManager->clearSprites();
	game->getAI()->clearBots();
	layers->clear();	
	currentLevel = 0;
	worldWidth = 0;
	worldHeight = 0;
}

/*
	getLevelFileName - This method gets the name of the file used to
	load the current level. Each level has a file where the layout
	of the level and what artwork to use is specified.
*/
wchar_t* GameWorld::getLevelFileName(int levelNumber)
{
	return levelFileNames->at(levelNumber-1);
}

/*
	loadCurrentLevel - This method changes the current level. This method should
	be called before loading all the data from a level file.
*/
void GameWorld::loadCurrentLevel(Game *game, int initLevel)
{

	game->setAttackEquipped(game->getPlayerObject()->getAtkEquipped()->name);
    game->setMscEquipped(game->getPlayerObject()->getMscEquipped()->name);


	int currentAreaZombies=0;
	if ((initLevel > 0) && (initLevel <= levelFileNames->size()))
	{
		unloadCurrentLevel(game);
		currentLevel = initLevel;
		GameDataLoader *dataLoader = game->getDataLoader();
		dataLoader->loadWorld(game);
		int numOfZombies;
		if(currentArea==1)
		{
			currentAreaZombies=area1Zombies;
		}
		if(currentArea==2)
		{
			currentAreaZombies=area2Zombies;

		}
		if(currentArea==3)
		{
			currentAreaZombies=area3Zombies;
		}
		if(currentAreaZombies>20)
		{
			currentAreaZombies=21;
		}
		
		for (int i=0; i<currentAreaZombies;i++)
		{
			int tracker=i;
			if(i>=9)
				i=i-9;
			dataLoader->loadASprite(game,i+2);
			i=tracker;
		}

	}
}

/*
	update - This method should be called once per frame. It updates
	all of the dynamic level data, like sprite animation states and
	particle locations.
*/
void GameWorld::update(Game *game)
{
	int time = game->getPlayTime();
	bool spawnedBoss=false;
	
	int currentArea=getCurrentArea();
	GameDataLoader *dataLoader=game->getDataLoader();
	int currentAreaZombies=0;
	if(currentArea==2)
	{
		//if (this->getSpriteManager()->getPlayer()->getPlayerObject()->hasItem("Diary Page") && this->getSpriteManager()->getPlayer()->getPlayerObject()->hasItem("Sword"))
		
	}

	if(zombieSpawnTimer>11)
	{
		if(spawnLocation>=9)
			{
				spawnLocation=2;
			}
		zombieSpawnTime++;
		if(zombieSpawnTime%zombieSpawnTimer==0)
		{

				if(currentArea==1)
				{

					currentAreaZombies=area1Zombies;
					
					if (currentAreaZombies<=getCurrentLevel()*10)
					{
				
						dataLoader->loadASprite(game,spawnLocation);
						area1Zombies++;
				
					}
				}
				if(currentArea==2)
				{
					currentAreaZombies=area2Zombies;
					if (currentAreaZombies<=getCurrentLevel()*10)
					{
				
						//dataLoader->loadASprite(game,spawnLocation);
						area2Zombies++;
						area1Zombies++;
					}
				}
				if(currentArea==3)
				{
					area1Zombies++;
					area2Zombies++;
					area1Zombies++;
					
				}

			
			
			spawnLocation++;
		}
	}
	
	if (time==100)//about 100 per 5 seconds
	{
		int x=spriteManager->getPlayer()->getPhysicalProperties()->getX();
		int y=spriteManager->getPlayer()->getPhysicalProperties()->getY();
		int viewPortx=viewport->getViewportX();
		int viewPorty=viewport->getViewportY();
		currentLevel=2;
		loadCurrentLevel(game,2);
		viewport->setViewportX(viewPortx);
		viewport->setViewportY(viewPorty);
		spriteManager->getPlayer()->getPhysicalProperties()->setX(x);
		spriteManager->getPlayer()->getPhysicalProperties()->setY(y);
		zombieSpawnTimer=30;
	}
	
	if(time==30000)
	{
		int x=spriteManager->getPlayer()->getPhysicalProperties()->getX();
		int y=spriteManager->getPlayer()->getPhysicalProperties()->getY();
		int viewPortx=viewport->getViewportX();
		int viewPorty=viewport->getViewportY();
		currentLevel=2;
		loadCurrentLevel(game,3);
		viewport->setViewportX(viewPortx);
		viewport->setViewportY(viewPorty);
		spriteManager->getPlayer()->getPhysicalProperties()->setX(x);
		spriteManager->getPlayer()->getPhysicalProperties()->setY(y);
		zombieSpawnTimer=100;
	}
	
	
	spriteManager->updateAllSprites();




}
int GameWorld::getCurrentAreaZombies()
{
	if(currentArea==1)
	{
		return area1Zombies;
	}
	if(currentArea==2)
	{
		return area2Zombies;
	}
	if(currentArea==3)
	{
		return area3Zombies;
	}
}
int GameWorld::setCurrentAreaZombies(int i)
{
	if(currentArea==1)
	{
		 area1Zombies=i;
	}
	if(currentArea==2)
	{
		return area2Zombies=i;
	}
	if(currentArea==3)
	{
		return area3Zombies=i;
	}
}