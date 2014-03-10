/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameText.h

	This class allows for easy rendering of text
	to the game screen.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include <vector>

class Game;
class TextGenerator;

const int MAX_TEXT_OBJECTS = 10;

class TextToDraw
{
private:
	wchar_t *text;

public:
	TextToDraw()	{}
	~TextToDraw()	{}
	int maxChars;
	int numChars;
	int x;
	int y;
	int width;
	int height;

	void setText(wchar_t *textToSet)
	{
		text = textToSet;
	}
	wchar_t* getText() { return text; }
};

class GameText
{
private:
	int textCounter;
	vector<TextToDraw*> *textToDraw;
	TextGenerator *textGenerator;
	
public:
	// INLINED ACCESSOR METHODS
	int				getNumTextObjectsToDraw()		{	return textToDraw->size();		}
	TextGenerator*	getTextGenerator()				{	return textGenerator;			}
	TextToDraw*		getTextToDrawAtIndex(int index)	{	return textToDraw->at(index);	}	

	// INLINED MUTATOR METHOD
	void setTextGenerator(TextGenerator *initTextGenerator)
	{
		textGenerator = initTextGenerator;
	}

	// METHODS DEFINED IN GameText.cpp
	GameText();
	~GameText();
	void addText(wchar_t *textToAdd, int initX, int initY, int initWidth, int initHeight);
	void changeTextOnly(wchar_t *textToAdd, int index);
	void moveText(int index, int xMove, int yMove);
	void clearText();
};