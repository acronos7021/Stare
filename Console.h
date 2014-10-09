#pragma once
#include <vector>
#include <string>

using namespace std;

class Console
{
public:
	Console();
	~Console();

	void MessageLoop(int argc, char *argv[]);
};

