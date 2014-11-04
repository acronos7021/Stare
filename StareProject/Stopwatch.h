#pragma once
#include<chrono>
#include<ctime>

enum stopwatchState { running, stopped, clear };

class Stopwatch
{
public:
	Stopwatch();
	~Stopwatch();
	void start();
	void end();
	int getTimeInMicroseconds();

private:
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
	stopwatchState swState;
};