/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Game.cpp

	See Game.h for a class description.
*/
#include "stdafx.h"
#include "Button.h"
#include "Cursor.h"
#include "Game.h"
#include "GameAI.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameInput.h"
#include "GameOS.h"
#include "GamePhysics.h"
#include "GameText.h"
#include "GameTimer.h"
#include "GameWorld.h"
#include "PlayerStatsGui.h"
#include "ScreenGUI.h"
#include "TextGenerator.h"
#include "TextFileWriter.h"
#include "TextFileReader.h"
#include "Player.h"
/*
	constructEmptyWCHAR_TArray - This is a simple helper
	method for constructing a wchar_t object on the heap
	and filling it in with some literal character data.
*/
bool spawnBoss=false;
wchar_t* constructEmptyWCHAR_TArray(LPCWSTR initChars)
{
	size_t initSize = wcslen(initChars) + 2;
	wchar_t *initArray = new wchar_t[initSize];
	initArray[0] = '\0';
	swprintf(initArray, initSize, initChars);
	return initArray;
}
/*
	Game  - Constructor, this method begins the 
	construction and loading of all major game objects. 
	This method only needs to be called once, when the 
	application is first started.

	NOTE: This constructor will not initialize the custom
	game objects: graphics, input, os, & timer.
	These must be constructed separately and fed to this
	object using the init method. Doing it this way allows for
	more platform independence.
*/
Game::Game()
{
	// DEFAULT GAME STATE
	gameState = SPLASH_SCREEN_GAME_STATE;
	paused = false;
	gameOverTimer=0;
	bgSound="";
	itemFound="empty";
	knightsAwake = false;
	physicsCheck = false;
	playerDead	 = false;
	deadTimer = 0;

	// CONSTRUCT BASIC GAME OBJECTS
	player		= new Player();
	ai			= new GameAI();
	physics		= new GamePhysics();
	dataLoader	= new GameDataLoader();
	gui			= new GameGUI();
	text		= new GameText();
	world		= new GameWorld();

	// OUR DEBUGGING LOG FILE
	debugLogFileName = new wchar_t[30];
	debugLogFileName[0] = '\0';
	wcscat(debugLogFileName, L"DebugOutput.txt");
	writer = new TextFileWriter();
	writer->initFile(debugLogFileName);
	reader = new TextFileReader();
	reader->loadFiles(this);
	int playTime = 0;

	// NOTE THAT graphics, input, os, & timer
	// ARE CUSTOM GAME OBJECTS. DEPENDING ON WHAT TECHNOLOGY 
	// IS TO BE USED THESE OBJECT SHOULD BE CONSTRUCTED
	// AND THEN FED TO THIS Game USING THE init METHOD
}

/*
	init - This method is to be used to feed this Game
	the constructed, technology-specific, objects. In addition,
	In addition, this method requires gameDataPathName, which
	specifies the location of the game design files.
*/
void Game::init(wchar_t *initGameDataPathName,
				GameGraphics *initGraphics,
				GameOS *initOS,
				GameInput *initInput,
				GameTimer *initTimer)
{
	// INITIALIZE ALL OF THE GAME'S CUSTOM OBJECTS
	graphics = initGraphics;
	os = initOS;
	input = initInput;
	timer = initTimer;

	// AND NOW LOAD THE GAME GUI
	gameDataPathName = initGameDataPathName;
	dataLoader->loadGUI(this);
}

/*
	~Game - Desctructor, it recovers memory allocated for
	the game objects. If Game is deleted, the application
	is closing, so everything should be cleaned up.
*/
Game::~Game() 
{
	delete ai;
	delete dataLoader;
	delete graphics;
	delete gui;
	delete input;
	delete os;
	delete physics;
	delete text;
	delete timer;
	delete world;
	delete writer;
	delete debugLogFileName;
}


void Game::foundItem()
{
	Item *itemToAdd = this->getDataLoader()->getItemType(itemFound);
	player = this->getPlayerObject();
	int inventoryIndex = player->getInventoryIndex();
	player->getInventoryImages()->at(inventoryIndex) = itemToAdd;
	player->increaseInventoryIndex();
	//
	player->updateInventoryBar(player->getInventoryImages());
}

void Game::changeGameState(int initGameState)
{
	if (initGameState == GAME_PLAYING_GAME_STATE)
	{
		if (!paused)
		{
		world->loadCurrentLevel(this, 1);
		
	

		}
	}
	else if (initGameState == SPLASH_SCREEN_GAME_STATE)
	{
		world->unloadCurrentLevel(this);
		XactSound *sound = this->getSound();
		sound->stopSound(bgSound);
		
			
	}
	else if (initGameState == INTRO_GAME_STATE)
	{
		this->getDataLoader()->loadIntro(this);
		this->getSound()->playSound("intro");
	}
	this->getText()->clearText();
	gameState = initGameState;
}

/*
	killGameApplication - This method changes the game loop
	control such that it will not iterate again. In addition,
	it posts a Windows message to close the application. The
	result is that the application will exit after the current
	game loop completes.
*/
void Game::killGameApplication()
{
	// END THE GAME LOOP
	changeGameState(EXIT_GAME_STATE);

	// AND KILL THE WINDOW
	os->killApp();
}

/*
	reloadAllDevices - Windows applications must cooperate
	with other running applications, so when someone hits
	ALT-TAB and switches from a full-screen game, it might
	lose ownership of the graphics card. This method can
	be called when a full-screen application retains ownership
	of all necessary resources such that all necessary
	data (like textures, sound, music, etc.) can be reloaded.
*/
void Game::reloadAllDevices()
{
	graphics->reloadGraphics();

	// WE MIGHT ADD MORE LATER
}
	
/*
	runGameLoop - This is the game loop management method.
	It runs continuously while the game is active. Once per
	frame it instructs the major game objects to get
	user input, record user input, update the GUI state, 
	update the sprites' states using AI and input, perform
	collision detection and resolution (physics), render
	the screen, etc.

	This loop is timed such that everything is kept to a 
	consistent framerate, thus the game should run 
	consistently on all machines.
*/
void Game::runGameLoop()
{
	// FIRST PROFILE?
	bool firstTimeThroughLoop = true;
	physicsCheck = false;
	// LET'S START THE TIMER FROM SCRATCH
	timer->resetTimer();
	playTime = 0;

	// KEEP RENDERING UNTIL SOMEONE PULLS THE PLUG
	while(isGameActive())
	{
		// MOVE ALONG WINDOWS MESSAGES, THIS ALLOWS
		// US TO GET USER INPUT
		os->processOSMessages();

		// GET USER INPUT AND UPDATE GAME, GUI, OR PLAYER
		// STATE OR WHATEVER IS NECESSARY
		input->processInput(this);

		// USE THE INPUT TO UPDATE THE GAME
		this->processGameData();


		
		// AND RENDER THE GAME
		graphics->renderGame(this);
		sound->doWork();
		if (gameState == INTRO_GAME_STATE)
		{
			gameOverTimer++;
		}
		if (gameOverTimer>1200 && gameState == INTRO_GAME_STATE)
		{
			gameOverTimer=0;
			this->changeGameState(GAME_PLAYING_GAME_STATE);
		}
		if(world->getCurrentArea()==2)
		{
			//if (world->getSpriteManager()->getPlayer()->getPlayerObject()->hasItem("Diary Page") && world->getSpriteManager()->getPlayer()->getPlayerObject()->hasItem("Sword")&& !spawnBoss)
			if (world->getSpriteManager()->getPlayer()->getPlayerObject()->hasItem("Frying Pan") && !spawnBoss)
			{
				world->loadCurrentLevel(this,3);
				spawnBoss=true;
				//bool spawnedBoss=true;
			}
		}
	}

	// SHUTDOWN AND RELEASE ALL RESOURCES
    shutdownEverything();
}

/*
	processGameLogic - The code inside this method progresses the
	game state forward. It contains the game logic and should only
	be executed when the game is in-progress.
*/
void Game::processGameLogic()
{

	// NOW PERFORM ALL AI
		if (gameState == GAME_PLAYING_GAME_STATE && !paused)
	{ai->update(this);

	// NOW CORRECT FOR COLLISION DETECTION
		physics->update(this);}

	// UPDATE THE WORLD, INCLUDING SPRITE AND
	// PARTICLE POSITIONS, FACTORING IN INPUT
	// AND AI
	world->update(this);
}

/*
	processGameData - This method directs game logic to be
	executed or not, depending on the game state, it addition
	it triggers the building or render lists and game loop
	timing each frame.
*/
void Game::processGameData()
{

	
	if(playerDead)
	{
		if(deadTimer > 120)
		{
			deadTimer = 0;
			playerDead = false;
			changeGameState(5);//GAMEOVER
			
		}
		else if(deadTimer==1)
		{
			this->getSound()->playSound("i must2");
			deadTimer++;
		}
		else
		{
			deadTimer++;
			this->processGameLogic();
		}
	}

	// WE ONLY PERFORM GAME LOGIC IF THE GAME
	// IS IN PROGRESS
	if (gameState == GAME_PLAYING_GAME_STATE && !paused) 
	{
		playTime++;
	}
	if ((gameState == GAME_PLAYING_GAME_STATE && !paused) || gameState == INTRO_GAME_STATE && !playerDead)
	{
		this->processGameLogic();
	}
	else if (gameState == EXIT_GAME_STATE)
	{
		this->killGameApplication();
	}

	// UPDATE TEXT
	TextGenerator *tg = text->getTextGenerator();
	tg->updateText(this);

	// BUILD THE RENDER LISTS
	graphics->fillRenderLists(this);

	// KEEP THE FRAME RATE CONSISTENT
	timer->timeGameLoop();
}

/*
	shutdownEverything - This method should be called after
	the game application has chosen to close. It will result
	in the releasing of all game resources.
*/
void Game::shutdownEverything()
{
	graphics->shutdownGraphics();
	input->shutdownInput();

	// WE MAY SHUTDOWN OTHER THINGS HERE LIKE SOUND & MUSIC
	// RESOURCES IN THE FUTURE
}

/*
	writeOutput - This method writes text to the debugging log file.
*/
void Game::writeOutput(wchar_t *output)	
{	
	writer->writeText(output);
}
