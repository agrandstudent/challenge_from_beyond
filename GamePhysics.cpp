#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "BoundingVolume.h"
#include "CollidableObject.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GamePhysics.h"
#include "GameWorld.h"
#include "PhysicalProperties.h"
#include "TiledLayer.h"
#include "TextFileReader.h"
#include "SparseLayer.h"
#include "SpriteManager.h"
#include "Viewport.h"
#include "WorldLayer.h"
#include <vector>
#include "PlayerStatsGui.h"
#include "OverlayImage.h"
#include "XactSound.h"
#include "Player.h"


PlayerStatsGui *playerStats;
Game *game;
PCSTR cue;
	GamePhysics::GamePhysics()
	{
		inAir = false;
		isReusableEmpty = true;
		isReusableCollisionsEmpty = true;
		isTimerOn = false;
		isInvincibleOn = false;
		isWallSoundTimerOn=false;
		currentReusable=0;
		minColumn = 0;
		maxColumn = 0;
		minRow = 0;
		maxRow = 0;
		knockBackTimer = 0;
		invincibleTimer = 0;
		wallSoundTimer=0;
	    BVtoAdd;
		PPtoAdd;
		objectToAdd;
		collisionToAdd;
		collidableObjects = new vector<CollidableObject*>();
		reusableCollidableObjects = new vector<CollidableObject*>();
		reusableCollisionObjects = new vector<CollisionObject*>();
		collisionObjects = new vector<CollisionObject*>();
		collidableTiles = new vector<CollidableObject*>();
		tilesToCheck = new vector<Tile*>;
		
		cues = new vector<PCSTR>;
		
		
		//sprite stuff
		spriteCollidableObjects = new vector<CollidableObject*>();
		spriteCollisionObjects = new vector<CollisionObject*>();
		spriteTilesToCheck = new vector<Tile*>;

	}

	GamePhysics::~GamePhysics(){}

	void GamePhysics::update(Game *game) 
	{
		if(isWallSoundTimerOn)
		{
			wallSoundTimer++;
			if(wallSoundTimer>=30)
			{
				
				isWallSoundTimerOn=false;
				wallSoundTimer=0;
			}
		}
		AnimatedSprite *player = game->getWorld()->getSpriteManager()->getPlayer();
		AnimatedSprite *goonie1 = game->getWorld()->getSpriteManager()->getSprite(0);
		PhysicalProperties *playerProps = player->getPhysicalProperties();

		tiledLayer = game->getWorld()->getTiledLayer();//GET tiledlayer


		//checkOutOfScreen(game);

		//ENFORCE GRAVITY
		//checkInAir(player);

		//if(playerProps->getInAir() == true)
		//{
		//playerProps->incVelocityY(1);
		//goonie1->getPhysicalProperties()->incVelocityY(.5);
		//}

		//clear all vectors and free memory
		collidableObjects->clear();
		collisionObjects->clear();
		tilesToCheck->clear();
		collidableTiles->clear();

		spriteCollidableObjects->clear();
		spriteCollisionObjects->clear();
		spriteTilesToCheck->clear();
		cues->clear();

		createCollidableObjects(); // create reusable objects
		createCollisionObjects(); // create reusable collision objects

		getOccupiedTiles(player, game->getWorld());
		getTilesToCheck(game->getWorld());
		checkSpriteCollisions(game);

		//check sprite to tile collisions
		checkSpriteToTileCollisions(game->getWorld());

		checkPlayerHP(game);

		//if(player->getCurrentState() == 7)
		//	game->changeGameState(6);//gameover

		addCollisionObjects(game->getWorld());
		findCollisionTimes(collisionObjects);
		sortCollisionObjects();
		resolveCollisions(game->getWorld(), collisionObjects, game);
		//if(collisionObjects->size() == 0)
		//	playerProps->setInAir(true);

		//clear memory used
		//delete BVtoAdd;
		//delete objectToAdd;
	}



	void GamePhysics::checkPlayerHP(Game *game)
	{
		if(game->getWorld()->getSpriteManager()->getPlayer()->getPlayerObject()->getHP() <= 0)
		{
			/*game->getWorld()->getSpriteManager()->getPlayer()->setCurrentState(12);
			if(game->getWorld()->getSpriteManager()->getPlayer()->getFrameIndex() == 5)
			{
				game->getWorld()->getSpriteManager()->getPlayer()->setCurrentState(13);
			}*/
			game->getWorld()->getSpriteManager()->getPlayer()->setCurrentState(13);
			game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties()->setCollidable(false);
			game->setPlayerDead(true);
			game->getWorld()->getSpriteManager()->getPlayer()->getPlayerObject()->setHP(1000);
		}
		
	}


	void GamePhysics::checkOutOfScreen(Game *game)
	{
		GameWorld *world = game->getWorld();
		AnimatedSprite *player = world->getSpriteManager()->getPlayer();
		PhysicalProperties *pp = player->getPhysicalProperties();

		if((pp->getY()+pp->getVelocityY()+player->getSpriteType()->getTextureHeight()) > world->getWorldHeight() ||
			(pp->getY() + player->getSpriteType()->getTextureHeight() + pp->getVelocityY()) < 100 || (pp->getX() + player->getSpriteType()->getTextureWidth() + pp->getVelocityX()) > game->getWorld()->getWorldWidth() )
		{
			pp->setX(1);
			pp->setY(1);
			game->changeGameState(6);
		}

	}


	void GamePhysics::checkInAir(AnimatedSprite *player)
	{
		int column = player->getPhysicalProperties()->getX()/64;//64
		int row = ((player->getPhysicalProperties()->getY() + player->getSpriteType()->getTextureWidth())/64) + 1;

		if(tiledLayer->getTile(row, column)->collidable == false)
			player->getPhysicalProperties()->setInAir(true);
	}

	void GamePhysics::createCollidableObjects()
	{
		currentReusable = 0;
		
		if(isReusableEmpty == true)
		{
			//reuse these collidable objects to avoid memory leaks
			for(int i = 0; i < 30; i++)
			{
				CollidableObject *objectToAdd = new CollidableObject();
				BoundingVolume *BVtoAdd = new BoundingVolume();
				PhysicalProperties *PPtoAdd = new PhysicalProperties();

				objectToAdd->setBoundingVolume(BVtoAdd);
				objectToAdd->setPhysicalProperties(PPtoAdd);

				reusableCollidableObjects->push_back(objectToAdd);
			}

			isReusableEmpty = false;
		}
		
	}

	void GamePhysics::createCollisionObjects()
	{
		currentReusableCollision = 0;
		
		if(isReusableCollisionsEmpty == true)
		{
			//reuse these collision objects to avoid memory leaks
			for(int i = 0; i < 20; i++)
			{
				CollidableObject *cb1 = new CollidableObject();
				CollidableObject *cb2 = new CollidableObject();
				CollisionObject *collisionToAdd = new CollisionObject();
				collisionToAdd->setCb1(cb1);
				collisionToAdd->setCb2(cb2);
				reusableCollisionObjects->push_back(collisionToAdd);
			}

			isReusableCollisionsEmpty = false;
		}
		
	}

	void GamePhysics::getOccupiedTiles(AnimatedSprite *sprite, GameWorld *world)
	{
		minColumn = sprite->getPhysicalProperties()->getX()/64;//64
		if((sprite->getPhysicalProperties()->getX()/64) - 1 >= 0)
			minColumn =(sprite->getPhysicalProperties()->getX()/64) - 1;

		maxColumn = ((sprite->getPhysicalProperties()->getX() + sprite->getSpriteType()->getTextureWidth())/64);
		if((sprite->getPhysicalProperties()->getX()/64) + 38 <= world->getWorldWidth())
			maxColumn =(sprite->getPhysicalProperties()->getX()/64) + 1;

		minRow = sprite->getPhysicalProperties()->getY()/64;
		if((sprite->getPhysicalProperties()->getY()/64) - 1 >= 0)
			minRow =(sprite->getPhysicalProperties()->getY()/64) - 1;

		maxRow = ((sprite->getPhysicalProperties()->getY() + sprite->getSpriteType()->getTextureHeight())/64);
		if((sprite->getPhysicalProperties()->getY()/64) + 52 <= world->getWorldHeight())
			maxRow =(sprite->getPhysicalProperties()->getY()/64) + 1;
	}


	void GamePhysics::getTilesToCheck(GameWorld *world)
	{

	
		//while (minColumn <= maxColumn)
		int i = minRow;
		int j = minColumn;

		while(i <= maxRow)
		{
			//int j = minColumn;
			//int minRowOld = minRow;
			//while(minRow <= maxRow)
			while (j <= maxColumn)
			{
				if(tiledLayer->getTile(i, j)->collidable == true)
				{
					AnimatedSprite *player = world->getSpriteManager()->getPlayer();
					PhysicalProperties *playerProps = player->getPhysicalProperties();
					//check if the tile will collide next frame, edit this later for dynamic objects by accounting for velocity of object2
					if(willCollide(playerProps->getX(), playerProps->getY(), playerProps->getVelocityX(), playerProps->getVelocityY(), world->getSpriteManager()->getSpriteType(0)->getTextureWidth(), world->getSpriteManager()->getSpriteType(0)->getTextureHeight(),
								(j * 64)   , (i* 64)		, 0							 , 0						  ,    64														  ,		64															))
					{
						addCollidableObjects(tiledLayer->getTile(i, j), (j * 64), (i* 64), collidableObjects); //find x, y by mult row by tilesize. Add to collidableObjects vector
					}
					//tilesToCheck->push_back(tiledLayer->getTile(minRow, minColumn));	//add tile to vector
				}
				if(tiledLayer->getTile(i, j)->collidable == true)
					addCollidableObjects(tiledLayer->getTile(i, j), (j * 64), (i* 64), collidableTiles);
				tilesToCheck->push_back(tiledLayer->getTile(i, j));
				//minRow++;
				j++;
			}
			//minColumn++;
			i++;
			//minRow = minRowOld;
			j = minColumn;
		}
	
		//vector<WorldLayer*> *layers = world->getLayers();
		//WorldLayer worldLayer = layers[0];
		//TiledLayer *tilerLayer = dynamic_cast<TiledLayer*> (layers(0));


	}

	void GamePhysics::addCollidableObjects(Tile *tileToAdd, int x, int y, vector<CollidableObject*> *cObjects)
	{
		/*for(int i=0; i < tilesToCheck->size(); i++)
		{
			BoundingVolume *BVtoAdd = new BoundingVolume();
			BVtoAdd->setX();

			tilesToCheck->at(i);


			//BVtoAdd->setX(tilesToCheck(i));
			CollidableObject *objectToAdd = new CollidableObject();
			objectToAdd->setBoundingVolume();

		}*/
		objectToAdd = reusableCollidableObjects->at(currentReusable);
		objectToAdd->getBoundingVolume()->setX(x);
		objectToAdd->getBoundingVolume()->setY(y);
		objectToAdd->getBoundingVolume()->setHeight(64);
		objectToAdd->getBoundingVolume()->setWidth(64);
		/*BVtoAdd = reusableCollidableObjects->at(currentReusable)->getBoundingVolume();

		BVtoAdd->setX(x);  //set x,y coordinates of tile into Bounding Volume
		BVtoAdd->setY(y);
		
		
		BVtoAdd->setHeight(64);	//for tiles only
		BVtoAdd->setWidth(64);
		


		objectToAdd = reusableCollidableObjects->at(currentReusable);
		objectToAdd->setBoundingVolume(BVtoAdd);	//set Bouding Volume of Collidable Object
		*/
		cObjects->push_back(objectToAdd);

		currentReusable++;

	}

	bool GamePhysics::willCollide(float x1, float y1, float x1Velocity, float y1Velocity, float width1, float height1, 
								  float x2, float y2, float x2Velocity, float y2Velocity, float width2, float height2)
	{
		//DO THE AXIOM AXIS BOUNDING BOX TO CHECK IF THEY ARE COLLIDING

		if(((x1 + x1Velocity) < (x2 + width2 + x2Velocity))	  &&	//LEFT SIDE < RIGHT SIDE
		   ((x1 + x1Velocity + width1) > (x2 + x2Velocity))   &&	//RIGHT SIDE > LEFT SIDE
		   ((y1 + y1Velocity) < (y2 + height2 + y2Velocity))  &&	//TOP SIDE < BOTTOM SIDE
		   ((y1 + y1Velocity + height1) > (y2 + y2Velocity)))		//BOTTOM SIDE > TOP SIDE
		{
			return true;
		}

		return false;
	}

	 
	void GamePhysics::addCollisionObjects(GameWorld *world)	//this method takes collidable object pairs and creats collision objects
	{
		for(int i=0; i < collidableObjects->size(); i++)//checking for collision with player to tiles only, no sprites yet
		{
			//change player into collidableobject
			AnimatedSprite *player = world->getSpriteManager()->getPlayer();
			PhysicalProperties *playerProps = world->getSpriteManager()->getPlayer()->getPhysicalProperties();
			BVtoAdd = reusableCollidableObjects->at(currentReusable)->getBoundingVolume();
			BVtoAdd->setHeight(player->getSpriteType()->getTextureHeight());
			BVtoAdd->setWidth(player->getSpriteType()->getTextureWidth());

			objectToAdd = reusableCollidableObjects->at(currentReusable);
			objectToAdd->setPhysicalProperties(playerProps);
			objectToAdd->setBoundingVolume(BVtoAdd);

			//collision between player and tile
			collisionToAdd = reusableCollisionObjects->at(currentReusableCollision);
			collisionToAdd->setCb1(objectToAdd);
			collisionToAdd->setCb2(collidableObjects->at(i));
			collisionObjects->push_back(collisionToAdd);
			currentReusableCollision++;
			currentReusable++;
		}
	}


	float distanceX;
	float distanceY;
	float velocityX;
	float velocityY;
	float timeX;
	float timeY;
	CollisionObject *collision;
	CollidableObject *co1;
	CollidableObject *co2;
	PhysicalProperties *pp1;
	PhysicalProperties *pp2;
	BoundingVolume *bv1;
	BoundingVolume *bv2;

	void GamePhysics::findCollisionTimes(vector<CollisionObject*> *objectsToCheck)
	{
		if(objectsToCheck->size() != 0)
		{										
			for(int i=0; i < objectsToCheck->size(); i++)	//only for player to tiles because tiles only has Bounding Volume
			{
				collision = objectsToCheck->at(i);
				co1 = collision->getCb1();
				co2 = collision->getCb2();
				pp1 = co1->getPhysicalProperties();
				bv1 = co1->getBoundingVolume();
				bv2 = co2->getBoundingVolume();
				
				

				if(pp1->getVelocityX() < 0)
					distanceX = fabs((pp1->getX()) - (bv2->getX()+64));
				if(pp1->getVelocityX() > 0)
					distanceX = fabs((pp1->getX() + bv1->getWidth()) - bv2->getX());// + width of player sprite

				if(pp1->getVelocityY() < 0)
					distanceY = fabs(pp1->getY() - (bv2->getY()+64));
				if(pp1->getVelocityY() > 0)
					distanceY = fabs((pp1->getY() + bv1->getHeight()) - bv2->getY());// + height of player sprite

				velocityX = fabs((pp1->getVelocityX()));
				velocityY = fabs((pp1->getVelocityY()));

				if(velocityX != 0)
				{
					timeX = (distanceX/velocityX);
				}
				else if (velocityX == 0 && distanceX == 0) { timeX = -1; }
				if(velocityY != 0)
				{
					timeY = (distanceY/velocityY);
				}
				else if (velocityY == 0) { timeY = -1; }


			
				if(timeX == -1 && timeY == -1)
				{
					objectsToCheck->at(i)->setFirstTimeOfCollision(1000);
					objectsToCheck->at(i)->setDirection(-1);
				}
				if(timeX < timeY && timeX != -1 && timeY != -1)	//whichever time is smaller, set that as first time of collision
				{
					objectsToCheck->at(i)->setFirstTimeOfCollision(timeX);
					objectsToCheck->at(i)->setDirection(0);
				}
				else if(timeX > timeY && timeX != -1 && timeY != -1)
				{
					objectsToCheck->at(i)->setFirstTimeOfCollision(timeY);
					objectsToCheck->at(i)->setDirection(1);
				}
				else if(timeX != -1 && timeY == -1)
				{
					objectsToCheck->at(i)->setFirstTimeOfCollision(timeX);
					objectsToCheck->at(i)->setDirection(0);
				}
				else if(timeY != -1 && timeX == -1)
				{
					objectsToCheck->at(i)->setFirstTimeOfCollision(timeY);
					objectsToCheck->at(i)->setDirection(1);
				}
				
			}
			


		}
	}

	void GamePhysics::sortCollisionObjects()
	{
		float time1;
		float time2;

		if(collisionObjects->size() != 0)
		{
			//SORTING USING BUBBLE SORTTT!!!
			/*for(int i=0; i < collisionObjects->size(); i++)
			{
				for(int j = 0; j < collisionObjects->size()-1-i; j++)
				{


					time1 = collisionObjects->at(j)->getFirstTimeOfCollision();
					time2 = collisionObjects->at(j+1)->getFirstTimeOfCollision();

					if(time1 > time2)
					{
						
						collisionObjects[j].swap(collisionObjects[j+1]);
					}

				}
			}*/
			int n = collisionObjects->size();
			for(int i=0; i < n; i++)
			{
				for(int j=n-1; j > i; j--)
				{
					time1 = collisionObjects->at(j)->getFirstTimeOfCollision();
					time2 = collisionObjects->at(j-1)->getFirstTimeOfCollision();
					if(time1 < time2)
					{
						CollisionObject *temp = collisionObjects->at(j-1);
						collisionObjects->at(j-1) = collisionObjects->at(j);
						collisionObjects->at(j) = temp;
						//collisionObjects[j-1].swap(collisionObjects[j]);
					}
				}
			}

		}
	}

	float distanceX1;
	float distanceY1;
	float distanceX2;
	float distanceY2;
	float moveToX;
	float moveToY;

	void GamePhysics::resolveCollisions(GameWorld *world, vector<CollisionObject*> *objectsToCheck, Game *game)
	{
		

		if(objectsToCheck->size() != 0)
		{
			//for(int i =0;i < objectsToCheck->size(); i++)
			//{
				int i = 0;
				co1 = objectsToCheck->at(i)->getCb1();
				co2 = objectsToCheck->at(i)->getCb2();
				

				//moveToX = co1->getPhysicalProperties()->getVelocityX() * (collisionObjects->at(i)->getFirstTimeOfCollision()/100);
				//moveToY = co1->getPhysicalProperties()->getVelocityY() * (collisionObjects->at(i)->getFirstTimeOfCollision()/100);

				//co1->getPhysicalProperties()->setX(co1->getPhysicalProperties()->getX() + moveToX);
				//co1->getPhysicalProperties()->setY(co1->getPhysicalProperties()->getY() + moveToY);

				//TIME COLLISION RESPONSE
				
				if(objectsToCheck->at(i)->getDirection() == 0) //moving X
				{
					if(co1->getPhysicalProperties()->getVelocityX() > 0) //moving right
						co1->getPhysicalProperties()->setX(co2->getBoundingVolume()->getX() - co1->getBoundingVolume()->getWidth()); // - width of player sprite
					if(co1->getPhysicalProperties()->getVelocityX() < 0) //moving left
						co1->getPhysicalProperties()->setX(co2->getBoundingVolume()->getX()+64);
					co1->getPhysicalProperties()->setVelocityX(0);
					if(!isWallSoundTimerOn)
					{
						if(co1->getBoundingVolume()->getWidth() == 38)//make sure its player sprite
						{
							/*cue="wall collide";
							XactSound *sound=game->getSound();
							sound->playSound(cue);
							isWallSoundTimerOn=true;*/
						}
					}
				}
				if(objectsToCheck->at(i)->getDirection() == 1)//move Y
				{
					if(co1->getPhysicalProperties()->getVelocityY() < 0) //moving up
						co1->getPhysicalProperties()->setY(co2->getBoundingVolume()->getY()+64);
					if(co1->getPhysicalProperties()->getVelocityY() > 0) //moving down
						co1->getPhysicalProperties()->setY(co2->getBoundingVolume()->getY() - co1->getBoundingVolume()->getHeight()); // - height of player sprite
					co1->getPhysicalProperties()->setVelocityY(0);
					co1->getPhysicalProperties()->setInAir(false);
					if(!isWallSoundTimerOn)
					{
						if(co1->getBoundingVolume()->getWidth() == 38)//make sure its player sprite
						{
							/*cue="wall collide";
							XactSound *sound=game->getSound();
							sound->playSound(cue);
							isWallSoundTimerOn=true;*/
						}
					}
				}

				/*if(collisionObjects->at(i)->getAnimatedSprite() != 0)//sprite to sprite collision
				{
					AnimatedSprite *sprite = collisionObjects->at(i)->getAnimatedSprite();
					PhysicalProperties *spriteProps = collisionObjects->at(i)->getCb2()->getPhysicalProperties();
					if(spriteProps->isCollidable() == true)
					{
						sprite->setCurrentState(6); //death left
						spriteProps->setVelocityX(0);
						spriteProps->setCollidable(false);
					}
				}*/
					//NEWEST COLLISION

					/*co1->getPhysicalProperties()->incX(co1->getPhysicalProperties()->getVelocityX() * (collisionObjects->at(i)->getFirstTimeOfCollision())-1);
					if(collisionObjects->at(i)->getDirection() == 0)
						co1->getPhysicalProperties()->setVelocityX(0);
		
					//co1->getPhysicalProperties()->incY(co1->getPhysicalProperties()->getVelocityY() * (collisionObjects->at(i)->getFirstTimeOfCollision())-1);
					if(collisionObjects->at(i)->getDirection() == 1)//move Y
					{
							co1->getPhysicalProperties()->setVelocityY(0);
							co1->getPhysicalProperties()->setY(co2->getBoundingVolume()->getY()-150-1);
					}
					co1->getPhysicalProperties()->setInAir(false);*/



				
				/*if(co1->getPhysicalProperties()->getVelocityX() != 0)
					co1->getPhysicalProperties()->setVelocityX(0);	
				if(co1->getPhysicalProperties()->getVelocityY() != 0)
					co1->getPhysicalProperties()->setVelocityY(0);	
				//co1->getPhysicalProperties()->setVelocityX(0);

				//AIR RESPOSNSE
				/*if((co1->getPhysicalProperties()->getX() + co1->getPhysicalProperties()->getVelocityX()) < (co2->getBoundingVolume()->getX() + co2->getBoundingVolume()->getWidth()) && co1->getPhysicalProperties()->getVelocityX() < 0 && co1->getPhysicalProperties()->getInAir() == false)
				{
					co1->getPhysicalProperties()->setX(co2->getBoundingVolume()->getX() + co2->getBoundingVolume()->getWidth() + 100);
					co1->getPhysicalProperties()->setVelocityX(0);
				}
				if((co1->getPhysicalProperties()->getX()+co1->getPhysicalProperties()->getVelocityX()+co1->getBoundingVolume()->getWidth()) > co2->getBoundingVolume()->getX() && co1->getPhysicalProperties()->getVelocityX() > 0 && co1->getPhysicalProperties()->getInAir() == false)
				{
					co1->getPhysicalProperties()->setX(co2->getBoundingVolume()->getX() - 100);
					co1->getPhysicalProperties()->setVelocityX(0);
				}
				if((co1->getPhysicalProperties()->getY() + co1->getPhysicalProperties()->getVelocityY()) < (co2->getBoundingVolume()->getY() + co2->getBoundingVolume()->getHeight()) && co1->getPhysicalProperties()->getVelocityY() < 0)
				{
					co1->getPhysicalProperties()->setY(co2->getBoundingVolume()->getY() + co2->getBoundingVolume()->getHeight());
					co1->getPhysicalProperties()->setVelocityY(0);
				}
				if((co1->getPhysicalProperties()->getY() + co1->getPhysicalProperties()->getVelocityY() + co1->getBoundingVolume()->getHeight()) > (co2->getBoundingVolume()->getY()) && co1->getPhysicalProperties()->getVelocityY() > 0)
				{
					co1->getPhysicalProperties()->setY(co2->getBoundingVolume()->getY() - 150);
					co1->getPhysicalProperties()->setVelocityY(0);
					co1->getPhysicalProperties()->setVelocityX(0);
					co1->getPhysicalProperties()->setInAir(false);

				}

				/*if(((x1 + x1Velocity) < (x2 + width2 + x2Velocity))	  &&	//LEFT SIDE < RIGHT SIDE
				 ((x1 + x1Velocity + width1) > (x2 + x2Velocity))   &&	//RIGHT SIDE > LEFT SIDE
				 ((y1 + y1Velocity) < (y2 + height2 + y2Velocity))  &&	//TOP SIDE < BOTTOM SIDE
				 ((y1 + y1Velocity + height1) > (y2 + y2Velocity)))*/
			//}
		}
		//co1->getPhysicalProperties()->setInAir(true);
		if(cues->size()!=0)
		{
			cue=cues->back();
			//XactSound *sound=game->getSound();
			//sound->playSound(cue);
		}
	}


	void GamePhysics::checkSpriteCollisions(Game *game)
	{
		GameWorld *world = game->getWorld();
		AnimatedSprite *player = world->getSpriteManager()->getPlayer();
		PhysicalProperties *playerProps = player->getPhysicalProperties();
		SpriteManager *spriteManager = world->getSpriteManager();
		AnimatedSprite *sprite;
		PhysicalProperties *spriteProps;
		bool doorChecked = false;

		if(isInvincibleOn)
		{
			if((invincibleTimer % 2) == 0 && isInvincibleOn == true)	//flash invincibility
					{
						if(player->getAlpha() == 140)
						{
							player->setAlpha(70);
						}
						else
						{
							player->setAlpha(140);
						}
					}
		}



		for(int i=0; i < world->getSpriteManager()->getNumberOfSprites();i++)
		{
			sprite = spriteManager->getSprite(i);
			spriteProps = sprite->getPhysicalProperties();
			if(sprite->getSpriteType()->getSpriteTypeID() != 0)
			{
				if(willCollide(playerProps->getX(), playerProps->getY(), playerProps->getVelocityX(), playerProps->getVelocityY(), player->getSpriteType()->getTextureWidth(), player->getSpriteType()->getTextureHeight(), 
					(spriteProps->getX()), spriteProps->getY(), spriteProps->getVelocityX(), spriteProps->getVelocityY(), sprite->getSpriteType()->getTextureWidth(), sprite->getSpriteType()->getTextureWidth()))
				{

					if(playerProps->isCollidable() && sprite->isCurrentlyCollidable() == true)
					{
						resolveSpriteCollisions(player, sprite);
						if (sprite->getSpriteType()->getSpriteTypeID()==4 && !game->knightsAreAwake())
							{
								break;
							}
						player->getPlayerObject()->decreaseHP();
						if (sprite->getSpriteType()->getSpriteTypeID()==3)
						{
							sprite->setAlpha(0);
						}
							cue="ouch";
							XactSound *sound=game->getSound();
							sound->playSound(cue);
							
						
						cues->push_back(cue);
					}
						

					/*if(spriteProps->isCollidable())
					{
							Player *playerObject = player->getPlayerObject();
							playerObject->decreaseHP();
							player->getPhysicalProperties()->setVelocityX(0);
							player->getPhysicalProperties()->setVelocityY(0);
						
						/*if(sprite->getCurrentState() != 4)
							sprite->setCurrentState(4);
							sprite->getPhysicalProperties()->setVelocityX(0);
						if(player->getCurrentState() == 3)
						{
							sprite->setCurrentState(6); //death left
							spriteProps->setVelocityX(0);
							spriteProps->setCollidable(false);
						}
						else
						{
							if(sprite->getFrameIndex() == 5 && sprite->getCurrentState() == 4)
								player->setCurrentState(7); //player is dead
						}
						//->getPhysicalProperties()->setVelocityX(0);
						//sprite->getPhysicalProperties()->setVelocityY(0);
					}*/

					//TRIED TO PUT INTO COLLISION VECTOR
					/*//change player into collidableobject
					AnimatedSprite *player = world->getSpriteManager()->getPlayer();
					PhysicalProperties *playerProps = world->getSpriteManager()->getPlayer()->getPhysicalProperties();
					BVtoAdd = reusableCollidableObjects->at(currentReusable)->getBoundingVolume();
					BVtoAdd->setHeight(player->getSpriteType()->getTextureHeight());
					BVtoAdd->setWidth(player->getSpriteType()->getTextureWidth());

					objectToAdd = reusableCollidableObjects->at(currentReusable);
					objectToAdd->setPhysicalProperties(playerProps);
					objectToAdd->setBoundingVolume(BVtoAdd);
					currentReusable++;

					//collision between player and sprite
					collisionToAdd = reusableCollisionObjects->at(currentReusableCollision);
					collisionToAdd->setCb1(objectToAdd);

					BVtoAdd = reusableCollidableObjects->at(currentReusable)->getBoundingVolume();
					BVtoAdd->setHeight(sprite->getSpriteType()->getTextureHeight());
					BVtoAdd->setWidth(sprite->getSpriteType()->getTextureWidth());
				
					PPtoAdd = reusableCollidableObjects->at(currentReusable)->getPhysicalProperties();
					PPtoAdd->setX(spriteProps->getX());
					PPtoAdd->setY(spriteProps->getY());
					PPtoAdd->setVelocityX(spriteProps->getVelocityX());
					PPtoAdd->setVelocityY(spriteProps->getVelocityY());

					objectToAdd = reusableCollidableObjects->at(currentReusable);
					objectToAdd->setPhysicalProperties(PPtoAdd);
					objectToAdd->setBoundingVolume(BVtoAdd);
					currentReusable++;

					//add to collision
					collisionToAdd->setCb2(collidableObjects->at(i));
					collisionToAdd->setDirection(-2);//means it is a sprite to sprite collision
					collisionToAdd->setAnimatedSprite(sprite);
					collisionObjects->push_back(collisionToAdd);
					currentReusableCollision++;*/
				
				}

				if(isInvincibleOn == true)
				{
					if(invincibleTimer > 50)  //invincibility lasts 30 frames
					{
						invincibleTimer = 0;
						isInvincibleOn = false;
						player->setAlpha(255);
						if(game->getPlayerDead() == false)
						{
							playerProps->setCollidable(true);
						}
					}

				}


			}

		}
		
		if(isInvincibleOn)
		{
			invincibleTimer++;
		}

		
		OverlayImage *sparseTilesToCheck;

		for(int i=0; i < world->getSparseLayer()->getNumberOfSparseTiles();i++)
		{
			sparseTilesToCheck = world->getSparseLayer()->getTile(i);
			if(willCollide(playerProps->getX(), playerProps->getY(), playerProps->getVelocityX(), playerProps->getVelocityY(), player->getSpriteType()->getTextureWidth(), player->getSpriteType()->getTextureHeight(), 
				sparseTilesToCheck->x, sparseTilesToCheck->y, 0, 0, sparseTilesToCheck->width, sparseTilesToCheck->height))
			{

				int currentLevel = world->getCurrentLevel();
				
				if(sparseTilesToCheck->name == "doorToGarden")
				{
					world->setCurrentArea(2);
					world->setFromArea(1);
					world->loadCurrentLevel(game, currentLevel);
					return;

				}
				if(sparseTilesToCheck->name == "doorToBasement")
				{
					world->setCurrentArea(3);
					world->setFromArea(1);
					world->loadCurrentLevel(game, currentLevel);
					return;

				}
				if(sparseTilesToCheck->name == "doorToFirstFloorFromBasement")
				{
					world->setCurrentArea(1);
					world->setFromArea(3);
					world->loadCurrentLevel(game, currentLevel);
					return;

				}
				if(sparseTilesToCheck->name == "doorToFirstFloorFromGarden")
				{
					world->setCurrentArea(1);
					world->setFromArea(2);
					world->loadCurrentLevel(game, currentLevel);
					return;

				}
				if(sparseTilesToCheck->name == "doorToSword" && !doorChecked)
				{
					if(player->getPlayerObject()->getMscEquipped()->name == "Key")
					{
						sparseTilesToCheck->isCollidable = false;
						sparseTilesToCheck->alpha = 0;
						return;
					}
					else
					{
						game->changeGameState(CHECK_GAME_STATE);
						game->setPaused(true);
						game->getText()->clearText();
						string s = "The door is locked...";
						wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
						game->getText()->addText(descriptor, 410,140,1000,1000);

						player->getPhysicalProperties()->setVelocityX(0);
						player->getPhysicalProperties()->setVelocityY(0);
						player->getPhysicalProperties()->setX(player->getPhysicalProperties()->getX()-2);
						doorChecked = true;

						return;
					}

				}
				else
				{
					if(sparseTilesToCheck->isCollidable == true)
					{
						

						/*if(playerProps->getVelocityX() > 0)
							playerProps->setX(playerProps->getX()-1);
						if(playerProps->getVelocityX() < 0)
							playerProps->setX(playerProps->getX()+1);
						if(playerProps->getVelocityY() > 0)
							playerProps->setY(playerProps->getY()-1);
						if(playerProps->getVelocityY() < 0)
							playerProps->setY(playerProps->getY()+1);*/

						player->getPhysicalProperties()->setVelocityX(0);
						player->getPhysicalProperties()->setVelocityY(0);
						//add sound code here
						if(!isWallSoundTimerOn)
						{
							/*cue="wall collide";
							XactSound *sound=game->getSound();
							sound->playSound(cue);
							isWallSoundTimerOn=true;*/
						}
						//here 

						//here
					}
					
				}
			}
		}
				
	}

	void GamePhysics::resolveSpriteCollisions(AnimatedSprite *player, AnimatedSprite *sprite)
	{
		PhysicalProperties *playerProps  = player->getPhysicalProperties();
		PhysicalProperties *spriteProps = sprite->getPhysicalProperties();
		/*float x1 = playerProps->getX();
		float y1 = playerProps->getY();
		float x1Velocity = playerProps->getVelocityX();
		float y1Velocity = playerProps->getVelocityY();
		float width1 = player->getSpriteType()->getTextureWidth();
		float height1 = player->getSpriteType()->getTextureHeight();
		float x2 = spriteProps->getX();
		float y2 = spriteProps->getY();
		float x2Velocity = spriteProps->getVelocityX();
		float y2Velocity = spriteProps->getVelocityY();
		float width2 = sprite->getSpriteType()->getTextureWidth();
		float height2 = sprite->getSpriteType()->getTextureWidth();*/
		







		if(isTimerOn == false)
		{
			//6isTimerOn == true;
			int knockBack = 10;
			int playerHeight = player->getSpriteType()->getTextureHeight();
			int playerWidth = player->getSpriteType()->getTextureWidth();
			int distance;
			bool willHitWall = false;
			float tileX;
			float tileY;
			float xToSet;
			float yToSet;

			if(spriteProps->getVelocityX() != 0 && spriteProps->getVelocityY() != 0)
			{
				if(playerProps->getVelocityX() > 0)		//PLAYER MOVING RIGHT
				{										//KNOCKBACK LEFT
					for(int i=0; i < collidableTiles->size(); i++)
					{
						tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
						tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
						if((playerProps->getX() + (knockBack * -1)) < (tileX+5))
						{
							willHitWall = true;
							xToSet = tileX;
						}

					}
					if(willHitWall == false)
						playerProps->setVelocityX(knockBack * -1);

				}

				if(playerProps->getVelocityX() < 0)		//PLAYER MOVING LEFT
				{										//KNOCKBACK RIGHT
					for(int i=0; i < collidableTiles->size(); i++)
					{
						tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
						tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
						if((playerProps->getX() + playerWidth + (knockBack)) > (tileX-5))
						{
							willHitWall = true;
							xToSet = tileX;
							break;
						}
					}
					if(willHitWall == false)
						playerProps->setVelocityX(knockBack);

				}

				if(playerProps->getVelocityY() > 0)		//PLAYER MOVING DOWN
				{										//KNOCKBACK UP
					for(int i=0; i < collidableTiles->size(); i++)
					{
						tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
						tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
						if((playerProps->getY() + (knockBack * -1)) < (tileY+64))
						{
							willHitWall = true;
							yToSet = tileY;
							break;
						}
					}
					if(willHitWall == false)
						playerProps->setVelocityY(knockBack * -1);

				}


				if(playerProps->getVelocityY() < 0)		//PLAYER MOVING UP
				{										//KNOCKBACK DOWN
					for(int i=0; i < collidableTiles->size(); i++)
					{
						tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
						tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
						if((playerProps->getY() + playerHeight + knockBack) > (tileY-5))
						{
							willHitWall = true;
							yToSet = tileY;
							break;
						}
					}
					if(willHitWall == false)
						playerProps->setVelocityY(knockBack);

				}
			}

			if(spriteProps->getVelocityX() > 0)		//SPRITE MOVING RIGHT
			{										//KNOCKBACK RIGHT

				for(int i=0; i < collidableTiles->size(); i++)
				{
					tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
					tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
					if((playerProps->getX() + playerWidth + (knockBack)) > (tileX-5))
					{
						willHitWall = true;
						xToSet = tileX;
						break;
					}
				}
				if(willHitWall == false)
					playerProps->setVelocityX(knockBack);

			}

			if(spriteProps->getVelocityX() < 0)		//SPRITE MOVING LEFT
			{										//KNOCKBACK LEFT
				for(int i=0; i < collidableTiles->size(); i++)
				{
					tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
					tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
					if((playerProps->getX() + (knockBack * -1)) < (tileX+5))
					{
						willHitWall = true;
						xToSet = tileX;
					}

				}
				if(willHitWall == false)
					playerProps->setVelocityX(knockBack * -1);
			}

			if(spriteProps->getVelocityY() > 0)		//SPRITE MOVING DOWN
			{										//KNOCKBACK DOWN
				for(int i=0; i < collidableTiles->size(); i++)
				{
					tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
					tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
					if((playerProps->getY() + playerHeight + knockBack) > (tileY-5))
					{
						willHitWall = true;
						distance = tileY - (playerProps->getY() + playerHeight);
						break;
					}
				}
				if(willHitWall == false)
					playerProps->setVelocityY(knockBack);
			}

			if(spriteProps->getVelocityY() < 0)		//SPRITE MOVING UP
			{										//KNOCKBACK UP
				for(int i=0; i < collidableTiles->size(); i++)
				{
					tileX = collidableTiles->at(i)->getBoundingVolume()->getX();
					tileY = collidableTiles->at(i)->getBoundingVolume()->getY();
					if((playerProps->getY() + (knockBack * -1)) < (tileY+64))
					{
						willHitWall = true;
						yToSet = tileY;
						break;
					}
				}
				if(willHitWall == false)
					playerProps->setVelocityY(knockBack * -1);

			}

			knockBackTimer++;
		}
		

		if(isInvincibleOn == false)
		{
			isInvincibleOn = true;
			playerProps->setCollidable(false);
			player->setAlpha(70);
		}


		if(knockBackTimer > 5)	//knockback lasts 5 frames
		{
			knockBackTimer = 0;
			isTimerOn == false;
		}

	}
	


	void GamePhysics::checkSpriteToTileCollisions(GameWorld *world)
	{
		for(int i=1; i < world->getSpriteManager()->getNumberOfSprites();i++)
		{
		
			spriteCollidableObjects->clear();
			spriteCollisionObjects->clear();
			spriteTilesToCheck->clear();

			AnimatedSprite *spriteToCheck = world->getSpriteManager()->getSprite(i);

			if(spriteToCheck->getSpriteType()->getSpriteTypeID() != 3)//projectile
			{
				checkSpriteToSparse(world, spriteToCheck);
				getSpriteOccupiedTiles(spriteToCheck, world);
				addSpriteCollisionObjects(world, spriteToCheck);
				findCollisionTimes(spriteCollisionObjects);
				resolveCollisions(world, spriteCollisionObjects,game);
				//createSpriteCollisionObjects();
				//resolveSpriteToTileCollisions();
			}

		}
	}


	void GamePhysics::getSpriteOccupiedTiles(AnimatedSprite *sprite, GameWorld *world)
	{
		int minColumn = sprite->getPhysicalProperties()->getX()/64;//64
		if((sprite->getPhysicalProperties()->getX()/64) - 1 >= 0)
			minColumn =(sprite->getPhysicalProperties()->getX()/64) - 1;
		int maxColumn = ((sprite->getPhysicalProperties()->getX() + sprite->getSpriteType()->getTextureWidth())/64);
		int minRow = sprite->getPhysicalProperties()->getY()/64;
		if((sprite->getPhysicalProperties()->getY()/64) - 1 >= 0)
			minRow =(sprite->getPhysicalProperties()->getY()/64) - 1;
		int maxRow = ((sprite->getPhysicalProperties()->getY() + sprite->getSpriteType()->getTextureHeight())/64);

		getSpriteTilesToCheck(world, sprite, minRow, minColumn, maxRow, maxColumn);

	}



	void GamePhysics::getSpriteTilesToCheck(GameWorld *world, AnimatedSprite *sprite, int minRow, int minColumn, int maxRow, int maxColumn)
	{

		TiledLayer *tiledLayer = world->getTiledLayer();
		//while (minColumn <= maxColumn)
		int i = minRow;
		int j = minColumn;

		while(i <= maxRow)
		{
			//int j = minColumn;
			//int minRowOld = minRow;
			//while(minRow <= maxRow)
			while (j <= maxColumn)
			{
				if(tiledLayer->getTile(i, j)->collidable == true)
				{
		
					PhysicalProperties *spriteProps = sprite->getPhysicalProperties();
					//check if the tile will collide next frame, edit this later for dynamic objects by accounting for velocity of object2
					if(willCollide(spriteProps->getX(), spriteProps->getY(), spriteProps->getVelocityX(), spriteProps->getVelocityY(), sprite->getSpriteType()->getTextureWidth(), sprite->getSpriteType()->getTextureHeight(),
								(j * 64)   , (i* 64)		, 0							 , 0						  ,    64														  ,		64															))
					{
						addSpriteCollidableObjects(tiledLayer->getTile(i, j), (j * 64), (i* 64)); //find x, y by mult row by tilesize. Add to collidableObjects vector
					}
					//tilesToCheck->push_back(tiledLayer->getTile(minRow, minColumn));	//add tile to vector
				}
				spriteTilesToCheck->push_back(tiledLayer->getTile(i, j));
				//minRow++;
				j++;
			}
			//minColumn++;
			i++;
			//minRow = minRowOld;
			j = minColumn;
		}

	}

	void GamePhysics::addSpriteCollidableObjects(Tile *tileToAdd, int x, int y)
	{
		
		objectToAdd = reusableCollidableObjects->at(currentReusable);
		objectToAdd->getBoundingVolume()->setX(x);
		objectToAdd->getBoundingVolume()->setY(y);
		objectToAdd->getBoundingVolume()->setHeight(64);
		objectToAdd->getBoundingVolume()->setWidth(64);
		
		spriteCollidableObjects->push_back(objectToAdd);

		currentReusable++;

	}


	void GamePhysics::addSpriteCollisionObjects(GameWorld *world, AnimatedSprite *sprite)	//this method takes collidable object pairs and creats collision objects
	{
		for(int i=0; i < spriteCollidableObjects->size(); i++)//checking for collision with player to tiles only, no sprites yet
		{
			//change player into collidableobject
			
			PhysicalProperties *spriteProps = sprite->getPhysicalProperties();
			BVtoAdd = reusableCollidableObjects->at(currentReusable)->getBoundingVolume();
			BVtoAdd->setHeight(sprite->getSpriteType()->getTextureHeight());
			BVtoAdd->setWidth(sprite->getSpriteType()->getTextureWidth());

			objectToAdd = reusableCollidableObjects->at(currentReusable);
			objectToAdd->setPhysicalProperties(spriteProps);
			objectToAdd->setBoundingVolume(BVtoAdd);

			//collision between player and tile
			collisionToAdd = reusableCollisionObjects->at(currentReusableCollision);
			collisionToAdd->setCb1(objectToAdd);
			collisionToAdd->setCb2(spriteCollidableObjects->at(i));
			spriteCollisionObjects->push_back(collisionToAdd);
			currentReusableCollision++;
			currentReusable++;
		}
	}

	
	void GamePhysics::checkSpriteToSparse(GameWorld *world, AnimatedSprite *sprite)
	{
		PhysicalProperties *spriteProps = sprite->getPhysicalProperties();
		OverlayImage *sparseTilesToCheck;

		for(int i=0; i < world->getSparseLayer()->getNumberOfSparseTiles();i++)
		{
			sparseTilesToCheck = world->getSparseLayer()->getTile(i);
			if(sprite->getSpriteType()->getSpriteTypeID() != 2)
			{
				if(willCollide(spriteProps->getX(), spriteProps->getY(), spriteProps->getVelocityX(), spriteProps->getVelocityY(), sprite->getSpriteType()->getTextureWidth(), sprite->getSpriteType()->getTextureHeight(), 
				sparseTilesToCheck->x, sparseTilesToCheck->y, 0, 0, sparseTilesToCheck->width, sparseTilesToCheck->height))
				{
				
					{
						if(sparseTilesToCheck->isCollidable == true)
						{

							sprite->getPhysicalProperties()->setVelocityX(0);
							sprite->getPhysicalProperties()->setVelocityY(0);
							if (sprite->getSpriteType()->getSpriteTypeID()==3)
							{
								sprite->setAlpha(0);
								sprite->setCurrentlyCollidable(false);
							}
						
						
							cue="wall collide";
							cues->push_back(cue);

						}
					
					}
				}
			}
			
		}
	}



	void GamePhysics::checkSparseObjects(Game *game)
	{
		PhysicalProperties *playerProps = game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties();
		int playerHeight = game->getWorld()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureHeight();
		int playerWidth = game->getWorld()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth();
		Item *itemFound;
		game->setItemFound("empty");
		game->setInFrontOf(false);
		int searchRange = 2;

		OverlayImage *sparseTileToCheck;

		for(int i=0; i < game->getWorld()->getSparseLayer()->getNumberOfSparseTiles();i++)
		{
			sparseTileToCheck = game->getWorld()->getSparseLayer()->getTile(i);

			if(playerProps->getDirection() == 0)//facing left
			{
				if((playerProps->getX() - searchRange) < (sparseTileToCheck->x + sparseTileToCheck->width) && playerProps->getX() > sparseTileToCheck->x)
				{
					if((playerProps->getY() + 25) > sparseTileToCheck->y && (playerProps->getY() + 25)  < (sparseTileToCheck->y + sparseTileToCheck->height))
					{
						if (sparseTileToCheck->itemContained.compare("empty")!=0)
						{
							if (!game->getPlayerObject()->hasItem(sparseTileToCheck->itemContained))
							{
								game->setItemFound(sparseTileToCheck->itemContained);
							}
							else
							{
								string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
								wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
								game->setDescriptor(descriptor);
								game->setInFrontOf(true);
							}
							sparseTileToCheck->itemContained="empty";
						}
						else 
						{
							string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
							wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
							game->setDescriptor(descriptor);
							game->setInFrontOf(true);
						}
						if (sparseTileToCheck->name.compare("knightSword")==0)
						{
							sparseTileToCheck->alpha=0;
							game->setKnightsAwake(true);
						}
						if (sparseTileToCheck->name.compare("boat")==0 && sparseTileToCheck->alpha==255)
						{
							game->changeGameState(CHECK_GAME_STATE);
							game->setPaused(true);
							game->getText()->clearText();
							string s = "CONGRATULATIONS, YOU HAVE ESCAPED!";
							wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
							game->getText()->addText(descriptor, 360,140,1000,1000);
							return;
						}
					}
				}
			}

			else if(playerProps->getDirection() == 2)//facing right
			{
				if((playerProps->getX() + playerWidth + searchRange) > sparseTileToCheck->x && playerProps->getX() < sparseTileToCheck->x)
				{
					if((playerProps->getY() + 25) > sparseTileToCheck->y && (playerProps->getY() + 25)  < (sparseTileToCheck->y + sparseTileToCheck->height))
					{
						if (sparseTileToCheck->itemContained.compare("empty")!=0)
						{
							if (!game->getPlayerObject()->hasItem(sparseTileToCheck->itemContained))
							{
								game->setItemFound(sparseTileToCheck->itemContained);
							}
							else
							{
								string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
								wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
								game->setDescriptor(descriptor);
								game->setInFrontOf(true);
							}
							sparseTileToCheck->itemContained="empty";
						}
						else 
						{
							string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
							wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
							game->setDescriptor(descriptor);
							game->setInFrontOf(true);
						}
							if (sparseTileToCheck->name.compare("knightSword")==0)
						{
							sparseTileToCheck->alpha=0;
							game->setKnightsAwake(true);
						}
					}
				}
			}

			else if(playerProps->getDirection() == 1)//facing up
			{
				if((playerProps->getY() - searchRange) < (sparseTileToCheck->y + sparseTileToCheck->height) && playerProps->getY() > sparseTileToCheck->y)
				{
					if((playerProps->getX() + 15) > sparseTileToCheck->x && (playerProps->getX() + 15)  < (sparseTileToCheck->x + sparseTileToCheck->width))
					{
						if (sparseTileToCheck->itemContained.compare("empty")!=0)
						{
							if (!game->getPlayerObject()->hasItem(sparseTileToCheck->itemContained))
							{
								game->setItemFound(sparseTileToCheck->itemContained);
							}
							else
							{
								string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
								wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
								game->setDescriptor(descriptor);
								game->setInFrontOf(true);
							}
							sparseTileToCheck->itemContained="empty";
						}
						else 
						{
							string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
							wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
							game->setDescriptor(descriptor);
							game->setInFrontOf(true);
						}
							if (sparseTileToCheck->name.compare("knightSword")==0)
						{
							sparseTileToCheck->alpha=0;
							game->setKnightsAwake(true);
						}
					}
				}
			}

			else if(playerProps->getDirection() == 3)//facing down
			{
				if((playerProps->getY() + playerHeight + searchRange) > sparseTileToCheck->y && playerProps->getY() < sparseTileToCheck->y)
				{
					if((playerProps->getX() + 15) > sparseTileToCheck->x && (playerProps->getX() + 15)  < (sparseTileToCheck->x + sparseTileToCheck->width))
					{
						if (sparseTileToCheck->itemContained.compare("empty")!=0)
						{
							if (!game->getPlayerObject()->hasItem(sparseTileToCheck->itemContained))
							{
								game->setItemFound(sparseTileToCheck->itemContained);
							}
							else
							{
								string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
								wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
								game->setDescriptor(descriptor);
								game->setInFrontOf(true);
							}
							sparseTileToCheck->itemContained="empty";
							
						}
						else 
						{
							string s = sparseTileToCheck->descriptor1 + "\n" + sparseTileToCheck->descriptor2 + "\n" + sparseTileToCheck->descriptor3
								+ "\n" + sparseTileToCheck->descriptor4;
							wchar_t *descriptor = game->getDataLoader()->stringToLCPWSTR(s);
							game->setDescriptor(descriptor);
							game->setInFrontOf(true);
						}
						if (sparseTileToCheck->name.compare("knightSword")==0)
						{
							sparseTileToCheck->alpha=0;
							game->setKnightsAwake(true);
						}
					}
				}
			}
		}
	}


	void GamePhysics::checkWhichSparseObject(Game *game, OverlayImage *sparseTileToCheck)
	{
		game->setItemFound(sparseTileToCheck->itemContained);
	}


	void GamePhysics::checkEnemyHit(Game *game)
	{
		AnimatedSprite *player = game->getWorld()->getSpriteManager()->getPlayer();
		PhysicalProperties *playerProps = game->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties();
		int playerHeight = game->getWorld()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureHeight();
		int playerWidth = game->getWorld()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth();

		int wideAttackRange = 60;
		int attackRange = 20;
		int knockback = 25;

		AnimatedSprite *enemyToCheck;

		for(int i=0; i < game->getWorld()->getSpriteManager()->getNumberOfSprites();i++)
		{
			enemyToCheck = game->getWorld()->getSpriteManager()->getSprite(i);
			PhysicalProperties *enemyProps = enemyToCheck->getPhysicalProperties();
			BoundingVolume *enemyBV = enemyToCheck->getBoundingVolume();
			
			if(enemyToCheck->getSpriteType()->getSpriteTypeID() != 5 && enemyToCheck->getSpriteType()->getSpriteTypeID() != 6)
			{
				float halfEnemyHeight = (enemyToCheck->getSpriteType()->getTextureHeight()/2);
				float halfEnemyWidth = (enemyToCheck->getSpriteType()->getTextureWidth()/2);
				if(playerProps->getDirection() == 0)//facing left
				{
					if((playerProps->getX() - (40)) < (enemyProps->getX() + enemyBV->getWidth()) && playerProps->getX() > enemyProps->getX())
					{
						//if((playerProps->getY() + 25) > enemyProps->getY() && (playerProps->getY() + 25)  < (enemyProps->getY() + enemyBV->getHeight()))
						float distance = fabs(playerProps->getY() - (enemyProps->getY()+halfEnemyHeight));
						if(playerProps->getY() < enemyProps->getY())
							distance = fabs(playerProps->getY() - enemyProps->getY());
						if(distance < wideAttackRange)
						{
							if(enemyProps->isCollidable())
							{
								enemyProps->setX(enemyProps->getX() - knockback);
								enemyToCheck->setHP(enemyToCheck->getHP() - player->getPlayerObject()->getAtkEquipped()->damage);
								//check if enemy has no health left
								checkEnemyHealth(enemyToCheck);
							}

						}
					}
				}

				if(playerProps->getDirection() == 2)//facing right
				{
					if((playerProps->getX() + playerWidth + attackRange) > enemyProps->getX() && playerProps->getX() < enemyProps->getX())
					{
						//if((playerProps->getY() + 25) > enemyProps->getY() && (playerProps->getY() + 25)  < (enemyProps->getY() + enemyBV->getHeight()))
						float distance = fabs(playerProps->getY() - (enemyProps->getY()+halfEnemyHeight));
						if(playerProps->getY() < enemyProps->getY())
							distance = fabs(playerProps->getY() - enemyProps->getY());
						
						if(distance < wideAttackRange)
						{
							if(enemyProps->isCollidable())
							{
								enemyProps->setX(enemyProps->getX() + knockback);
								//enemyToCheck->setHP(enemyToCheck->getHP() - player->getPlayerObject()->getAtkEquipped()->damage);
								enemyToCheck->setHP(enemyToCheck->getHP() - player->getPlayerObject()->getAtkEquipped()->damage);
								//check if enemy has no health left
								checkEnemyHealth(enemyToCheck);
							}
						}
					}
				}

				if(playerProps->getDirection() == 1)//facing up
				{
					if((playerProps->getY() - attackRange) < (enemyProps->getY() + enemyBV->getHeight()) && playerProps->getY() > enemyProps->getY())
					{
						//if((playerProps->getX() + 15) > enemyProps->getX() && (playerProps->getX() + 15)  < (enemyProps->getX() + enemyBV->getWidth()))
						float distance = fabs(playerProps->getX() - (enemyProps->getX() + halfEnemyWidth));
						
						if(distance < wideAttackRange)
						{
							if(enemyProps->isCollidable())
							{
								enemyProps->setY(enemyProps->getY() - knockback);
								enemyToCheck->setHP(enemyToCheck->getHP() - player->getPlayerObject()->getAtkEquipped()->damage);
								//check if enemy has no health left
								checkEnemyHealth(enemyToCheck);
							}
						}
					}
				}

				if(playerProps->getDirection() == 3)//facing down
				{
					if((playerProps->getY() + playerHeight + attackRange) > enemyProps->getY() && playerProps->getY() < enemyProps->getY())
					{
						//if((playerProps->getX() + 15) > enemyProps->getX() && (playerProps->getX() + 15)  < (enemyProps->getX() + enemyBV->getWidth()))
						float distance = fabs(playerProps->getX() - (enemyProps->getX() + halfEnemyWidth));
						
						if(distance < wideAttackRange)
						{
							if(enemyProps->isCollidable())
							{
								enemyProps->setY(enemyProps->getY() + knockback);
								enemyToCheck->setHP(enemyToCheck->getHP() - player->getPlayerObject()->getAtkEquipped()->damage);
								//check if enemy has no health left
								checkEnemyHealth(enemyToCheck);
							}
						}
					}
				}
			}
			

		}

	}

	void GamePhysics::checkEnemyHealth(AnimatedSprite *enemyToCheck)
	{
		if(enemyToCheck->getSpriteType()->getSpriteTypeID() != 3)//not a projectile
		{
			if(enemyToCheck->getHP() <= 0)
			{
				if(enemyToCheck->getSpriteType()->getSpriteTypeID() == 7)//vampire
				{
					enemyToCheck->setCurrentState(9);
				}
				else if(enemyToCheck->getSpriteType()->getSpriteTypeID() == 2)//boss
				{
					enemyToCheck->setCurrentState(0);
					enemyToCheck->setAlpha(0);
					
					
				}
				else//zombie
				{
					enemyToCheck->setCurrentState(8);
				}

				enemyToCheck->getPhysicalProperties()->setVelocityX(0);
				enemyToCheck->getPhysicalProperties()->setVelocityY(0);
				enemyToCheck->getPhysicalProperties()->setCollidable(false);
				enemyToCheck->setCurrentlyCollidable(false);
			}
		}
		
	}