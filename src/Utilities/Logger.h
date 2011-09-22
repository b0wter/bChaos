/*
 * Logger.h
 *
 *	The logger writes all the messages the user should not see into a log file.
 *	Since we are on linux this is not really neccessary as all our debug/error/warning
 *	output will be sent to the error stream which can be directly redirected to a
 *	file of the users choice.
 *
 *  Created on: 31 Jan 2011
 *      Author: b0wter
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Utils.h"

/*
 *
 */
class Logger
{
public:
	static Logger* Instance();
	bool openLogFile(std::string logFile);
	void writeToLogFile(std::string text);
	void closeLogFile();

private:
	Logger();
	virtual ~Logger();
	Logger(Logger const&);				// make copy constructor private
	Logger& operator=(Logger const&);	// make assignement operator private
	static Logger* instance_;
	std::ofstream output;
	static bool isOpen_;
};

#endif /* LOGGER_H_ */
