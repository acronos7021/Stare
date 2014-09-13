#pragma once

using namespace std;

#include <iostream>
#include <vector>
#include "DataStructs.h"

// For testing purposes, the command line arguments to be passed to the debug console by
// Menu:project -> StareProject Properties -> Configuration Properties  ->  Debugging  -> Command Arguments


//  Takes the command line arguments and uses it to load the MetaData struct.
class CMDparser
{
public:
	static MetaData parseCMD(int argc, char *argv[]);
	
};

