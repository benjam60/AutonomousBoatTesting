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
	TestGPS * testGPS = new TestGPS;
	GPSParser gpsParser(testGPS);
	const char * s2 = "Hello World";
    REQUIRE(strcmp(gpsParser.waitAndGetNextNmeaSentence(), s2) ==  0);

}
