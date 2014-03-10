/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileWriter.cpp

	See TextFileWriter.h for a class description.
*/

#include "stdafx.h"
#include "TextFileWriter.h"

/*
	TextFileWriter - This constructor opens a text stream
	to the file provided as an argument, initFileName.
*/
TextFileWriter::TextFileWriter()
{
	fileName = NULL;
	outFile = NULL;
}

/*
	~TextFileWriter - This destructor makes sure the text
	stream is closed before destroying it and the file name
	pointers.
*/
TextFileWriter::~TextFileWriter()
{
	closeWriter();
}

/*
	closeWriter - This method closes the stream used for 
	reading from a file and then deletes the inFile stream. If
	you want to read again from the stream, you must call initFile
	first, which will re-initialize everything.
*/
void TextFileWriter::closeWriter()
{
	if (fileName != NULL)
	{
		delete fileName;
		fileName = NULL;
	}

	if (outFile != NULL)
	{
		outFile->close();
		delete outFile;
		outFile = NULL;
	}
}

/*
	initFile - This method initializes the stream for writing
	to a file using the file name provided as an argument. After
	calling this method, we are ready to write text.
*/
void TextFileWriter::initFile(wchar_t *initFileName)
{
	fileName = initFileName;
	outFile = new wofstream();
	outFile->open(fileName);
}

/*
	writeText - This method appends the textToWrite argument
	to the end of the text file this object writes to.
*/
void TextFileWriter::writeText(wchar_t *textToWrite)
{	
	outFile->write(textToWrite, wcslen(textToWrite));
}