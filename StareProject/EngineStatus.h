#pragma once

#include <chrono>
#include <mutex>
#include "DataStructs.h"



class EngineStatus
{

private:
	int clientID;
	int percentComplete;
	CompareResult cr;
	std::chrono::system_clock::time_point timeout = std::chrono::system_clock::now();
	//static std::mutex myLock;
public:

	EngineStatus(int ClientID);

	~EngineStatus();

	const int getClientID();
	const int getPercentComplete();
	const CompareResult getResult();
	void setPercentComplete(int percent);
	void setResult(CompareResult CR);
	void setTimeout();
	bool isTimedOut();


};

