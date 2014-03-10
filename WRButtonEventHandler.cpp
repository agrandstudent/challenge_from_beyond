/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRButtonEventHandler.cpp

	See WRButtonEventHandler.h for a class
	description.
*/

#include "stdafx.h"
#include "Game.h"
#include "GameWorld.h"
#include "WRButtonEventHandler.h"
#include "XactSound.h"
#include "PlayerStatsGui.h"
#include "GameText.h"


void WRButtonEventHandler::handleButtonEvents(Game *game, 
											  wchar_t *command)
{
	// SOMEONE PRESSED THE Exit BUTTON, SO CHANGE THE
	// Game State TO END THE APPLICATION
	GameDataLoader *dataloader = game->getDataLoader();
	Player *playerObject = game->getPlayerObject();
	
	if (wcscmp(command, L"Exit") == 0)
	{
		int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"You are about to exit the game. \n Are you sure?",
        (LPCWSTR)L"Are you sure?",
        MB_ICONWARNING | MB_YESNO | MB_TASKMODAL
		);
		switch (msgboxID)
		{
		case IDYES:
		game->changeGameState(EXIT_GAME_STATE);
	        break;
	    case IDNO:
			// TODO: add code
			break;
		}
	}
	else if (wcscmp(command, L"Start") == 0)
	{
		game->getWorld()->setFromArea(1);
		Player *playerObject = game->getPlayerObject();
		playerObject->setHP(6);
		wchar_t *fileName = constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR("textures/gui/overlays/itemButton.png"));
		int inventoryButtonID = game->getGraphics()->getGUITextureManager()->loadTexture(fileName);
					Item *itemtorender = new Item();
					itemtorender->imageID = inventoryButtonID;
					playerObject->setAtkEquipped(itemtorender);
					playerObject->setMscEquipped(itemtorender);
		playerObject->resetHP(playerObject->getHPImages());
		game->getDataLoader()->clearInventory(game);
		game->getDataLoader()->clearSelectedItem();
		//game->changeGameState(GAME_PLAYING_GAME_STATE);
		game->getWorld()->setCurrentLevel(1);
		game->setPlayTime(0);
		game->changeGameState(INTRO_GAME_STATE);

	}
	else if (wcscmp(command, L"Quit") == 0)
	{
		int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"You are about to reset the game. \n Are you sure?",
        (LPCWSTR)L"Are you sure?",
        MB_ICONWARNING | MB_YESNO | MB_TASKMODAL
		);

		switch (msgboxID)
		{
		case IDYES:
		game->setPaused(false);
			game->changeGameState(SPLASH_SCREEN_GAME_STATE);
			
			break;
		case IDNO:
	        // TODO: add code
			break;
		}
	}
	else if (wcscmp(command, L"Controls") == 0)
	{
		game->changeGameState(CONTROLS_GAME_STATE);
	}
	else if (wcscmp(command, L"Help") == 0)
	{
		game->changeGameState(HELP_GAME_STATE);
	}
	else if (wcscmp(command, L"About") == 0)
	{
		game->changeGameState(ABOUT_GAME_STATE);
	}
	else if (wcscmp(command, L"Back") == 0)
	{
		if (game->isPaused())
		{
			game->changeGameState(PAUSE_GAME_STATE);
		}
		else
		{
			game->changeGameState(SPLASH_SCREEN_GAME_STATE);
		}
	}
	else if (wcscmp(command, L"Resume") == 0)
	{
		game->changeGameState(GAME_PLAYING_GAME_STATE);
		game->setPaused(false);
	}
	else if (wcscmp(command, L"Pause") == 0)
	{
		game->setPaused(true);
		game->changeGameState(PAUSE_GAME_STATE);
	}
	else if (wcscmp(command, L"Restart") == 0)
	{
		game->setPaused(false);
		game->changeGameState(SPLASH_SCREEN_GAME_STATE);
		game->getWorld()->setCurrentArea(1);
		game->getWorld()->setCurrentAreaZombies(0);
		game->getWorld()->setCurrentArea(2);
		game->getWorld()->setCurrentAreaZombies(0);
		game->getWorld()->setCurrentArea(3);
		game->getWorld()->setCurrentAreaZombies(0);
		game->getWorld()->setCurrentArea(1);
	}
	else if (wcscmp(command, L"Equip") == 0)
	{
		Item *selecteditem = dataloader->getSelectedItem();
		if (selecteditem->name.compare("empty")!=0 && selecteditem->damage>0)
		{
			playerObject->setAtkEquipped(selecteditem);
		}
		else if (selecteditem->name.compare("empty")!=0 && selecteditem->damage==0)
		{
			playerObject->setMscEquipped(selecteditem);
		}
	}
	else
	{
		for (int i = 0; i<15;i++)
		{
			stringstream out;
			out << i;
			string buttonName = "item" + out.str();
			wchar_t *commandName = dataloader->stringToLCPWSTR(buttonName);
			if (wcscmp(command,commandName)==0)
			{
				Item *currentItem = playerObject->getInventoryImages()->at(i);
				dataloader->setSelectedItem(currentItem);
				if (currentItem->name.compare("empty")!=0)
				{
					game->getText()->clearText();
					stringstream dmg;
					dmg << currentItem->damage;
					string damage = dmg.str();
					string s = currentItem->name + "\n\nDamage: " + damage + "\n\nDescription: " + currentItem->description1
						+ "\n" + currentItem->description2 + "\n" + currentItem->description3 + "\n" + currentItem->description4;
					wchar_t *descriptor = dataloader->stringToLCPWSTR(s);
					game->getText()->addText(descriptor, 300,200,1000,1000);
				}
				else
				{
					game->getText()->clearText();
					string s = "";
					wchar_t *descriptor = dataloader->stringToLCPWSTR(s);
					game->getText()->addText(descriptor, 330,200,1000,1000);
				}
			
			}
		}
	}
}