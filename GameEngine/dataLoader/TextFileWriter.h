/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	TextFileWriter.h

	This is a wrapper class for writing text to a file.
	Use the init method to initialize the stream. Text 
	can then be written to the file using the writeText
	method. Re-init the object to write to a new file.
	Also, remember to delete pointers of this object
	when done.
*/

#pragma once
#include "stdafx.h"

class TextFileWriter
{
private:
	// THE NAME OF THE TEXT FILE THIS OBJECT WILL WRITE TO
	wchar_t *fileName;

	// THE OUTPUT STREAM WE WILL USE TO WRITE
	wofstream *outFile;

public:
	// METHODS DEFINED IN TextFileWriter.cpp
	TextFileWriter();
	~TextFileWriter();
	void closeWriter();
	void initFile(wchar_t *initFileName);
	void writeText(wchar_t *textToWrite);
};