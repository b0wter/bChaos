/*
 * Logger.cpp
 *
 *  Created on: 31 Jan 2011
 *      Author: b0wter
 */

#include "Logger.h"
#include "Utils.h"

using namespace Utilities;

Logger* Logger::instance_ = NULL;
bool Logger::isOpen_ = false;

Logger::Logger()
{
	// TODO Auto-generated constructor stub

}

Logger::~Logger()
{
	if(isOpen_)
		Logger::closeLogFile();
}

Logger* Logger::Instance()
{
	if(!Logger::instance_)
		instance_ = new Logger;

	return instance_;
}

bool Logger::openLogFile(std::string fileName)
{
	try
	{
		output.open(fileName.c_str());
		Logger::isOpen_ = true;
	}
	catch(exception) { ERROR("Logger could not open logfile (" << fileName << ").") }
	return true;
}

void Logger::closeLogFile()
{
	if(output.is_open())
		output.close();
}

void Logger::writeToLogFile(std::string text)
{
	if(output.is_open())
		output << text << std::endl;
}
