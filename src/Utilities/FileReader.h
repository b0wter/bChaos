/*
 * FileReader.h
 *
 *  Created on: 25 Feb 2011
 *      Author: b0wter
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include "Utils.h"

using namespace std;
/*
 *
 */
namespace Utilities
{
//! Opens files for input.
/*!
 * The file reader is encapsuled in a seperate class to make sure that, even if something in the program goes wrong
 * the file is still close because thats done in this class' destructor.
 */
class FileReader
{
public:
	FileReader(string fileName);
	virtual ~FileReader();
	bool readLine(string* target);

private:
	ifstream input;

};

}

#endif /* FILEREADER_H_ */
