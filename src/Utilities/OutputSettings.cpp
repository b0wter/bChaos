/*
 * OutputSettings.cpp
 *
 *  Created on: 2 Mar 2011
 *      Author: b0wter
 */

#include "OutputSettings.h"


OutputSettings* OutputSettings::fromString(string line)
{
	OutputSettings* settings = NULL;

	// one needs to search for 2 strings.
	// 1. contains where the data should be printed to ("terminal", "file", "both")
	// 2. the filename (only if needed)
	size_t equalChar, commaChar, semiChar;
	equalChar = line.find('=');
	commaChar = line.find(',');
	semiChar = line.find(':');

	// check if there is an equal sign.
	if(equalChar == string::npos)
	{
		//WARNING("Broken outputSettings line found! (could not find a '=') - " + line)
		return NULL;
	}

	if(line.find("terminal", equalChar) != string::npos)
	{
		// just check if terminal is not part of the filename
		if(line.find("file"), equalChar != string::npos && commaChar != string::npos)
		{
			// probably a broke line. Skip it and print a warning
			//WARNING("Found strange output settings in config file. Will be skipped - " + line)
			return NULL;
		}
		else
		{
			// settings seem fine

		}

	}
	else if(line.find("file", equalChar) != string::npos)
	{

	}
	else
	{
		//WARNING("Could not determine type of output (file/terminal) from config file - " + line)
		return NULL;
	}

	return settings;
}
