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
	
	PHPsocket(CMDparser* cmd); 
	~PHPsocket();
 	
	Json::Value parseJSON(std::string json);
	std::string doCompare(Json::Value json);
	Json::Value formCheckCompareReturn(int status);
	std::string doCheckCompare(Json::Value json);
    std::string doLearn(Json::Value json);
	std::string jsonDecoder(std::string json);
	std::string getStyles();
	std::string doCreate(Json::Value json);
	std::string sanitize(std::string);
	std::string desanitize(std::string);
	
private:
	CMDparser* cmd;
};

