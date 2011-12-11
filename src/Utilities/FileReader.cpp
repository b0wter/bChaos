/*
 * FileReader.cpp
 *
 *  Created on: 25 Feb 2011
 *      Author: b0wter
 */

#include "FileReader.h"

namespace Utilities
{

FileReader::FileReader(string fileName)
{
	ENTER
	input.open(fileName.c_str());

	if(!input.is_open())
		ERROR("Could not open file: " + fileName);
	LEAVE
}

FileReader::~FileReader()
{
	ENTER	;
	if(input.is_open())
		input.close();
	LEAVE ;
}

bool FileReader::readLine(string* target)
{
	//ENTER
	if(input.good())
	{
		getline(input, *target);
		//LEAVE
		return true;
	}
	//LEAVE
	return false;
}


} // end of namespace
