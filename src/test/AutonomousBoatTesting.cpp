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
//TODO: feed the gps lots of crap data and try to get it to segfault


//Latitude

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

TEST_CASE("Only parse messages for longitude with messages that begin with $GPRMC", "[GPSParser]" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray badAndThenGoodInput;
	strcpy(badAndThenGoodInput[0], "$GPGLL,3907.360,N,12102.481,W,183730,A*33");
	strcpy(badAndThenGoodInput[1], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(badAndThenGoodInput, 2);
	GPSParser gpsParser(testGPS);

	float expectedLatitude = 5107.0017737f;

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLatitude = actualGPSCoordinates.latitude;

    REQUIRE(actualLatitude ==  expectedLatitude);
}

TEST_CASE("handle incomplete $GPRMC message", "[GPSParser]" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray incompleteMessage;
	strcpy(incompleteMessage[0], "$GPRMC,144326.00,A,4007.0017737,N,11402.3291611,W,0.080,323.3");
	strcpy(incompleteMessage[1], "$GPRMC,144326.00,A,4107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(incompleteMessage, 2);
	GPSParser gpsParser(testGPS);

	float expectedLatitude = 4107.0017737f;

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLatitude = actualGPSCoordinates.latitude;

    REQUIRE(actualLatitude ==  expectedLatitude);
}


//Longitude

TEST_CASE("get longitude from $GPRMC message", "[GPSParser]" ) {
	typedef char NmeaMessageArray[1][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 1);
	GPSParser gpsParser(testGPS);

	float expectedLongitude = 11402.3291611f;
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLongitude = actualGPSCoordinates.longitude;
	REQUIRE(actualLongitude ==  expectedLongitude);
}



//Bad messages
TEST_CASE("if the message status is V then we should ignore", "[GPSParser]" ) {
	typedef char NmeaMessageArray[1][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 1);
	GPSParser gpsParser(testGPS);

	float expectedLongitude = 11402.3291611f;
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	float actualLongitude = actualGPSCoordinates.longitude;
	REQUIRE(actualLongitude ==  expectedLongitude);
}


//TEST_CASE("handle invalid character e.g. control characters in middle of message", "[GPSParser]" ) {
//	typedef char NmeaMessageArray[2][150];
//	NmeaMessageArray incompleteMessage;
//	strcpy(incompleteMessage[0], "$GPRMC,144326.00,A,4\x01\x02\x03\x04,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
//	strcpy(incompleteMessage[1], "$GPRMC,144326.00,A,4107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
//	TestGPS * testGPS = new TestGPS(incompleteMessage, 2);
//	GPSParser gpsParser(testGPS);
//
//	float expectedLatitude = 4107.0017737f;
//
//	struct GPSCoordinates actualGPSCoordinates;
//	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
//	float actualLatitude = actualGPSCoordinates.latitude;
//
//    REQUIRE(actualLatitude ==  expectedLatitude);
//}




