#pragma once

using namespace std;

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "DataStructs.h"
#include "json.h"
#include "CMDparser.h"

class PHPsocket
{
public:
	
	PHPsocket(CMDparser* cmd) {
		this->cmd = cmd;
	}
	
	~PHPsocket();
 	
	Json::Value parseJSON(string json);
	string doCompare(Json::Value json);
	Json::Value formCheckCompareReturn(int status);
	string doCheckCompare(Json::Value json);
	void jsonDecoder(string json);
	
private:
	CMDparser* cmd;
};

