/*
 * Utils.cpp
 *
 *  Created on: 18 Jan 2011
 *      Author: b0wter
 */

#include "Utils.h"

namespace Utilities
{

//! Reads a matrix from a file. Needed for the harmonic force.
bool matrixFromFile(MatrixXd* targetMatrix, string fileName)
{
	ENTER 	;
	FileReader* reader = new FileReader(fileName);
	string line;
	vector<string> lines;

	while(reader->readLine(&line))
	{
		if(line != "" && line != "\n" && line.substr(0,1) != "#")
			lines.push_back(line);
	}

	// check if any lines were actually read from the file
	if(lines.size() == 0)
	{
		WARNING("No input read from the matrix file: " + fileName + " (wrong filename?)")
		LEAVE
		return false;
	}
	// now, that the text is read, create an appropriate matrix
	unsigned int dim = (unsigned int)sqrt(lines.size());
	(*targetMatrix) = MatrixXd::Zero(dim,dim);

	// fill the matrix with data
	for(unsigned int i = 0; i < dim; i++)
		for(unsigned int j = 0; j < dim; j++)
			(*targetMatrix)(i,j) = doubleFromString(lines[i*dim + j]);

	delete(reader);
	LEAVE	;
	return true;
}

/*!
 * Removes all directories from a given filename by simply searching the
 * last position of '/' and cropping all that comes earlier.
 */
string removePathFromFileName(string fileName)
{
	unsigned int i = fileName.rfind('/') + 1;
	return fileName.substr(i);
}

/*!
 * Performs some manipulations on the function's argument (e.g. remove the unneccessary information
 * from vectors)
 */
string cleanFunctionNameAndArgument(string functionName)
{
	unsigned int index;
	bool wasDirty = true; // assume the function needs some cleanup

	/*
	 * The loop checks the at least one time to search for something to clean
	 * if it does clean something the loop has to run a another time to make sure there is
	 * nothing more to clean.
	 */
	while(wasDirty)
	{
		wasDirty = false;
		// Clean vectors as arguments
		index = functionName.find_first_of('(');
		if(functionName.find("vector<", index) != string::npos)
		{
			wasDirty = true;
			// delete everything from the first '<' after the std::vector to the first '> > >' as it is most likely the
			// closing of it's definition.
			unsigned int positionBracket = functionName.find('<', index);
			unsigned int positionComma = functionName.find("> > >", index);
			functionName.erase(positionBracket, positionComma - positionBracket + 7);
		}

	}
	return functionName;
}

/**
 * Benutzung: valueFromString<Datentyp>("Variable, die den Wert erhalten soll", String, Konvertierungstyp (i.d.R. std::dec)
 */
template <class T>
bool valueFromString(T& t, const string& s, ios_base& (*f)(ios_base&) = std::dec)
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

/*
 * Converts a string to an int
 */
int intFromString(string s)
{
	istringstream stream(s);
	int i;
	stream >> i;
	return i;
}

unsigned int uintFromString(string s)
{
	istringstream stream(s);
	unsigned int i;
	stream >> i;
	return i;
}

double doubleFromString(string s)
{
	istringstream stream(s);
	double d;
	stream >> d;
	return d;
}

//double doubleFromString(string s)
//{
//	istringstream stream(s);
//	double f;
//	stream >> f;
//	return f;
//}

/*
 * Konvertiert eine Zeichenfolge "x,y,z" in einen Vektor
 */
Vector vectorFromString(string s)
{
	Vector vec;

	// Füge weitere Kommata ein, damit das parsen generalisiert werden kann
	s.append(",");
	s.insert(0,",");

	// Legt die Indizes fest, current wird auf 0 gesetzt, weil der String jetzt mit einem Semikolon beginnt
	// und next auf das nächste gefundene Semikolon
	size_t current = s.find(","); size_t next = s.find(",",1);

	// Prüfe Gültigkeit von next
	if(next == string::npos)
		throw "[ERROR] vectorFromString(string s) - could not find enough commas to parse string";

	double coordinates[3];
	unsigned int index = 0;

	// Werte parsen
	while(next != string::npos)
	{
		// Wert auslesen
		//valueFromString<double>(coordinates[index], s.substr(current + 1, (next - (current + 1))), std::dec);
		coordinates[index] = doubleFromString(s.substr(current + 1, (next - (current + 1))));
		// Indizes neu setzen
		current = next;
		next = s.find(",", next+1);

		//if(next == string::npos)
			//break;

		index++;
	}

	vec[0] = coordinates[0];
	vec[1] = coordinates[1];
#ifdef THREE_DIMENSIONS
	vec[2] = coordinates[2];
#endif
	// 3D Umstellung: hier muß auch noch die Z-Koordinate gesetzt werden

	return vec;
}

string vectorToString(Vector* v)
{
	stringstream oss;
	oss << "[" << (*v)[0] << "," << (*v)[1] << "]";
	return oss.str();
}

bool valueAndIdentifierFromString(string source, string* value, string* identifier)
{
	// Check if there is exactle 1 '=' and exactly 1 ';'
	// 1. Check if there is more than one occurance by comparing the first und last position of the characters
	// 2. Check if the position of the characters is != string::npos
	//
	// Code is lengthenend for better readability
	//
	size_t firstEqual, lastEqual, firstSemiColon, lastSemiColon;

	firstEqual = source.find_first_of('=');
	lastEqual = source.rfind(';');

	firstSemiColon = source.find_first_of(';');
	lastSemiColon = source.rfind(';');

	if(firstEqual == lastEqual
			&& firstSemiColon == lastSemiColon
			&& firstEqual != string::npos
			&& firstSemiColon != string::npos)
	{

		return true;
	}

	return false;
}

void vectorToString(string* target_string, Vector vec)
{
	ostringstream oss;
	oss << vec[0] << "," << vec[1];
	*target_string = oss.str();
}

equationSolverType equationSolverFromString(string s)
{
	if(s == "euler")
		return equationSolverTypeEuler;
	if(s == "vv")
		return equationSolverTypeVelocityVerlet;
	else
		return equationSolverTypeUnknown;
}

string equationSolverTypeName(equationSolverType t)
{
	if(t == 0)
		return "Unknown";
	if(t == equationSolverTypeEuler)
		return "Euler";
	if(t == equationSolverTypeVelocityVerlet)
		return "Velocity-Verlet";
	return "Unknown";
}

boundaryConditionType boundaryConditionFromString(string s)
{
	if(s == "none")
		return boundaryConditionTypeNone;
	else if(s == "square")
		return boundaryConditionTypeSquare;
	else if(s == "circle")
		return boundaryConditionTypeCircular;
	else if(s == "outflow")
		return boundaryConditionOutFlow;
	else
		return boundaryConditionTypeUnknown;
}

string boundaryConditionToString(boundaryConditionType t)
{
	switch((int)t)
	{
	case (int)boundaryConditionTypeNone:
		return string("None");
		break;
	case (int)boundaryConditionTypeCircular:
		return string("Circular");
		break;
	case (int)boundaryConditionTypeSquare:
		return string("Square");
		break;
	case (int)boundaryConditionOutFlow:
		return string("Outflow");
		break;
	default:
		return string("Unknown");
		break;
	}
}

} // end of namespace
