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
	PHPsocket();
	~PHPsocket();

	Json::Value parseJSON(string json);
	string doCompare(Json::Value json);
};

