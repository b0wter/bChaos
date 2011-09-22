//============================================================================
// Name        : bChaos.cpp
// Author      : b0wter
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "./Simulation/Simulation.h"
#include "./Utilities/Utils.h"

#include <ctime>

using namespace std;

string stringFromTimeDifference(time_t start, time_t end)
{
	unsigned int dif= end - start;
	unsigned int hour, min, sec;

	hour = dif / 3600; dif -= hour * 3600;
	min = dif / 60; dif -= min * 60;
	sec = dif;

	stringstream oss;
	oss << setfill('0');
	oss << setw(2) << hour << ":" << setw(2) << min << ":" <<setw(2) << sec;
	return oss.str();
}

void outputMessageStart(struct tm* tmStart)
{
	cerr << setfill('0');
	cerr << endl << "----------------------{ Beginning simulation at " << setw(2) << tmStart->tm_hour << ":" << setw(2) << tmStart->tm_min << ":" << setw(2) << tmStart->tm_sec << " }----------------------" << endl << endl;
	cerr << setfill(' ');
}

void outputMessageEnd(struct tm* tmEnd, time_t start, time_t end)
{
	cerr << setfill('0');
	cerr << endl << "--------{ Finishing simulation at " << setw(2) << tmEnd->tm_hour << ":" << setw(2) << tmEnd->tm_min << ":" << setw(2) << tmEnd->tm_sec <<
			" -- Total runtime: " << stringFromTimeDifference(start, end) << "}--------" << endl;
	cerr << setfill(' ');
}


int main(int argc, char** argv)
{
	time_t start = time(0);
	struct tm* tmStart = localtime(&start);
	outputMessageStart(tmStart);

	//Logger::Instance()->openLogFile("log.txt");
	//Logger::Instance()->writeToLogFile("Log file successfully opened!");

	DEBUG("Debugging is enabled!")	;
	INFO("Starting new simulation ...")	;
	delete (new SimulationSpace::Simulation(argc,argv));

	//Logger::Instance()->closeLogFile();

	time_t end = time(0);
	struct tm* tmEnd = localtime(&end);
	outputMessageEnd(tmEnd, start, end);

	return 0;
}

/*! \mainpage bChaos index page
 *	\section Introduction
 *	bChaos is a simple molecular dynamics simulator that is mainly used for researching quantum chaos in many body systems where the bodies are coupled by a harmonic potential (springs).
 *	It does also offers computation of coulomb- und gravitational potentials. More are likely to be added.
 *
 *	\section Usage
 *	bChaos requires a config file that is supplied as a command line parameter, e.g.:  
 *	<tt> $ ./bChaos -c=config.txt</tt> 
 *	which holds all neccessary simulation paramters as well as a list of particles.
 *
 *	\section Config-file
 *	This section gives an overview of the possible options in the config file. All options have a default value hardcoded that is used if nothing else specified.
 *	Usually the hardcoded values are designed to simply deactivate the feature (e.g. boundary conditions). The code listing below includes the data type.
 *	An example file will be shown at the end of the listing. Note that if the data type is an enum or similar you can click on it for a more detailed description.
 *	Bool values can only be entered as 0 for \c false and any other number for \c true
 *
 *  \code
 * // data type              // option name
 * unsigned int              timeStepNo
 * double                    timeStepSize
 * equationSolverType        equationSolver
 * boundaryConditionType     boundaryCondition
 * double                    boundaryConditionSize
 * bool                      outputParticleData
 * bool                      outputCloudData
 * bool                      saveParticleHistory
 * bool                      saveCloudHistory
 * bool                      globalGravity
 * double                    globalGravityAcceleration
 * Vector                    globalGravityDirection
 * bool                      coulombForce
 * bool                      gravitationalForce
 * bool                      harmonicForce
 * bool 					 diagonalizedHarmonicForce
 * string                    harmonicInteractionMatrix
 *
 *	\endcode
 *	\n
 *	Particles are read from the same configuration file. They too have a couple of parameters:\n
 *	\n
 *	<tt>(particle)p=0,0; r=1; m=1; c=0; v=0,0; t=Cloud01;</tt>\n
 *	\n
 *	The parameters are: \n
 *	\li <b> (particle)</b> - is for the parser. It indicates that this line does not contain configuration info but a particle description.
 *	\li \b p - is the position, entered as x,y(,z)
 *	\li \b r - is the radius. (will be used for the not yet implemented collision detection and scattering) Entered as a double, e.g. 0.1e-2, or 1.5
 *	\li \b m - is the mass. Entered as a double.
 *	\li \b c - is the charge. Used for the coloumb force and entered as a double.
 *	\li \b v - is the initial velocity, entered as x,y(,z).
 *	\li \b t - is the tag. Every particle belongs to a cloud. All particles sharing the same tag will be combined to a cloud. This offers easy computation of e.g. center of mass motion/velocities.
 *
 *	\section Example
 *	A rather simple simulation shows the time evolution of a 4 body systems. It uses only gravitational forces and the bodies roughly translate to bodies found in our solar systems.\n
 *	(black = Sun, red = Earth, green = Jupiter, blue = comet Halley)\n
 *	\n
 *	<b>Config-file:</b>
 *	\code
 * [config]
 * timeStepNo=71233;
 * timeStepSize=0.015;
 * equationSolver=vv;
 * gravitationalForce=1;
 * outputParticleData=1;
 * boundaryCondition=none;
 *
 * [entities]
 * (particle)p=0,0; r=1; m=1; c=0; v=0,0; t=Sonne;
 * (particle)p=0,1; r=1; m=3.0e-6; c=0; v=-1,0; t=Erde;
 * (particle)p=0.0,5.36; r=1; m=9.55e-4; c=0; v=-0.425,0; t=Jupiter;
 * (particle)p=34.75,0; r=1; m=1e-14;  c=0; v=0,0.0296; t=Halley;
 *
 *	\endcode
 *	\n
 *	<b>Result:</b>
 *	\image html 4koerper.png
 *	Plot was made using xmgrace with default settings.
 */
