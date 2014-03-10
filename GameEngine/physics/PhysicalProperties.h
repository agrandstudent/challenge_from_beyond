#pragma once
#include "resource.h"
#include "stdafx.h"
const float MAX_VELOCITY = 100.0f;
class PhysicalProperties
{
protected:
	float buoyancy;
	float mass;
	bool  collidable;
	bool  state;
	bool inAir;
	float coefficientOfRestitution;
	float x;
	float y;
	float z;
	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;
	int direction;	//LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3

public:
	PhysicalProperties()	{accelerationX = 0; accelerationY = 0;}
	~PhysicalProperties()	{}

	// INLINED METHODS
	float getBuoyancy()						{ return buoyancy;				}
	float getMass()							{ return mass;					}
	float getCoefficientOfRestitution()		{ return coefficientOfRestitution;	}
	bool isCollidable()						{ return collidable;			}
	bool getInAir()							{ return inAir;					}
	float getX()							{ return x;						}
	float getY()							{ return y;						}
	float getZ()							{ return z;						}
	float getVelocityX()					{ return velocityX;				}
	float getVelocityY()					{ return velocityY;				}
	float getAccelerationX()				{ return accelerationX;			}
	float getAccelerationY()				{ return accelerationY;			}
	int	  getDirection()					{ return direction;				}
	void setBuoyancy(float initBuoyancy)	{ buoyancy = initBuoyancy;		}
	void setMass(float initMass)			{ mass = initMass;				}
	void setCofficientOfRestitution(float cR) { coefficientOfRestitution = cR; }
	void setCollidable(bool initCollidable)	{ collidable = initCollidable;	}
	void setX(float initX)					{ x = initX;					}
	void incX(float xToInc)					{ x += xToInc;					}
	void setY(float initY)					{ y = initY;					}
	void incY(float yToInc)					{ y += yToInc;					}
	void setZ(float initZ)					{ z = initZ;					}
	void setInAir(bool tf)					{ inAir = tf;					}
	void setDirection(int initDirection)	{ direction = initDirection;	}
	void capVelocity(float &velocity)
	{
		if (velocity > MAX_VELOCITY)
			velocity = MAX_VELOCITY;
		else if (velocity < -MAX_VELOCITY)
			velocity = -MAX_VELOCITY;
	}

	void setVelocityX(float initVx)			
	{
		velocityX = initVx;			
		capVelocity(velocityX);
	}
	void incVelocityX(float incVx)			
	{ 
		velocityX += (accelerationX + incVx);	
		capVelocity(velocityX);
	}
	void setVelocityY(float initVy)			
	{ 
		velocityY = initVy;			
		capVelocity(velocityY);
	}
	void incVelocityY(float incVy)						
	{ 
		velocityY += (accelerationY + incVy);	
		capVelocity(velocityY);
	}
	void setAccelerationX(float initAx)		{ accelerationX = initAx;		}
	void setAccelerationY(float initAy)		{ accelerationY = initAy;		}
};