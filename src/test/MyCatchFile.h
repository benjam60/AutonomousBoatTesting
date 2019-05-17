#ifndef MYCATCHFILE_CPP
#define MYCATCHFILE_CPP

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "GPSParser.cpp"
#include "test/TestGPS.cpp"


const int MaxNumNMEASentencesAllowed = 10;
const int maxLengthOfSetence = 150;
typedef char NmeaMessageArray[MaxNumNMEASentencesAllowed][maxLengthOfSetence];
struct GPSCoordinates actualGPSCoordinates;



GPSParser createGPSParser(const char * args[], const int numArgs) {
	NmeaMessageArray messages;
	for (int i = 0; i < numArgs; ++i) { strcpy(messages[i], args[i]); }
	TestGPS * testGPS = new TestGPS(messages, numArgs);
	GPSParser testgps(testGPS);
	return testgps;
}
#endif
