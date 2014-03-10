/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRKeyEventHandler.cpp

	See WRKeyEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "Cursor.h"
#include "WRKeyEventHandler.h"
#include "Game.h"
#include "GameInput.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameTimer.h"
#include "GamePhysics.h"
#include "StringTable.h"
#include "WindowsGameTimer.h"
#include "AnimatedSprite.h"
#include "SpriteManager.h"
#include "XactSound.h"

const int A_KEY = 0X41;
const int B_KEY = 0X42;
const int C_KEY = 0X43;
const int D_KEY = 0X44;


//const int X_KEY = 0X

void WRKeyEventHandler::handleKeyEvents(Game *game)
{
	wchar_t *title;
	wchar_t *counterText;
	
	GamePhysics *physics = game->getPhysics();
	GameInput *input = game->getInput();
	GameGraphics *graphics = game->getGraphics();
	GameWorld *world = game->getWorld();
	GameDataLoader *dataloader = game->getDataLoader();
	SpriteManager *spriteManager = world->getSpriteManager();
	AnimatedSprite *player =  spriteManager->getPlayer();
	Viewport *viewport = world->getViewport();
	unsigned int escKey = 0x1B;
	unsigned int iKey = (unsigned int)'I';
	unsigned int kKey = (unsigned int)'K';
	unsigned int lKey = (unsigned int)'L';


	
	if (input->isKeyDownForFirstTime(escKey)){
		if (game->getGameState() == INTRO_GAME_STATE)
		{
			game->changeGameState(GAME_PLAYING_GAME_STATE);
		}
		else if ((game->getGameState() == GAME_PLAYING_GAME_STATE) || (game->getGameState() == PAUSE_GAME_STATE) || (game->getGameState() == INVENTORY_GAME_STATE) || (game->getGameState()== CHECK_GAME_STATE))
			{
				if (game->isPaused())
				{
					game->changeGameState(GAME_PLAYING_GAME_STATE);
					game->setPaused(false);
				}
				else if (!game->isPaused())
				{
					game->setPaused(true);
					
					game->changeGameState(PAUSE_GAME_STATE);
					
				}
			}
		else
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
	}
	
	if (input->isKeyDownForFirstTime(kKey))
		{
			wchar_t *descriptor;
			//Item* itemFound = dataloader->getItemType(physics->getItemFound());
			if (game->isPaused() && game->getGameState()==CHECK_GAME_STATE)
			{
				game->changeGameState(GAME_PLAYING_GAME_STATE);
				game->setPaused(false);
			}
			else if (game->getGameState()==GAME_PLAYING_GAME_STATE && !game->isPaused())
			{
				physics->checkSparseObjects(game);
				game->setPaused(true);
				game->changeGameState(CHECK_GAME_STATE);
				if (game->getItemFound().compare("empty")!=0)
				{
					game->foundItem();
					game->getText()->clearText();
					string s = "You have found a " + game->getItemFound() + "!";
					descriptor = dataloader->stringToLCPWSTR(s);
					game->getText()->addText(descriptor, 360,140,1000,1000);
				}
				else if (!game->isInFrontOfObject())
				{
					game->getText()->clearText();
					string s = "Nothing to check here,\n move along...";
					descriptor = dataloader->stringToLCPWSTR(s);
					game->getText()->addText(descriptor, 360,140,1000,1000);
				}
				else
				{
					game->getText()->clearText();
					descriptor = game->getDescriptor();
					game->getText()->addText(descriptor, 360,110,1000,1000);
				}
			}
		}

	if (input->isKeyDownForFirstTime(iKey))
		{
			if (game->isPaused() && game->getGameState()==INVENTORY_GAME_STATE)
			{
				game->changeGameState(GAME_PLAYING_GAME_STATE);
				game->setPaused(false);
			}
			else if (game->getGameState()==GAME_PLAYING_GAME_STATE && !game->isPaused())
			{
				game->setPaused(true);
				game->changeGameState(INVENTORY_GAME_STATE);
			}
		}
		
	if (game->getGameState() == GAME_PLAYING_GAME_STATE)
	{
		unsigned int aKey = (unsigned int)'A';
		unsigned int dKey = (unsigned int)'D';
		unsigned int sKey = (unsigned int)'S';
		unsigned int wKey = (unsigned int)'W';
		unsigned int gKey = (unsigned int)'G';
		unsigned int zKey = (unsigned int)'Z';
		unsigned int xKey = (unsigned int)'X';
		unsigned int leftKey = (unsigned int)0x25;
		unsigned int rightKey = (unsigned int)0x27;
		unsigned int upKey = (unsigned int)0x26;
		unsigned int downKey = (unsigned int)0x28;
		unsigned int leftShiftKey = (unsigned int)0xA0;
		unsigned int pageUp = (unsigned int)0x21;
		unsigned int pageDown = (unsigned int)0x22;
		unsigned int onekey = (unsigned int) 0x31;
		unsigned int twokey = (unsigned int) 0x32;
		unsigned int threekey = (unsigned int) 0x33;
		const int CTRL_KEY = 0X11;
		bool isMoving=false;
		bool finishAttackLeft;
		player=spriteManager->getPlayer();
		int currentState = player->getCurrentState();
		player->getPhysicalProperties()->setVelocityX(0);
		player->getPhysicalProperties()->setVelocityY(0);

			//set static versions of the walkign animations
			if (!input->isKeyDown(aKey) && !input->isKeyDown(dKey) && !input->isKeyDown(sKey) && !input->isKeyDown(wKey))
			{
				switch (currentState)
				{
					case 0:
						player->setCurrentState(1);
					break;
					case 2:
						player->setCurrentState(3);
					break;
					case 4:
						player->setCurrentState(4);
					break;
					case 6:
						player->setCurrentState(7);
					break;
				}
			}
	

		int incX = 0;
		int incY = 0;
		bool moveViewport = false;
		int movementSpeed = player->getSpriteType()->getMovementSpeed();

		if (input->isKeyDown(leftKey))
		{
			incX = 15 * (-1);
			moveViewport = true;
		}
		if (input->isKeyDown(rightKey))
		{
			incX = 15;
			moveViewport = true;
		}
		if (input->isKeyDown(upKey))
		{
			incY = 15 * (-1);
			moveViewport = true;
		}
		if (input->isKeyDown(downKey))
		{
			incY = 15;
			moveViewport = true;
		}
		
		if (input->isKeyDown(aKey) && !isMoving && player->getCurrentState() < 8)
		{
			//set what direction player is facing
			player->getPhysicalProperties()->setDirection(0); //facing left
			//set Animation state
			if (player->getCurrentState()!=2)
				{
					player->setCurrentState(2);
				}
			//don't allow to move off world
			if ((player->getPhysicalProperties()->getX()) <= 0)
			{
				player->getPhysicalProperties()->setX(0);
			}
			else 
			{
			player->getPhysicalProperties()->setVelocityX(movementSpeed * (-1));
			isMoving=true;
			
			//move viewport of player moves too far.
				if ((player->getPhysicalProperties()->getX())<(viewport->getViewportX()))
				{
					incX = viewport->getViewportWidth() * (-1);
						moveViewport = true;
				}
			}
		}
		if (input->isKeyDown(dKey)&& !isMoving && player->getCurrentState() < 8)
		{
			player->getPhysicalProperties()->setDirection(2); //facing right
			if (player->getCurrentState()!=6)
				{
					player->setCurrentState(6);
				}
			if ((player->getPhysicalProperties()->getX()) >= (world->getWorldWidth() - player->getBoundingVolume()->getWidth()))
			{
				player->getPhysicalProperties()->setX(world->getWorldWidth()-player->getBoundingVolume()->getWidth());
			}
			else 
			{
			player->getPhysicalProperties()->setVelocityX(movementSpeed);
			isMoving=true;
			if ((player->getPhysicalProperties()->getX())>(viewport->getViewportX())+viewport->getViewportWidth())
				
					incX = viewport->getViewportWidth();
					moveViewport = true;
				}
			}
		
		if (input->isKeyDown(sKey) && !isMoving && player->getCurrentState() < 8)
		{
			player->getPhysicalProperties()->setDirection(3); //facing down
			if (player->getCurrentState()!=0)
				{
					player->setCurrentState(0);
				}
			if ((player->getPhysicalProperties()->getY()) >= (world->getWorldHeight() - player->getBoundingVolume()->getHeight()))
			{
				player->getPhysicalProperties()->setY(world->getWorldHeight()-player->getBoundingVolume()->getHeight());
			}
			else 
			{
			player->getPhysicalProperties()->setVelocityY(movementSpeed);
			isMoving=true;
			if (player->getPhysicalProperties()->getY()>(viewport->getViewportY()+viewport->getViewportHeight()))
				{
					incY = viewport->getViewportHeight();
					moveViewport = true;
				}
			}
		}
		if (input->isKeyDown(wKey) && !isMoving && player->getCurrentState() < 8)
		{
			player->getPhysicalProperties()->setDirection(1); //facing up
			if (player->getCurrentState()!=4)
				{
					player->setCurrentState(4);
				}
			if ((player->getPhysicalProperties()->getY()) <0)
			{
				player->getPhysicalProperties()->setY(0);
			}
			else 
			{
			player->getPhysicalProperties()->setVelocityY(movementSpeed*(-1));
			isMoving=true;
				if ((player->getPhysicalProperties()->getY())<(viewport->getViewportY()))
				{
					incY = viewport->getViewportHeight()*(-1);
					moveViewport = true;
				}
			}
		}
			if (!input->isKeyDown(aKey) && !input->isKeyDown(dKey) && !input->isKeyDown(sKey) && !input->isKeyDown(wKey))
			{
				switch (currentState)
				{
					case 0:
						player->setCurrentState(1);
					break;
					case 2:
						player->setCurrentState(3);
					break;
					case 4:
						player->setCurrentState(5);
					break;
					case 6:
						player->setCurrentState(7);
					break;
				}
			}
		


		if (input->isKeyDownForFirstTime(lKey) && player->getCurrentState() < 8)
		{
			if(game->getPlayerObject()->getAtkEquipped()->damage > 0)
			{
				if(player->getPhysicalProperties()->getDirection() == 0)
				{
					player->getSpriteType()->setAnimationSpeed(2);
					player->setCurrentState(9); //attacking left
					if(world->getSpriteManager()->getSwordLeft()->getAlpha() == 0)
					{
						world->getSpriteManager()->getSwordLeft()->getSpriteType()->setAnimationSpeed(1);
						world->getSpriteManager()->getSwordLeft()->getPhysicalProperties()->setX(world->getSpriteManager()->getPlayer()->getPhysicalProperties()->getX() - 64);
						world->getSpriteManager()->getSwordLeft()->getPhysicalProperties()->setY(game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getY());
						world->getSpriteManager()->getSwordLeft()->setAlpha(255);
						world->getSpriteManager()->getSwordLeft()->setCurrentState(0);
					}

				}

				else if(player->getPhysicalProperties()->getDirection() == 1)
				{
					player->getSpriteType()->setAnimationSpeed(2);
					player->setCurrentState(11); //attacking up
					if(world->getSpriteManager()->getSwordUp()->getAlpha() == 0)
					{
						world->getSpriteManager()->getSwordUp()->getSpriteType()->setAnimationSpeed(2);
						world->getSpriteManager()->getSwordUp()->getPhysicalProperties()->setX(world->getSpriteManager()->getPlayer()->getPhysicalProperties()->getX()-15);
						world->getSpriteManager()->getSwordUp()->getPhysicalProperties()->setY(game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties()->getY()- 34);
						world->getSpriteManager()->getSwordUp()->setAlpha(255);
						world->getSpriteManager()->getSwordUp()->setCurrentState(0);
					}

				}

				else if(player->getPhysicalProperties()->getDirection() == 2)
				{
					player->getSpriteType()->setAnimationSpeed(2);
					player->setCurrentState(10); //attacking right
				}

				else if(player->getPhysicalProperties()->getDirection() == 3)
				{
					player->getSpriteType()->setAnimationSpeed(4);
					player->setCurrentState(8); //attacking down
				}

				game->getPhysics()->checkEnemyHit(game);
			
			}
			else
			{
				if (game->isPaused() && game->getGameState()==CHECK_GAME_STATE)
				{
					game->changeGameState(GAME_PLAYING_GAME_STATE);
					game->setPaused(false);
				}
				else if (game->getGameState()==GAME_PLAYING_GAME_STATE && !game->isPaused())
				{
						game->changeGameState(CHECK_GAME_STATE);
						game->setPaused(true);
						game->getText()->clearText();
						string s = "You have no weapon equipped...";
						wchar_t *descriptor = dataloader->stringToLCPWSTR(s);
						game->getText()->addText(descriptor, 360,140,1000,1000);
				}
			}

		}

		/*AnimatedSprite *player = world->getSpriteManager()->getPlayer();
		if(player->getCurrentState() == 9 && player->getFrameIndex() == 5)//face left after
		{
			player->getSpriteType()->setAnimationSpeed(6);
			player->setCurrentState(3);
			player->getPhysicalProperties()->setX(player->getPhysicalProperties()->getX()+30);
		}*/


		if(player->getCurrentState() == 8 && player->getFrameIndex() == 3)//face down after
		{
			player->getSpriteType()->setAnimationSpeed(6);
			player->setCurrentState(0);
		}
		if(player->getCurrentState() == 9 && player->getFrameIndex() == 5)//face left after
		{
			player->getSpriteType()->setAnimationSpeed(6);
			player->setCurrentState(3);
		}
		if(world->getSpriteManager()->getSwordLeft()->getFrameIndex() == 5)//reset sword to invisible
		{
			world->getSpriteManager()->getSwordLeft()->setAlpha(0);
		}
		if(player->getCurrentState() == 10 && player->getFrameIndex() == 5)//face right after
		{
			player->getSpriteType()->setAnimationSpeed(6);
			player->setCurrentState(7);
		}
		if(player->getCurrentState() == 11 && player->getFrameIndex() == 3)//face up after
		{
			player->getSpriteType()->setAnimationSpeed(6);
			player->setCurrentState(5);
		}
		if(world->getSpriteManager()->getSwordUp()->getFrameIndex() == 3)//reset sword to invisible
		{
			world->getSpriteManager()->getSwordUp()->setAlpha(0);
		}


		//CHEATS =============================================================
		int currentLevel = world->getCurrentLevel();
		if (input->isKeyDown(onekey))
		{
			world->setCurrentArea(1);
			world->loadCurrentLevel(game, currentLevel);
			return;
		}
		if (input->isKeyDown(twokey))
		{
			world->setCurrentArea(2);
			world->loadCurrentLevel(game, currentLevel);
			return;
		}
		if (input->isKeyDown(threekey))
		{
			world->setCurrentArea(3);
			world->loadCurrentLevel(game, currentLevel);
			return;
		}

		if (input->isKeyDown(leftShiftKey))
		{
			///Do something
		}
		
		if (input->isKeyDownForFirstTime(pageUp))
		{
			player->getSpriteType()->incMovementSpeed();
		}
		else if (input->isKeyDownForFirstTime(pageDown))
		{
			if (movementSpeed>1)
			{
				player->getSpriteType()->decMovementSpeed();
			}
		}


		//GOD MODE
		if(input->isKeyDown(CTRL_KEY) && input->isKeyDownForFirstTime(gKey))
		{
			if(player->getPhysicalProperties()->isCollidable() == false)
			{
				player->getPhysicalProperties()->setCollidable(true);
				player->setAlpha(255);
			}
			else
			{
				player->getPhysicalProperties()->setCollidable(false);
				player->setAlpha(70);
			}
			
		}

		//GET KEY
		if(input->isKeyDown(CTRL_KEY) && input->isKeyDownForFirstTime(zKey))
		{
		
				game->setItemFound("Key");
				game->foundItem();
				game->setItemFound("empty");		
		}


		//GET KEY
		if(input->isKeyDown(CTRL_KEY) && input->isKeyDownForFirstTime(xKey))
		{
				game->setItemFound("Sword");
				game->foundItem();
				game->setItemFound("empty");		
		}


	//END CHEATS ====================================================
		if (moveViewport)
		{
			GameWorld *world = game->getWorld();
			Viewport *viewport = world->getViewport();
			viewport->moveViewport( incX,
									incY,
									world->getWorldWidth(),
									world->getWorldHeight());
		}
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		StringTable *guiStringTable = graphics->getGUITextureManager()->getStringTable();
		int greenCursorID = guiStringTable->getIndexOfStringFromStringTable(L"textures/gui/cursor/green_cursor.bmp");
		int redCursorID = guiStringTable->getIndexOfStringFromStringTable(L"textures/gui/cursor/red_cursor.bmp");
		int currentCursorID = cursor->getActiveCursorID();
		if (currentCursorID == greenCursorID)
			cursor->setActiveCursorID(redCursorID);
		else
			cursor->setActiveCursorID(greenCursorID);
	}

	if (input->isKeyDown(VK_HOME))
	{
		WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();
		int fps = timer->getTargetFPS();
		if (fps < 100)
			timer->setTargetFPS(fps + 1);
	}
	else if (input->isKeyDown(VK_END))
	{
		WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();
		int fps = timer->getTargetFPS();
		if (fps > 1)
			timer->setTargetFPS(fps - 1);
	}
}