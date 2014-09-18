#pragma once

using namespace std;

#include <iostream>
#include <vector>
#include "DataStructs.h"

//  Takes the command line arguments and uses it to load the MetaData struct.
class CMDparser
{
public:
	bool parseCMD(vector<string> cmdList);
	void Learn(vector<string> cmdList);
	void Compare(vector<string> cmdList);
	void Create(vector<string> cmdList);

	void Quit(vector<string> cmdList);

	string ReadFile(string fileName);

};

