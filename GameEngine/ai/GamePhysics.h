/*	
	Author: _______________

	GamePhysics.h

	This class will be used to manage all game collisions
	and physics. This includes collision detection, and resolution.
	Each frame we will update the collided objects velocities and
	positions accordingly.

	Students will implement this as part of their projects.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"

#include "stdafx.h"
#include "TextFileReader.h"
#include "GameDataLoader.h"
#include "TiledLayer.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameWorld.h"

#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "BoundingVolume.h"
#include "CollidableObject.h"
#include "CollisionObject.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameWorld.h"
#include "PhysicalProperties.h"
#include "TiledLayer.h"
#include "TextFileReader.h"
#include "SparseLayer.h"
#include "SpriteManager.h"
#include "Viewport.h"
#include "WorldLayer.h"
#include "XactSound.h"
#include "Item.h"

class GamePhysics
{

protected:
	float buoyancy;
	float mass;
	bool  collidable;
	float coefficientOfRestitution;
	float x;
	float y;
	float z;
	


public:

	//variables we need
	
	bool isReusableCollisionsEmpty;
	bool isReusableEmpty;
	bool isCollidable;
	bool inAir;
	bool isTimerOn;
	bool isInvincibleOn;
	bool isWallSoundTimerOn;
	int currentReusable;
	int currentReusableCollision;
	int minColumn;
	int maxColumn;
	int minRow;
	int maxRow;
	int knockBackTimer;
	int invincibleTimer;
	int wallSoundTimer;
	TiledLayer *tiledLayer;
	vector<Tile*> *tilesToCheck;
	vector<CollidableObject*> *collidableObjects;
	vector<CollidableObject*>  *reusableCollidableObjects;
	vector<CollisionObject*>   *reusableCollisionObjects;
	vector<CollisionObject*>  *collisionObjects;
	vector<PCSTR> *cues;
	vector<CollidableObject*> *collidableTiles;

	//sprite stuff
	vector<CollidableObject*>	*spriteCollidableObjects;
	vector<CollisionObject*>	*spriteCollisionObjects;
	vector<Tile*>	*spriteTilesToCheck;
	BoundingVolume *BVtoAdd;
	PhysicalProperties *PPtoAdd;
	CollidableObject *objectToAdd;
	CollisionObject *collisionToAdd;


	// WE'LL DEFINE THIS CLASS LATER
	GamePhysics();
	~GamePhysics();

	bool willCollide(float x1, float y1, float x1Velocity, float y1Velocity, float width1, float height1, 
								  float x2, float y2, float x2Velocity, float y2Velocity, float width2, float height2);

	void update(Game *game); 
	void getOccupiedTiles(AnimatedSprite *player, GameWorld *world);
	void getTilesToCheck(GameWorld *world);
	void addCollidableObjects(Tile *tileToAdd, int x, int y, vector<CollidableObject*> *cObjects);
	void addCollisionObjects(GameWorld *world);
	void findCollisionTimes(vector<CollisionObject*> *objectsToCheck);
	void sortCollisionObjects();
	void resolveCollisions(GameWorld *world, vector<CollisionObject*> *objectsToCheck,Game *game);
	void createCollidableObjects();
	void createCollisionObjects();
	void checkInAir(AnimatedSprite *player);
	void checkOutOfScreen(Game *game);
	void checkSpriteCollisions(Game *game);
	void checkPlayerHP(Game *game);
	void resolveSpriteCollisions(AnimatedSprite *player, AnimatedSprite *sprite);
	void checkSparseObjects(Game *game);
	void checkWhichSparseObject(Game *game, OverlayImage *sparseTileToCheck);
	void checkEnemyHit(Game *game);
	void checkEnemyHealth(AnimatedSprite *enemyToCheck);

	//SPRITE STUFF
	void checkSpriteToTileCollisions(GameWorld *world);
	void getSpriteOccupiedTiles(AnimatedSprite *sprite, GameWorld *world);
	void getSpriteTilesToCheck(GameWorld *world, AnimatedSprite *sprite, int minRow, int minColumn, int maxRow, int maxColumn);
	void addSpriteCollidableObjects(Tile *tileToAdd, int x, int y);
	void addSpriteCollisionObjects(GameWorld *world, AnimatedSprite *sprite);
	void checkSpriteToSparse(GameWorld *world, AnimatedSprite *sprite);
	

	
};