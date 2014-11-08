#pragma once
#include<vector>
#include<string>
#include "CMDparser.h"

using namespace std;

class Console
{
public:
	Console(CMDparser TheCMDparser);
	~Console();


	CMDparser cmdParser;

	void MessageLoop(int argc, char *argv[]);
};

