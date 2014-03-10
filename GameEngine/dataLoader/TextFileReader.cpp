/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileReader.cpp

	See TextFileReader.h for a class description.
*/

#include "stdafx.h"
#include "TextFileReader.h"
#include "WorldScrollingGame.h"
#include "GameDataLoader.h"

/*
	Default Constructor - We don't know the name of the file
	to use yet, so we'll initialize everything to NULL.
*/
TextFileReader::TextFileReader()
{
	fileName = NULL;
	inFile = NULL;
}

/*
	Deconstructor - Make sure the file stuff doesn't cause
	a memory leak.
*/
TextFileReader::~TextFileReader()
{
	closeReader();
}

/*
	closeReader - This method closes the stream used for 
	reading from a file and then deletes the inFile stream. If
	you want to read again from the stream, you must call initFile
	first, which will re-initialize everything.
*/
void TextFileReader::closeReader()
{
	if (fileName != NULL)
	{
		delete fileName;
		fileName = NULL;
	}

	if (inFile != NULL)
	{
		inFile->close();
		delete inFile;
		inFile = NULL;
	}
}

/*
	initFile - This method initializes the stream for reading
	from a file using the file name provided as an argument. After
	calling this method, we are ready to read text.
*/
void TextFileReader::initFile(wchar_t *initFileName)
{
	fileName = initFileName;
	inFile = new ifstream();
	inFile->open(fileName);
}

/*
	readLineOfText - This method reads a line of text from the
	file and returns it. Note that before calling this method, first
	call initFile to setup the stream. Note that this method is
	allocating memory on the heap for 256 characters for each line,
	so that should not be exceeded. Also, remember to delete the
	line when done using it to avoid memory leaks.
*/
wchar_t* TextFileReader::readLineOfText()
{
    if (!inFile)
        return NULL;

	char readText[256];
	inFile->getline(readText, 256);

	wchar_t *charLine = new wchar_t[256];
	charLine[0] = '\0';

	mbtowc(charLine, readText, 256 * sizeof(char));
	return charLine;
}


void TextFileReader::loadFiles(Game *game)
{
	
	using namespace std;
	ifstream inputFile;
	string lineRead;
	inputFile.open("fileLocations.csv");
if (inputFile)
{
	char inputLine[255];
	stringstream ss;

	while (getline(inputFile, lineRead))
	{
		ss<<lineRead;
		while (getline(ss, lineRead, ','))
		{
			if (lineRead.compare("gameMenu") ==0)
			{
				getline(ss,lineRead, ',');
				game->getDataLoader()->setGameMenuFile(lineRead);
			}
			if (lineRead.compare("spritesFile") ==0)
			{
				getline(ss,lineRead, ',');
				game->getDataLoader()->setSpritesFile(lineRead);
			}
			if (lineRead.compare("worldSpecs") ==0)
			{
				getline(ss,lineRead, ',');
				game->getDataLoader()->setWorldSpecsFile(lineRead);
			}
			if (lineRead.compare("actFiles") ==0)
			{
				while(getline(ss, lineRead, ','))
				{
					wchar_t* levelFile=constructEmptyWCHAR_TArray(game->getDataLoader()->stringToLCPWSTR(lineRead));
					game->getWorld()->addLevelFileName(levelFile);
				}
			}
			if (lineRead.compare("tileFiles")==0)
			{
				getline(ss,lineRead, ',');
				game->getDataLoader()->setTileFile(lineRead);
			}
			if (lineRead.compare("itemFiles")==0)
			{
				getline(ss,lineRead, ',');
				game->getDataLoader()->setItemFile(lineRead);
			}
			if (lineRead.compare("introFile")==0)
			{
				getline(ss,lineRead, ',');
				game->getDataLoader()->setIntroFile(lineRead);
			}
		}
		ss.clear();
	}
}


}