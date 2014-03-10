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


class CollisionObject
{

protected:
	float firstTimeOfCollision;
	float lastTimeOfCollision;
	int direction;
	CollidableObject *cb1;
	CollidableObject *cb2;
	AnimatedSprite *animatedSprite;
	bool  collidable;


public:

	//variables we need
	

	// WE'LL DEFINE THIS CLASS LATER
	CollisionObject(){};
	~CollisionObject(){};

	CollidableObject* getCb1(){ return cb1; }
	CollidableObject* getCb2(){ return cb2; }
	float getFirstTimeOfCollision(){ return firstTimeOfCollision; }
	float getLastTimeOfCollision(){ return lastTimeOfCollision; }
	int getDirection(){return direction; }
	AnimatedSprite* getAnimatedSprite(){ return animatedSprite; }

	void setCb1(CollidableObject* cObject1){ cb1 = cObject1; }
	void setCb2(CollidableObject* cObject2){ cb2 = cObject2; }
	void setFirstTimeOfCollision(float time){ firstTimeOfCollision = time; }
	void setLastTimeOfCollision(float time){ lastTimeOfCollision = time;   }
	void setDirection(int xy){ direction = xy;}
	void setAnimatedSprite(AnimatedSprite *s){ animatedSprite = s;}
};