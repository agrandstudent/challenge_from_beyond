/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StringTable.h

	This class manages an array of wchar_ts in a vector. Basically it is
	useful for storing the names of loaded resources so that all the game
	objects don't have to. They can instead just store the indices of the
	resources in this table in order to save memory and trade in the id
	for the resource when needed.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include <vector>

class StringTable
{
private:
	vector<wchar_t*> *stringTable;

public:
	// INLINED ACCESSOR METHOD
	wchar_t* getStringAtIndexFromStringTable(int index)	{	return stringTable->at(index);	}

	// METHODS DEFINED IN StringTable.cpp
	StringTable();
	~StringTable();
	void	clear();
	int		getIndexOfStringFromStringTable(wchar_t *key);
	int		putStringInStringTable(wchar_t *key);
};