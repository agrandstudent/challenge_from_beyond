/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WorldRenderingGame.cpp

	This is a test game application, a game that
	demonstrates use of	the game engine simply
	for rendering images and text as well as for reacting to
	key presses and button cicks. Students will extend
	this for their projects, gradually adding other components,
	like additional gui controls, tiling, sprites, collision 
	detection, etc.
 */
#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>

// GAME OBJECT INCLUDES
#include "Cursor.h"
#include "ScreenGUI.h"
#include "PlayerStatsGui.h"
#include "Game.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameInput.h"
#include "GameOS.h"
#include "GameText.h"
#include "GameDataLoader.h"
#include "XactSound.h"


// EMPTY GAME INCLUDES
#include "WRButtonEventHandler.h"
#include "WRKeyEventHandler.h"
#include "WRTextGenerator.h"

// WINDOWS PLATFORM INCLUDES
#include "WindowsGameOS.h"
#include "WindowsGameInput.h"
#include "WindowsGameTimer.h"

// DIRECTX INCLUDES
#include "DirectXGraphics.h"
#include "DirectXTextureManager.h"

// METHODS OTHER THAN WinMain DEFINED BELOW
wchar_t*	constructEmptyWCHAR_TArray(LPCWSTR initChars);
void		initWRgui(Game *game);
GameDataLoader *dataloader;

	wchar_t *fileName;
	int imageID;
	string file;

/*
	WinMain - This is the application's starting point. In this
	method we will construct a Game object, then construct all the
	custom data for our game, and then initialize the Game with
	our custom data. We'll then start the game loop.
*/
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// USE WINDOWED MODE (ONE LESS HEADACHE)
	bool fullscreen = false;

	// CREATE A GAME
	file = "";
	Game *worldRenderingGame = new Game();
	

	// SPECIFY THE DIRECTORY WHERE ALL GAME DESIGN FILES
	// ARE TO BE LOADED FROM
	wchar_t *gameDataPathName = constructEmptyWCHAR_TArray(L"design/");

	// WE'RE USING THE WINDOWS PLATFORM, SO MAKE A CUSTOM
	// GameOS OBJECT (WindowsGameOS), FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	wchar_t *gameTitle = constructEmptyWCHAR_TArray(L"World Scrolling Game");
	WindowsGameOS *os = new WindowsGameOS(	hInstance, 
											nCmdShow,
											fullscreen,
											gameTitle,
											worldRenderingGame);

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *graphics = new DirectXGraphics(worldRenderingGame);
	graphics->init();
	graphics->initGraphics(os->getWindowHandle(), fullscreen);
	graphics->initTextFont(22);
	// SOUND STUFF FROM XACT	
	XactSound *sound= new XactSound(worldRenderingGame);
	sound->intiSound();
	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsGameInput *input = new WindowsGameInput();

	// AND THE TIMER
	WindowsGameTimer *timer = new WindowsGameTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	worldRenderingGame->init(	gameDataPathName,
						(GameGraphics*)graphics,
						(GameOS*)os,
						(GameInput*)input,
						(GameTimer*)timer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	WRTextGenerator *textGenerator = new WRTextGenerator();
	textGenerator->initText(worldRenderingGame);
	worldRenderingGame->getText()->setTextGenerator((TextGenerator*)textGenerator);

	// LOAD THE GUI STUFF, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	initWRgui(worldRenderingGame);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	WRButtonEventHandler *eventHandler = new WRButtonEventHandler();
	GameGUI *gui = worldRenderingGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)eventHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	WRKeyEventHandler *keyHandler = new WRKeyEventHandler();
	input->registerKeyHandler((KeyEventHandler*)keyHandler);

	GameWorld *world = worldRenderingGame->getWorld();
	dataloader = worldRenderingGame->getDataLoader();

	// START THE GAME LOOP
	worldRenderingGame->runGameLoop();

	return 0;
}

/*
	initWRgui - This method builds a GUI for the Empty Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void initWRgui(Game *game)
{
	GameGraphics *graphics = game->getGraphics();
	GameDataLoader *dataLoader = game->getDataLoader();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(255, 255, 255);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(96, 128, 224);

		// SETUP THE CURSOR
	vector<int> *imageIDs = new vector<int>();

	PlayerStatsGui *screenGUI = new PlayerStatsGui();
	Player *playerObject = game->getPlayerObject();
	OverlayImage *imageToAdd;
	Button *buttonToAdd;
	wchar_t *buttonCommand;
	int normalTextureID;
	int mouseOverTextureID;
	int initX;
	int initY;
	int initWidth;
	int initHeight;

	file = game->getDataLoader()->getGameMenuFile();

	using namespace std;
	wchar_t *value;
	ifstream inputFile;
	string lineRead;
	inputFile.open(file);
	if (inputFile)
	{
		char inputLine[255];
		stringstream ss;
		while (getline(inputFile, lineRead))
		{
			ss<<lineRead;
			while(getline(ss,lineRead,','))
			{
				if(lineRead.compare("cursor")==0)
				{
					getline(ss,lineRead,',');
					fileName= dataLoader->stringToLCPWSTR(lineRead);
					imageID = guiTextureManager->loadTexture(fileName);
					imageIDs->push_back(imageID);
					Cursor *cursor = new Cursor();
					cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
					gui->setCursor(cursor);
				}
				else if (lineRead.compare("splashScreen")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					gui->addScreenGUI(screenGUI);
				}
				else if (lineRead.compare("exit")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Exit");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("startGame")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Start");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("about")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"About");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("controls")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Controls");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("help")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Help");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("equip")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Equip");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("controlsScreen")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					gui->addScreenGUI(screenGUI);
				}
				else if (lineRead.compare("back")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Back");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("restart")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Restart");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("resume")==0)
				{
					buttonToAdd = new Button();
					buttonCommand = constructEmptyWCHAR_TArray(L"Resume");
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					normalTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					mouseOverTextureID = guiTextureManager->loadTexture(fileName);
					getline(ss, lineRead, ',');
					initX = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initY = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initWidth = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					initHeight = atoi(lineRead.c_str());
					buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
					screenGUI->addButton(buttonToAdd);
				}
				else if (lineRead.compare("helpScreen")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					gui->addScreenGUI(screenGUI);
				}
				else if (lineRead.compare("aboutScreen")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					gui->addScreenGUI(screenGUI);
				}
				else if (lineRead.compare("gameOverScreen")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					gui->addScreenGUI(screenGUI);
				}
				else if (lineRead.compare("inGameScreen")==0)
				{
					screenGUI = new PlayerStatsGui();
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 255;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);

					//equipBar images
					
					
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR("textures/gui/overlays/itemButton.png"));
					int inventoryButtonID = guiTextureManager->loadTexture(fileName);
					Item *itemtorender = new Item();
					itemtorender->imageID = inventoryButtonID;
					playerObject->setAtkEquipped(itemtorender);
					playerObject->setMscEquipped(itemtorender);
					//initialize inventory empty images
					dataloader->clearInventory(game);
					/*for (int i=0; i<15; i++)
					{
						playerObject->setInventoryImage(i,inventoryButtonID);
					}*/

					//hp images
					while(getline(ss, lineRead, ','))
					{
						fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
						imageID = guiTextureManager->loadTexture(fileName);
						imageToAdd = new OverlayImage();
						getline(ss, lineRead, ',');
						imageToAdd->x = atoi(lineRead.c_str());
						getline(ss, lineRead, ',');
						imageToAdd->y = atoi(lineRead.c_str());
						imageToAdd->z = 0;
						imageToAdd->alpha = 255;
						getline(ss, lineRead, ',');
						imageToAdd->width = atoi(lineRead.c_str());
						getline(ss, lineRead, ',');
						imageToAdd->height = atoi(lineRead.c_str());
						imageToAdd->imageID = imageID;
						playerObject->addHPIcon(imageToAdd);
						playerObject->resetHP(playerObject->getHPImages());
					}
					screenGUI->setHPBar(playerObject->getHPBar());
					screenGUI->setEquipBar(playerObject->getEquipBar());
					gui->addScreenGUI(screenGUI);

				}
				else if (lineRead.compare("pauseScreen")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);

					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 255;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					screenGUI->setHPBar(playerObject->getHPBar());
					screenGUI->setEquipBar(playerObject->getEquipBar());
					gui->addScreenGUI(screenGUI);
				}
				else if (lineRead.compare("inventory")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);

					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 255;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					screenGUI->setHPBar(playerObject->getHPBar());
					screenGUI->setEquipBar(playerObject->getEquipBar());
					playerObject->updateInventoryBar(playerObject->getInventoryImages());
					screenGUI->setInventoryBar(playerObject->getInventoryBar());
					for (int i = 0; i <15;i++)
					{
						buttonToAdd = new Button();
						stringstream out;
						out << i;
						string buttonName = "item" + out.str();
						buttonCommand = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(buttonName));
						fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR("textures/gui/overlays/itemButton.png"));
						normalTextureID = guiTextureManager->loadTexture(fileName);
						mouseOverTextureID = guiTextureManager->loadTexture(fileName);
						initX = (290 + 8 + (i%5)*95);
						initY = (450 + 8 + (i/5)*90);
						initWidth = 50;
						initHeight = 50;
						buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							initX,
							initY,
							0,
							255,
							initWidth,
							initHeight,
							false,
							buttonCommand);
						screenGUI->addButton(buttonToAdd);
					}
					gui->addScreenGUI(screenGUI);
				}
				else if (lineRead.compare("inventoryRow")==0)
				{
					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 255;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
				}
				else if (lineRead.compare("checkScreen")==0)
				{
					getline(ss, lineRead, ',');
					screenGUI = new PlayerStatsGui();
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 200;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);

					getline(ss, lineRead, ',');
					fileName = constructEmptyWCHAR_TArray(dataLoader->stringToLCPWSTR(lineRead));
					imageID = guiTextureManager->loadTexture(fileName);
					imageToAdd = new OverlayImage();
					getline(ss, lineRead, ',');
					imageToAdd->x = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->y = atoi(lineRead.c_str());
					imageToAdd->z = 0;
					imageToAdd->alpha = 255;
					getline(ss, lineRead, ',');
					imageToAdd->width = atoi(lineRead.c_str());
					getline(ss, lineRead, ',');
					imageToAdd->height = atoi(lineRead.c_str());
					imageToAdd->imageID = imageID;
					screenGUI->addOverlayImage(imageToAdd);
					screenGUI->setHPBar(playerObject->getHPBar());
					screenGUI->setEquipBar(playerObject->getEquipBar());
					gui->addScreenGUI(screenGUI);

					/////////////////////////////
				}
			}
			ss.clear();
		}
	}

}