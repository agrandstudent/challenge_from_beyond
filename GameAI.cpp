#pragma once

/*	
	Author: _______________

	GameAI.h

	This class will be used to manage all game bots, and the
	Artificial Intelligence associated with them. Each frame
	we will update the AI state, which should update all bots
	accordingly.

	Students will implement this as part of their projects.
*/

// PREPROCESSOR INCLUDE STATEMENTS
#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameWorld.h"
#include "RenderList.h"
#include "GamePhysics.h"
#include "GameAI.h"
#include "Bot.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Viewport.h"
#include <vector>
#include "TiledLayer.h"	
#include "XactSound.h"


Bot *bot;
vector<int> *instructions;

int counter;
int instructionNumber;
int instruction;
int cycleLimit;
int state;
AnimatedSprite *sprite;
PhysicalProperties *pp;
int distanceX;
int distanceY;
vector<AnimatedSprite*> *projectileVector;

GameAI::GameAI()
{
	bots = new vector<Bot*>();
	projectileVector = new vector<AnimatedSprite*>();
}

void GameAI::clearBots()
{
	bots->clear();
}

void GameAI::addBot(Bot *bot)
{
	bots->push_back(bot);
}
	// WE'LL DEFINE THESE METHODS LATER
	void GameAI::update(Game *game)
	{
		GameDataLoader *dataloader = game->getDataLoader();
		GameWorld *world = game->getWorld();
		AnimatedSprite *player = world->getSpriteManager()->getPlayer();
		
		for (int i = 0; i<bots->size();i++)
		{
			
			bot = bots->at(i);
			int movementSpeed = bot->getAnimatedSprite()->getSpriteType()->getMovementSpeed();
			instructionNumber = bot->getInstructionNumber();
			instruction = bot->getInstruction(instructionNumber);
			sprite = bot->getAnimatedSprite();
			pp=sprite->getPhysicalProperties();
			state=sprite->getCurrentState();
			int spriteID=sprite->getSpriteType()->getSpriteTypeID();
			bool isBoss = false;
			bool isKnight = false;
			bool isVampire=false;
			bool spawnBoss=false;
			
			if (sprite->getSpriteType()->getSpriteTypeID()==2)
			{
				isBoss=true;
				if(sprite->getAlpha()==0)
				{
					XactSound *sound=game->getSound();
					sound->playSound("banshi");
					OverlayImage *sparseTileToCheck;

					for(int i=0; i < game->getWorld()->getSparseLayer()->getNumberOfSparseTiles();i++)
					{
						OverlayImage *sparseObject = game->getWorld()->getSparseLayer()->getTile(i);
						if (sparseObject->name=="boat")
						{
							sparseObject->alpha=255;
						}
					}
					sprite->setAlpha(1);
				}
				
			}
			if (sprite->getSpriteType()->getSpriteTypeID()==4)
			{
				isKnight=true;
			}
			if(sprite->getSpriteType()->getSpriteTypeID()==1)
			{
				int check=0;
			}
			if(sprite->getSpriteType()->getSpriteTypeID()==7)
			{
				isVampire=true;
			}

			//check if death animation is finished for zombie
			if(sprite->getCurrentState() > 7 && sprite->getFrameIndex() == 8)
			{
				if(sprite->getAlpha()!=0)
					world->setCurrentAreaZombies(world->getCurrentAreaZombies()-1);
				sprite->setAlpha(0);
				bots->erase(bots->begin()+i);
			}


			//check if death animation is finished for vampire
			if(sprite->getCurrentState() == 9 && sprite->getFrameIndex() == 4 && sprite->getSpriteType()->getSpriteTypeID() == 7)
			{
				sprite->setAlpha(0);
			}

			if(sprite->getHP() > 0 && (!isKnight || game->knightsAreAwake()))
			{
				
				
			
				distanceX = fabs((pp->getX()+ sprite->getSpriteType()->getTextureWidth()/2) - (player->getPhysicalProperties()->getX() + player->getSpriteType()->getTextureWidth()/2));
				distanceY = fabs((pp->getY() + sprite->getSpriteType()->getTextureHeight()/2) - (player->getPhysicalProperties()->getY() + player->getSpriteType()->getTextureHeight()/2));
				if(instruction==7)//vampire attack 
				{
					
					int currentState=sprite->getCurrentState();
					if(currentState==0)
					{
						sprite->setCurrentState(8);
					

					}
					else if(currentState==1)
					{
						sprite->setCurrentState(6);
					}
					else if(currentState==2)
					{
						sprite->setCurrentState(7);
					}
					else if(currentState==3)
					{
						sprite->setCurrentState(5);
					}
					else if(currentState>=4)
					{
						if(bot->getCount()>=1)
						{
							if(sprite->getFrameIndex()>=4&&currentState==4)
							{
								bot->incrementCount();
								bot->resetInstructionNumber();
							}
						
						}
						if(currentState!=4&&sprite->getFrameIndex()>4)
							sprite->setCurrentState(4);
					}
					pp->setVelocityX(0);
					pp->setVelocityY(0);
				

				}
				else if(instruction==6)//vampire move
				{
					int currentState=sprite->getCurrentState();
				
					if (distanceX < 500 && distanceY < 500&&currentState<4)//charge
					{
						//bot->incrementCount();
						int count=bot->getCount();
						if(bot->getCount()>90)
						{
							bot->resetCount();
							bot->incrementInstructionNumber();
						}
						else if(bot->getCount()%10==0)
						{
						
							int x=guessSpriteLocation(player,90-count,true);
							int y=guessSpriteLocation(player,90-count,false);
							int botY=pp->getY();
							int botX=pp->getX();
							int deltax=x-botX;
							int deltay=y-botY;
							bool yPositive=false;
							bool xPositive=false;
							bool moveInX=false;
							if(deltax>=0)//player to right
							{
							
								xPositive=true;
							}
							if(deltay>=0)
							{
							
								yPositive=true;
							}
							if(player->getPlayerObject()->getMscEquipped()->name.compare("Garlic")==0)
								movementSpeed--;
							pp->setVelocityX(calculateProjectileXMoveSpeed(x,y,botX,botY,movementSpeed));
							pp->setVelocityY(calculateProjectileXMoveSpeed(y,x,botY,botX,movementSpeed));
						
								if(yPositive&&xPositive)//player to bottom and right
								{
									if(deltax-deltay>=0)
									{
										moveInX=true;
									}
								}
								if(!yPositive)
								{
									if(xPositive)
									{
										if(deltax+deltay>0)
										{
											moveInX=true;
										}
									}
								}
								if(yPositive)
								{
									if(deltax+deltay<0)
									{
										moveInX=true;
									}
								}
						
							if(moveInX)
							{
								if(xPositive)
								{
									sprite->setCurrentState(3);
								}
								else
									sprite->setCurrentState(1);
							}
							else
							{
								if(yPositive)
									sprite->setCurrentState(0);
								else
									sprite->setCurrentState(2);
							}

						}
					
					
					}
					else
					{
					sprite->setCurrentState(0);
					pp->setVelocityX(0);
					pp->setVelocityY(0);
					}
				}
				
				else if ((distanceX < 250 && distanceY < 400) && isBoss)
				{

					//BASHEE SHRIEK MEMERIZE WALKING
					//if (player->getPlayerObject()->getMscEquipped()->name.compare("Headphones")!=0)
					if(false)
					{
							if(player->getPhysicalProperties()->getDirection() == 2)//facing right move left
							{
								player->setCurrentState(2);
								player->getPhysicalProperties()->setVelocityX(-2);
							}
							else if(player->getPhysicalProperties()->getDirection() == 0)//facing left move right
							{
								player->setCurrentState(6);
								player->getPhysicalProperties()->setVelocityX(2);
							}
							else if(player->getPhysicalProperties()->getDirection() == 1)//facing up move down
							{
								player->setCurrentState(4);
								player->getPhysicalProperties()->setVelocityY(2);
							}
							else if(player->getPhysicalProperties()->getDirection() == 3)//facing down move up
							{
								player->setCurrentState(0);
								player->getPhysicalProperties()->setVelocityY(-2);
							}
					}
					if (sprite->isOffCooldown())
					{
						//shoot screaming projectiles
						sprite->setCurrentState(0);
						XactSound *sound=game->getSound();
						sound->playSound("banshi2");
						AnimatedSprite *spriteToAdd = new AnimatedSprite();
						int index = 3;
						int initX = sprite->getPhysicalProperties()->getX();
						int initY = sprite->getPhysicalProperties()->getY() ;
						int accelerationX=0;
						int accelerationY=0;
						int spriteType =3;
						int alpha= 255;
						int currentState=0;
						AnimatedSpriteType *ast = game->getWorld()->getSpriteManager()->getSpriteType(index);
						PhysicalProperties *spriteProps = new PhysicalProperties();
						int velocityX=4;
						int velocityY=2;
						//int x=guessSpriteLocation(player,90,true);
						//int y=guessSpriteLocation(player,90,false);
						int x=player->getPhysicalProperties()->getX();
						int y=player->getPhysicalProperties()->getY();
						spriteProps->setVelocityX(calculateProjectileXMoveSpeed(x,y,initX,initY,velocityX));
						spriteProps->setVelocityY(calculateProjectileXMoveSpeed(y,x,initY,initX,velocityX));
						/*
						float distX = (pp->getX()+ sprite->getSpriteType()->getTextureWidth()/2) - (player->getPhysicalProperties()->getX() + player->getSpriteType()->getTextureWidth()/2);
						float distY = (pp->getY() + sprite->getSpriteType()->getTextureHeight()/2) - (player->getPhysicalProperties()->getY() + player->getSpriteType()->getTextureHeight()/2);
						float slope=0;
						if (distY!=0 && distX!=0)
						{
							slope= fabs(distX)/fabs(distY);
						}
						if (slope == 0)
						{
							if (distX=0 && distY!=0)
							{
								if (distY>0)
								{
									velocityY = -5;
									velocityX = 0;
								}
								else if(distY<0)
								{
									velocityY = 5;
									velocityX = 0;
								}
							}
							if (distY=0 && distX!=0)
							{
								if (distX>0)
								{
									velocityX=-5;
									velocityY=0;
								}
								else if (distX<0)
								{
									velocityX=5;
									velocityY=0;
								}
							}		
						}
						if (slope<.5)
						{
							if (distY>0)
							{
								velocityY = -5;
								velocityX = 0;
							}
							else if (distY<0)
							{
								velocityY = 5;
								velocityX = 0;
							}
						}
						if (slope>2.5)
						{
							if (distX>0)
							{
								velocityX = -5;
								velocityY = 0;
							}
							if (distX<0)
							{
								velocityX = 5;
								velocityY = 0;
							}
						}
						if (slope<2.5 && slope>.5)
						{
							if (distX>0 && distY>0)
							{
								velocityX = -3;
								velocityY = -3;
							}
							else if (distX<0 && distY>0)
							{
								velocityX = 3;
								velocityY = -3;
							}
							else if (distX>0 && distY<0)
							{
								velocityX = -3;
								velocityY = 3;
							}
							else if (distX<0 && distY<0)
							{
								velocityX = 3;
								velocityY = 3;
							}
						}
						*/
						spriteProps->setX(initX);
						spriteProps->setY(initY);
						spriteProps->setAccelerationX(accelerationX);
						spriteProps->setAccelerationY(accelerationY);
						//spriteProps->setVelocityX(velocityX);
						spriteProps->setInAir(false);
						//spriteProps->setVelocityY(velocityY);
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
						spriteToAdd->setSpriteType(ast);
						game->getWorld()->getSpriteManager()->addSprite(spriteToAdd);
						sprite->onCooldown();
						
					}
					sprite->updateCooldown();
				}
				if(instruction==9)
				{
					int bcounter=bot->getCount();
					if (bcounter>100)
					{
						sprite->setCurrentState(6);
						sprite->getPhysicalProperties()->setVelocityX(movementSpeed);
					}
					if(bcounter<=100)
					{
						sprite->setCurrentState(2);
						sprite->getPhysicalProperties()->setVelocityX(movementSpeed*-1);
					}
					if(bcounter>200)
					{
						bot->resetCount();
					}
				}
				//check distance to see if tracking AI or regular AI should be used
				else if (distanceX < 150 && distanceY < 200&&(instruction==5||isKnight))
				{
					int count=bot->getCount();
					int deltaX=player->getPhysicalProperties()->getX()-pp->getX();
					int deltaY=player->getPhysicalProperties()->getY()-pp->getY();
					bool yPositive=false;
					bool xPositive=false;
					int countLimit=100;
					if(deltaX>=0)
						xPositive=true;
					if(deltaY>=0)
						yPositive=true;

					if(count>countLimit)
						bot->resetCount();
					count=bot->getCount();
					//tracking AI
					if(count%30==0)
					{
						if ((deltaX>deltaY&&xPositive&&yPositive)||(deltaX+deltaY>0&&!yPositive))//more right than down or up
						{
						
								if ( !isBoss)
								{
									sprite->setCurrentState(6);
									pp->setVelocityX(movementSpeed*(1));
									pp->setVelocityY(0);
									XactSound *sound=game->getSound();
									if(count==0&&!isKnight)
									{
										sound->playSound("zombie");
									}

								}
							
								
						}
						else if((deltaX+deltaY>0&&yPositive&&!xPositive)||(deltaY>deltaX&&xPositive))//more down than left or right
						{
						
								if ( !isBoss)
								{
									sprite->setCurrentState(0);
									pp->setVelocityY(movementSpeed);
									pp->setVelocityX(0);
									XactSound *sound=game->getSound();
									if(count==0&&!isKnight)
									{
										sound->playSound("zombie");
									}
								}
							
						}
						else  if((deltaX+deltaY<0&&!xPositive&&yPositive)||(deltaX<deltaY&&!xPositive&&!yPositive))//more left than down or up
						{
							if ( !isBoss)
							{
								sprite->setCurrentState(2);
								pp->setVelocityY(0);
								pp->setVelocityX(movementSpeed*-1);
								XactSound *sound=game->getSound();
									if(count==0&&!isKnight)
									{
										sound->playSound("zombie");
									}
							}
						

						}
						else
						{
							sprite->setCurrentState(4);
							pp->setVelocityY(movementSpeed*-1);
							pp->setVelocityX(0);
							XactSound *sound=game->getSound();
							if(count==0&&!isKnight)
							{
								sound->playSound("zombie");
							}
						}
					}
				}
				
				//regular bot AI

				//ANDREW AI STARTS HERE
				
				else if( instruction==5)//zombie move
				{
					int moveInYAxix=rand()%10+1;
					int zcounter=counter = bot->getCount();
					int limit=bot->getCycle(0);
					if((pp->getVelocityX()==0&&pp->getVelocityY()==0)||zcounter%limit==0)
					{
						if(counter>=limit)
						{
							bot->resetCount();
						}
					
						int velocity=0;
						bool moveUpOrDown=false;
						bool movePositive=false;
						if(moveInYAxix>=5)
						{
							moveUpOrDown=true;
						}
						if(moveInYAxix%2==0)
						{
							movePositive=true;
						}
						if(moveUpOrDown&&movePositive)
						{
							sprite->setCurrentState(4);
							pp->setVelocityY(movementSpeed*-1);
							pp->setVelocityX(0);
						}
						else if(moveUpOrDown)
						{
						
							sprite->setCurrentState(0);
							pp->setVelocityY(movementSpeed);
							pp->setVelocityX(0);
						}
						else if(movePositive)
						{
							sprite->setCurrentState(6);
							pp->setVelocityX(movementSpeed);
							pp->setVelocityY(0);
						}
						else
						{
							sprite->setCurrentState(2);
							pp->setVelocityX(movementSpeed*-1);
							pp->setVelocityY(0);
						}
					}
				}
				else if(instruction<=4&& game->knightsAreAwake())
				{
					switch(instruction)
					{
					case 1:
						if (state!=0)
						{
							sprite->setCurrentState(0);
							pp->setVelocityY(movementSpeed);
							pp->setVelocityX(0);
						}
						break;
					case 2:
						if (state!=2)
						{
							sprite->setCurrentState(2);
							pp->setVelocityX(movementSpeed*(-1));
							pp->setVelocityY(0);
						}
						break;
					case 3:
						if (state!=4)
						{
							sprite->setCurrentState(4);
							pp->setVelocityY(movementSpeed*(-1));
							pp->setVelocityX(0);
						}
						break;
					case 4:
						if (state!=6)
						{
							sprite->setCurrentState(6);
							pp->setVelocityX(movementSpeed);
							pp->setVelocityY(0);
						}
						break;
					}
					bot->incrementCount();
					counter = bot->getCount();
					cycleLimit = bot->getCycle(instructionNumber);
					if (counter>=cycleLimit)
					{
						bot->incrementInstructionNumber();
						if (bot->getInstructionNumber() >= bot->getInstructionSize())
						{
							bot->resetInstructionNumber();
						}
						bot->resetCount();
					}
				}
					bot->incrementCount();
			}

				//END ANDREW AI
				
			}

			if (!sprite->isOffCooldown())
			{
				sprite->updateCooldown();
			}
	}



	
	double GameAI::calculateProjectileXMoveSpeed(int targetX,int targetY,int shooterX, int shooterY, double projectileMoveSpeed)
	{
		
		int deltaX=targetX-shooterX;
		int deltaY=targetY-shooterY;
		double totalDistance=(deltaX*deltaX)+(deltaY*deltaY);
		totalDistance=std::sqrt(totalDistance);
		double time=totalDistance/projectileMoveSpeed;
		double xmoveSpeed=deltaX/time;
		return xmoveSpeed;
	}
	double GameAI::guessSpriteLocation(AnimatedSprite* sprite,int numOfFramesIntoFuture,bool Xlocation)
	{
		PhysicalProperties *pp=sprite->getPhysicalProperties();
		int spriteVelocity;
		
		int spriteLocation;
		if(Xlocation)
		{
			spriteVelocity=pp->getVelocityX();
			spriteLocation=pp->getX();
		}
		
		if(!Xlocation)
		{
			spriteLocation=pp->getY();
			spriteVelocity=pp->getVelocityY();
		}
		
		spriteLocation=(spriteVelocity*numOfFramesIntoFuture)+spriteLocation;
		return spriteLocation;
	}