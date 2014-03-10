/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	StringTable.cpp

	See StringTable.h for a class description.
*/

#include "stdafx.h"
#include "StringTable.h"
#include <vector>

/*
	StringTable - Default constructor, it creates our vector for storing
	the wchar_ts.
*/
StringTable::StringTable()
{
	// CONSTRUCT wchar_t TABLE
	stringTable = new vector<wchar_t*>();
}

/*
	~StringTable - Destructor, it recovers the memory used by the 
	wchar_t table. We'll have to delete all the contents as well.
*/
StringTable::~StringTable()
{
	clear();
	delete stringTable;
}

/*
	clear - This method clears out the wchar_t table vector, deleting
	all of the contents.
*/
void StringTable::clear()
{
	wchar_t *dataToDelete;
	int i;
	for (i = stringTable->size() - 1; i >= 0; i--)
	{
		dataToDelete = stringTable->at(i);
		delete dataToDelete;
	}
	stringTable->empty();
}

/*
	getIndexOfStringFromStringTable - This method searches for the key
	parameter in the wchar_t table. If found, it returns it's index, if
	not found it returns -1;
*/
int StringTable::getIndexOfStringFromStringTable(wchar_t *key)
{
	size_t i = 0;
	bool found = false;
	for (;(i < stringTable->size()) && !found; i++)
	{
		wchar_t *test = stringTable->at(i);
		int testResult = wcscmp(key, test);
		if (testResult == 0) 
			found = true;
	}
	if (!found)
		return -1;
	else
		return i-1;
}

/*
	putStringInStringTable - This method is for adding elements
	to the wchar_t table. Since the wchar_t table only allows for
	single copies of each element, this method first checks to 
	see if the key wchar_t is already inside. If it is, it simply
	returns its index. If it's not found, it adds it and returns
	the index of the location where it was added.
*/
int StringTable::putStringInStringTable(wchar_t *key)
{
	bool found = false;
	size_t i = 0;
	for (; i < stringTable->size() && !found; i++)
	{
		if (wcscmp(key, stringTable->at(i)) == 0)
			found = true;
	}
	if (!found)
	{
		stringTable->push_back(key);
			return stringTable->size()-1;
	}
	else
		return i-1;	
}
