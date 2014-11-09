#include "EngineStatus.h"

EngineStatus::EngineStatus(int ClientID)
{
	clientID = ClientID;
	percentComplete = 0;
}

EngineStatus::~EngineStatus()
{

}

// can only be read so not synchronized
const int EngineStatus::getClientID() 
{ 
	return clientID; 
}

const int EngineStatus::getPercentComplete() 
{ 
	int ret;
	//statusLock.lock();
	ret = percentComplete;
	//statusLock.unlock();
	return ret; 
}

const CompareResult EngineStatus::getResult() 
{
	CompareResult ret;
	//statusLock.lock();
	ret = cr;
	//statusLock.unlock();
	return ret;
}

void EngineStatus::setPercentComplete(int percent) 
{ 
	//statusLock.lock();
	percentComplete = percent;
	//statusLock.unlock();
}

void EngineStatus::setResult(CompareResult CR) 
{ 
	//statusLock.lock();
	cr = CR; 
	//statusLock.unlock();
}

void EngineStatus::setTimeout()
{ 
	//statusLock.lock();
	timeout = std::chrono::system_clock::now(); 
	//statusLock.unlock();
}

bool EngineStatus::isTimedOut()
{
	//statusLock.lock();
	std::chrono::system_clock::time_point timeoutTime = timeout;
	//statusLock.unlock();
	// set for 20 second timeout
	timeoutTime += std::chrono::seconds(20);

	if (timeoutTime > std::chrono::system_clock::now())
		return false;
	else
		return true;
}

