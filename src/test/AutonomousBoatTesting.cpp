//============================================================================
// Name        : AutonomousBoatTesting.cpp
// Author      : Ben
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <stdio.h>
#include <string.h>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "test/TestGPS.cpp"
#include "GPSParser.cpp"

//check for valid chars as well as valid #s...e.g. number range as well
TEST_CASE("Get latitude", "[GPSParser]" ) {
	char validNmeaWithLatitude[1][150];
	strcpy(validNmeaWithLatitude[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	char ** input = (char **)validNmeaWithLatitude;
	TestGPS * testGPS = new TestGPS(input, 2);
	GPSParser gpsParser(testGPS);

	float expectedLatitude = 5107.0017737f;

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLatitude = actualGPSCoordinates.latitude;

    REQUIRE(actualLatitude ==  expectedLatitude);
}

TEST_CASE("Ignore missing latitude", "[GPSParser]" ) {
	char badAndThenGoodInput[2][150];
	strcpy(&badAndThenGoodInput[0][0], "$GPRMC,144326.00,A,,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	strcpy(&badAndThenGoodInput[1][0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	char ** input = (char **)badAndThenGoodInput;
	TestGPS * testGPS = new TestGPS(input, 2);
	GPSParser gpsParser(testGPS);

	float expectedLatitude = 5107.0017737f;

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLatitude = actualGPSCoordinates.latitude;

    REQUIRE(actualLatitude ==  expectedLatitude);
}


