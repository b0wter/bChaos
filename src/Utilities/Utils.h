/*
 * Utils.h
 *
 *	Defines for the different outputs (INFO, WARNING, ERROR, DEBUG, ENTER, LEAVE)
 *	as well as for some global utility functions (e.g. conversion)
 *
 *  Created on: 24 Dec 2010
 *      Author: b0wter
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <eigen2/Eigen/Eigen>
#include "Logger.h"
#include "FileReader.h"

USING_PART_OF_NAMESPACE_EIGEN

using namespace std;

namespace Utilities
{

/**
 * defines to handle 2 or 3 dimensions
 * #define THREE_DIMENSIONS // toggles EXPERIMENTAL and not fully implemented 3d support.
 * Since the Vector is defined by a typedef no search & replace must be done.
 * The typedef must be placed first in this file since the later part refers to it.
*/
//#define THREE_DIMENSIONS
#ifdef THREE_DIMENSIONS
#define DIMENSIONS 3
typedef Vector3d Vector;
#else
#define DIMENSIONS 2
typedef Vector2d Vector;
#endif

//! Read a matrix from a file
bool matrixFromFile(MatrixXd* targetMatrix, string filename);

//! Creates values from string with a template class, does not work correctly (only converts doubles?!)
template <class T> bool valueFromString(T& t, const string& s, ios_base& (*f)(ios_base&));
//! Create a vector from a string "x,y,z"
Vector vectorFromString(string s);
//! Reads a text line and checks if it is a valid configuration file line.
bool valueAndIdentifierFromString(string source, string* value, string* identifier);
//! Converts a vector to a string "[x,y,z]"
string vectorToString(Vector* v);

//! Removes the path from a filename, useful for the debugging output mechanism which states file name and function name.
string removePathFromFileName(string fileName);
//! Removes long parameters from function names (especially from vectors).
string cleanFunctionNameAndArgument(string functionName);

//! Creates an int from a string. Can be used to check for (numerical) bools with a simple typecast.
int intFromString(string s);
//! Creates an uinsigned int from a string.
unsigned int uintFromString(string s);
//! Creates a double from a string.
double doubleFromString(string s);
//! Creates a double from a string.
double doubleFromString(string s);
//! Writes a vector into a given string.
void vectorToString(string* target_string, Vector* vec);

//! Simple enum, to not save a simple string in SimulationOptions.
enum equationSolverType{equationSolverTypeUnknown, equationSolverTypeEuler, equationSolverTypeVelocityVerlet};
//! Converts a string to an enum.
equationSolverType equationSolverFromString(string s);
//! Converts an enum to a string.
string equationSolverTypeName(equationSolverType t);

//! Enum for the type of boundary condition
/*!
 * Defines the type of boundary conditions used. The size is saved in the simulationOptions. 
 * Note that the size is the radius for circular and outflow boundary conditions and 1/2 of the edge length for square
 * boundary conditions. Outflow boundary condition is treated as distance from the origin (-> circle)
 */
enum boundaryConditionType{boundaryConditionTypeUnknown, boundaryConditionTypeNone, boundaryConditionTypeSquare, boundaryConditionTypeCircular, boundaryConditionOutFlow};
//! Converts a string to an enum.
boundaryConditionType boundaryConditionFromString(string s);
//! Converts an enum to a string.
string boundaryConditionToString(boundaryConditionType t);

// Various defines to toggle specific output
#ifdef DEBUG_INLINE
#define INLINE
#else
#define INLINE inline
#endif

#ifdef DEBUG_OUTPUT
#define DEBUG(s) 	std::cerr << left << "[DBG] " << std::setw(21) << removePathFromFileName(__FILE__) << " : " << right << setw(4) << __LINE__ << " : " << left << s << std::endl;
#else
#define DEBUG(s)
#endif

#ifdef INFO_OUTPUT
#define INFO(s) 	std::cerr << left << "[NFO] " << std::setw(21) << removePathFromFileName(__FILE__) << " : " << right << setw(4) << __LINE__ << " : " << s << std::endl;
#else
#define INFO(s)
#endif

#ifdef WARNING_OUTPUT
#define WARNING(s) 	std::cerr << left << "[WRN] " << std::setw(21) << removePathFromFileName(__FILE__) << " : " << right << setw(4) << __LINE__ << " : " << s << std::endl;
#else
#define WARNING(s)
#endif

#ifdef ERROR_OUTPUT
#define ERROR(s) std::cerr << left << "[ERR] " << std::setw(21) << removePathFromFileName(__FILE__) << " : " << right << setw(4) << __LINE__ << " : " << left << setw(70) << cleanFunctionNameAndArgument(__PRETTY_FUNCTION__) << " : " << s << std::endl;
#else
#define ERROR(s)
#endif

#ifdef FUNCTION_ENTER_LEAVE
#define ENTER		std::cerr << left << "[FNC] " << std::setw(21) << removePathFromFileName(__FILE__) << " : " << right << setw(4) << __LINE__ << " : " << left << setw(70) << cleanFunctionNameAndArgument(__PRETTY_FUNCTION__) << " : Start" << std::endl;
#define LEAVE		std::cerr << left << "[FNC] " << std::setw(21) << removePathFromFileName(__FILE__) << " : " << right << setw(4) << __LINE__ << " : " << left << setw(70) << cleanFunctionNameAndArgument(__PRETTY_FUNCTION__) << " : End" << std::endl;
#else
#define ENTER
#define LEAVE
#endif



} // end of namespace

#endif /* UTILS_H_ */

