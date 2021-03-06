#ifndef GPSPARSINGTESTS_CPP
#define GPSPARSINGTESTS_CPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AutoPilot.cpp>
#include <test/TestCompass.cpp>
#include <test/TestRudder.cpp>
#include <OverrideableCompass.cpp>
#include <OverrideableRudder.cpp>
#include "MyCatchFile.h"

//To Reader: If we are testing a bad message, we usually have in our test an input of a bad message, followed by a good message, and we assert
//on the good message.

//check for valid chars as well as valid #s...e.g. number range as well

//Latitude
TEST_CASE("Get latitude", "[GPSParser]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	actualGPSCoordinates = createGPSParser(nmeaMessages, 1).waitAndGetNextPosition();

    REQUIRE(5107.0017737f == actualGPSCoordinates.latitude);
}

TEST_CASE("Ignore messages with missing latitude", "[GPSParser]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A"  ",,"  "N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20",
			                       "$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20" };
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 2).waitAndGetNextPosition();

    REQUIRE(5107.0017737f == actualGPSCoordinates.latitude);
}

TEST_CASE("Only parse messages for longitude with messages that begin with $GPRMC", "[GPSParser]" ) {
	const char * nmeaMessages[] = {"$GPGLL,"  "3907.360,N,12102.481,W,183730,A*33",
								   "$GPRMC,"  "144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 2).waitAndGetNextPosition();

    REQUIRE(5107.0017737f == actualGPSCoordinates.latitude);
}

TEST_CASE("handle incomplete $GPRMC message", "[GPSParser]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,4007.0017737,N,11402.3291611,W,0.080,323.3",
			                       "$GPRMC,144326.00,A,4107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 2).waitAndGetNextPosition();

    REQUIRE(4107.0017737f == actualGPSCoordinates.latitude);
}

//Longitude
TEST_CASE("get longitude from $GPRMC message", "[GPSParser]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,N,"  "11402.3291611"  ",W,0.080,323.3,210307,0.0,E,A*20" };

	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 1).waitAndGetNextPosition();
	REQUIRE(11402.3291611f == actualGPSCoordinates.longitude);
}

//Bad messages
TEST_CASE("if the message status is V then we should ignore", "[GPSParser]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,"  "V"  ",5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20",
								  "$GPRMC,144326.00,"  "A"  ",5107.0017737,N,10002.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 2).waitAndGetNextPosition();
	REQUIRE(10002.3291611f == actualGPSCoordinates.longitude);
}

////Random messages
//TEST_CASE("should not segfault on random characters" ) {
//	NmeaMessageArray messages;
//
//	printf("Random sequence of numbers to be converted to string:\n");
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
//	struct GPSCoordinates actualGPSCoordinates;
//	actualGPSCoordinates = gpsParser.waitAndGetNextPosition();
//	REQUIRE(11402.3291611f == actualGPSCoordinates.longitude);
//}

TEST_CASE("get north direction" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 1).waitAndGetNextPosition();

    REQUIRE('N' == actualGPSCoordinates.northOrSouth);
}

TEST_CASE("get south direction" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,S,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 1).waitAndGetNextPosition();

    REQUIRE('S' == actualGPSCoordinates.northOrSouth);
}

TEST_CASE("No North or south direction on first message" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,"  ""  ",11402.3291611,W,0.080,323.3,210307,0.0,E,A*20",
	    						   "$GPRMC,144326.00,A,5107.0017737," "S"  ",11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 2).waitAndGetNextPosition();

    REQUIRE('S' == actualGPSCoordinates.northOrSouth);
}

TEST_CASE("Missing North or South on first message" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,"  "q"  ",11402.3291611,W,0.080,323.3,210307,0.0,E,A*20",
								   "$GPRMC,144326.00,A,5107.0017737,"  "S"  ",11402.3291611,W,0.080,323.3,210307,0.0,E,A*20" };
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 2).waitAndGetNextPosition();

    REQUIRE('S' == actualGPSCoordinates.northOrSouth);
}

TEST_CASE("get west direction" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,S,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 1).waitAndGetNextPosition();

    REQUIRE('W' == actualGPSCoordinates.westOrEast);
}

TEST_CASE("get east direction" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,S,11402.3291611,E,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 1).waitAndGetNextPosition();

    REQUIRE('E' == actualGPSCoordinates.westOrEast);
}

TEST_CASE("Missing West or East on first message" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,S,11402.3291611,,0.080,323.3,210307,0.0,E,A*20",
								   "$GPRMC,144326.00,A,5107.0017737,S,11402.3291611,E,0.080,323.3,210307,0.0,E,A*20"};
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 2).waitAndGetNextPosition();

    REQUIRE('E' == actualGPSCoordinates.westOrEast);
}

TEST_CASE("missing latitude and then missing longitude and then valid message") {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A"  ",,"  "N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20",
			                       "$GPRMC,144326.00,A,5107.0017737,N"  ",,"  "W,0.080,323.3,210307,0.0,E,A*20",
			                       "$GPRMC,144326.00,A,80.0017737,S,90.3291611,E,0.080,323.3,210307,0.0,E,A*20" };
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 3).waitAndGetNextPosition();
    REQUIRE('S' == actualGPSCoordinates.northOrSouth);
    REQUIRE(80.0017737f == actualGPSCoordinates.latitude);
	REQUIRE(90.3291611f == actualGPSCoordinates.longitude);
}

TEST_CASE("missing latitude and then missing longitude with wrong number of commas and then valid message") {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A"  ",,"  "N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20",
			                       "$GPRMC,144326.00,A,5107.0017737,N"  ","  "W,0.080,323.3,210307,0.0,E,A*20", //missing comma compared to above test
			                       "$GPRMC,144326.00,A,80.0017737,S,90.3291611,E,0.080,323.3,210307,0.0,E,A*20" };
	struct GPSCoordinates actualGPSCoordinates;
	actualGPSCoordinates = createGPSParser(nmeaMessages, 3).waitAndGetNextPosition();
    REQUIRE('S' == actualGPSCoordinates.northOrSouth);
    REQUIRE(80.0017737f == actualGPSCoordinates.latitude);
	REQUIRE(90.3291611f == actualGPSCoordinates.longitude);
}

TEST_CASE("reading multiple valid messages in a row and make sure they are all good") {


}

//test no message status

//Unusual and bad input
TEST_CASE("Weird message", "[GPSParser]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,5107.0017737,N,11402.3291611,W,0.080,323.3,210307,0.0,E,A*20"};
	actualGPSCoordinates = createGPSParser(nmeaMessages, 1).waitAndGetNextPosition();

    REQUIRE(5107.0017737f == actualGPSCoordinates.latitude);
}

//write a test with 3 messages, one is missing latitude, next is missing longitude, and the third one is valid

//run tests with many nmea messages

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


TEST_CASE("Rudder should not move since we are heading in current direction", "[Autopilot]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,0.0000000,N,00.0000000,E,0.080,323.3,210307,0.0,E,A*20"};
	float heading[] = {90.0f};
	TestRudder * rudder = new TestRudder;
	GPSParser gpsParserIn = createGPSParser(nmeaMessages, 1);
	AutoPilot pilot(&gpsParserIn, new TestCompass(heading, 1), rudder);
	GPSCoordinates waypoint(0.0000000f, 1.000f, 'N', 'E');
	pilot.driveToWaypoint(waypoint);
	int straightAheadInDegreesForRudder = 90;
	int expected = rudder->recordedPositions[0];
	REQUIRE(expected == straightAheadInDegreesForRudder);
}


TEST_CASE("Boat is facing 45 degrees more than waypoint so rudder should angle correct error by half", "[Autopilot]" ) {
	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,0.0000000,N,00.0000000,W,0.080,323.3,210307,0.0,E,A*20"};
	float heading[] = {90.0f};
	TestRudder * rudder = new TestRudder;
	GPSParser gpsParserIn = createGPSParser(nmeaMessages, 1);
	AutoPilot pilot(&gpsParserIn, new TestCompass(heading, 1), rudder);
	GPSCoordinates waypoint(1.0f, 1.0f, 'N', 'E');
	pilot.driveToWaypoint(waypoint);
	int * expected = rudder->recordedPositions;
	REQUIRE(expected[0] == (90 + 22));
}


//moving in the wrong direction
//TEST_CASE("Rudder should not move since we are heading in current direction", "[Autopilot]" ) {
//	const char * nmeaMessages[] = {"$GPRMC,144326.00,A,0.0000000,N,00.0000000,W,0.080,323.3,210307,0.0,E,A*20"};
//	float heading[] = {90.0f};
//	TestRudder * rudder = new TestRudder;
//	GPSParser gpsParserIn = createGPSParser(nmeaMessages, 1);
//	AutoPilot pilot(&gpsParserIn, new TestCompass(heading, 1), rudder);
//	pilot.driveToWaypoint(0.0000000f, 'N', 1.000f, 'E');
//	int straightAheadInDegreesForRudder = 90;
//	int expected = rudder->recordedPositions[0];
//	REQUIRE(expected == straightAheadInDegreesForRudder);
//}


#endif
