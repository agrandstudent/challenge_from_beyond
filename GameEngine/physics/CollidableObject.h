
#pragma once
#include "resource.h"
#include "stdafx.h"
#include "BoundingVolume.h"
#include "PhysicalProperties.h"
#include <vector>

class CollidableObject
{
protected:
	bool currentlyCollidable;
	BoundingVolume *bv;
	PhysicalProperties *pp;



public:
	CollidableObject()	{}
	~CollidableObject()	{}

	// INLINED METHODS
	bool isCurrentlyCollidable() { return currentlyCollidable; }
	BoundingVolume* getBoundingVolume() { return bv; }
	PhysicalProperties* getPhysicalProperties() { return pp; }

	void setCurrentlyCollidable(bool initCurrentlyCollidable)
	{	currentlyCollidable = initCurrentlyCollidable; }
	void setBoundingVolume(BoundingVolume *bvToSet)
	{	bv = bvToSet;	}
	void setPhysicalProperties(PhysicalProperties *initPP)
	{	pp = initPP;	}
};