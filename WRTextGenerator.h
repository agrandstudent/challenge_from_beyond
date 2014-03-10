/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRTextGenerator.h

	This class is a custom text generator for the EmptyGame
	application. Basically, by defining the initText and updateText
	methods, we can have it display whatever text we like.
*/

#pragma once
#include "stdafx.h"
#include "TextGenerator.h"

class WRTextGenerator : public TextGenerator
{
public:
	WRTextGenerator()	{}
	~WRTextGenerator()	{}
	void initText(Game *game);
	void updateText(Game *game);
}
;