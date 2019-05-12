//============================================================================
// Name        : AutonomousBoatTesting.cpp
// Author      : Ben
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <stdio.h>
#include <string.h>
#include "catch.hpp"
#include "test/TestGPS.cpp"
#include "GPSParser.cpp"

//check for valid chars as well as valid #s...e.g. number range as well
TEST_CASE("Get latitude", "[GPSParser]" ) {
	typedef char NmeaMessageArray[1][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 1);
	GPSParser gpsParser(testGPS);

	float expectedLatitude = 5107.0017737f;

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLatitude = actualGPSCoordinates.latitude;

    REQUIRE(actualLatitude ==  expectedLatitude);
}

TEST_CASE("Ignore messages with missing latitude", "[GPSParser]" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray badAndThenGoodInput;
	strcpy(badAndThenGoodInput[0], "$GPRMC,144326.00,A,,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	strcpy(badAndThenGoodInput[1], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(badAndThenGoodInput, 2);
	GPSParser gpsParser(testGPS);

	float expectedLatitude = 5107.0017737f;

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLatitude = actualGPSCoordinates.latitude;

    REQUIRE(actualLatitude ==  expectedLatitude);
}


