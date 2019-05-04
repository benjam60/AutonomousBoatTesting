//============================================================================
// Name        : AutonomousBoatTesting.cpp
// Author      : Ben
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
//#include "catch.hpp"
//
//int main() {
//	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
//	return 0;
//}

#include <stdio.h>
#include <string.h>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "TestGPS.cpp"
#include "../GPSParser.cpp"


TEST_CASE("Factorials are computed", "[factorial]" ) {
	const char * nmeaSentence = "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20";
	TestGPS * testGPS = new TestGPS(nmeaSentence);
	GPSParser gpsParser(testGPS);

	float expectedLatitude = 5107.0017737f;

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextNmeaSentence();
	float actualLatitude = actualGPSCoordinates.latitude;
    REQUIRE(actualLatitude ==  expectedLatitude);

}
