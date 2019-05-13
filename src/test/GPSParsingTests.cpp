//============================================================================
// Name        : AutonomousBoatTesting.cpp
// Author      : Ben
// Version     :
// Copyright   : Your copyright notice
// Description : Testing my boat's gps parsing
//============================================================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "catch.hpp"
#include <test/TestGPS.cpp>
#include <GPSParser.cpp>

//check for valid chars as well as valid #s...e.g. number range as well
//TODO: feed the gps lots of crap data and try to get it to segfault


//Latitude

TEST_CASE("Get latitude", "[GPSParser]" ) {
	typedef char NmeaMessageArray[1][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 1);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();

    REQUIRE(5107.0017737f == actualGPSCoordinates.latitude);
}

TEST_CASE("Ignore messages with missing latitude", "[GPSParser]" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray badAndThenGoodInput;
	strcpy(badAndThenGoodInput[0], "$GPRMC,144326.00,A,,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	strcpy(badAndThenGoodInput[1], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(badAndThenGoodInput, 2);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();

    REQUIRE(5107.0017737f == actualGPSCoordinates.latitude);
}

TEST_CASE("Only parse messages for longitude with messages that begin with $GPRMC", "[GPSParser]" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray badAndThenGoodInput;
	strcpy(badAndThenGoodInput[0], "$GPGLL,3907.360,N,12102.481,W,183730,A*33");
	strcpy(badAndThenGoodInput[1], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(badAndThenGoodInput, 2);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();

    REQUIRE(5107.0017737f == actualGPSCoordinates.latitude);
}

TEST_CASE("handle incomplete $GPRMC message", "[GPSParser]" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray incompleteMessage;
	strcpy(incompleteMessage[0], "$GPRMC,144326.00,A,4007.0017737,N,11402.3291611,W,0.080,323.3");
	strcpy(incompleteMessage[1], "$GPRMC,144326.00,A,4107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(incompleteMessage, 2);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();

    REQUIRE(4107.0017737f == actualGPSCoordinates.latitude);
}


//Longitude

TEST_CASE("get longitude from $GPRMC message", "[GPSParser]" ) {
	typedef char NmeaMessageArray[1][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 1);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	REQUIRE(11402.3291611f == actualGPSCoordinates.longitude);
}



//Bad messages
//TEST_CASE("if the message status is V then we should ignore", "[GPSParser]" ) {
//	typedef char NmeaMessageArray[1][150];
//	NmeaMessageArray messages;
//	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
//	TestGPS * testGPS = new TestGPS(messages, 1);
//	GPSParser gpsParser(testGPS);
//
//	struct GPSCoordinates actualGPSCoordinates;
//	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
//	REQUIRE(11402.3291611f == actualGPSCoordinates.longitude);
//}


//Random messages
TEST_CASE("should not segfault on random characters" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray messages;

	printf("Random sequence of numbers to be converted to string:\n");
	for (int i = 0; i < 149; ++i) {
		int randNumber = rand() % 256;
		char letter = (char)randNumber;
		printf("%i, ", randNumber);
		messages[0][i] = letter;
	}
	messages[0][149] = '\0';
	strcpy(messages[1], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 2);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
	REQUIRE(11402.3291611f == actualGPSCoordinates.longitude);
}

TEST_CASE("get north direction" ) {
	typedef char NmeaMessageArray[1][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 1);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();

    REQUIRE('N' == actualGPSCoordinates.northOrSouth);
}

TEST_CASE("get south direction" ) {
	typedef char NmeaMessageArray[1][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,S,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 1);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();

    REQUIRE('S' == actualGPSCoordinates.northOrSouth);
}

TEST_CASE("No North or south direction on first message" ) {
	typedef char NmeaMessageArray[2][150];
	NmeaMessageArray messages;
	strcpy(messages[0], "$GPRMC,144326.00,A,5107.0017737,q,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	strcpy(messages[1], "$GPRMC,144326.00,A,5107.0017737,S,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
	TestGPS * testGPS = new TestGPS(messages, 2);
	GPSParser gpsParser(testGPS);

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();

    REQUIRE('S' == actualGPSCoordinates.northOrSouth);
}


//TEST_CASE("should not parse longitude or latitude if non-alphanumeric characters" ) {
//	typedef char NmeaMessageArray[2][150];
//	NmeaMessageArray messages;
//
//	for (int i = 0; i < 149; ++i) {
//		int randNumber = rand() % 256;
//		char letter = (char)randNumber;
//		printf("%i, ", randNumber);
//		messages[0][i] = letter;
//	}
//	messages[0][149] = '\0';
//	strcpy(messages[1], "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20");
//	TestGPS * testGPS = new TestGPS(messages, 2);
//	GPSParser gpsParser(testGPS);
//
//	float expectedLongitude = 11402.3291611f;
//	struct GPSCoordinates actualGPSCoordinates;
//	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
//	float actualLongitude = actualGPSCoordinates.longitude;
//	REQUIRE(actualLongitude ==  expectedLongitude);
//}