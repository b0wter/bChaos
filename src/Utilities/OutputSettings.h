/*
 * OutputSettings.h
 *
 *  Created on: 2 Mar 2011
 *      Author: b0wter
 */

#ifndef OUTPUTSETTINGS_H_
#define OUTPUTSETTINGS_H_

#include "Utils.h"

using namespace std;

//! Stores information on what is to be done with the computed values for output.
/*!
 * This class saves information wether the data should be printed in a terminal, or a file.
 */
class OutputSettings
{
	bool terminalOutput;
	bool fileOutput;
	string fileName;

	//! Generates an instance of OutputSettings from a string.
	/*!
	 * Syntax for the line: \n
	 * "output=terminal;"
	 * just prints the data into the terminal, where as
	 * "output=file,output.dat;"
	 * prints the data into data1.dat.
	 * "output=both,data1.dat;"
	 * puts the data both, into terminal and file.
	 */
	static OutputSettings* fromString(string line);
};

#endif /* OUTPUTSETTINGS_H_ */
