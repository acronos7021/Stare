#pragma once
#include<vector>
#include<string>
#include "CMDparser.h"

using namespace std;

class Console
{
public:
	Console();
	~Console();

	void MessageLoop(CMDparser &cmdParser, int argc, char *argv[]);
};

