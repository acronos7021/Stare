#include "Stopwatch.h"

Stopwatch::Stopwatch()
{
	swState = stopwatchState::clear;
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::start()
{
	if (swState != stopwatchState::running)
	{
		startTime = std::chrono::steady_clock::now();
		swState = stopwatchState::running;
	}
}

void Stopwatch::end()
{
	if (swState == stopwatchState::running)
	{
		endTime = std::chrono::steady_clock::now();
		swState = stopwatchState::stopped;
	}
	else if (swState == stopwatchState::clear)
		throw "Stopwatch has not started yet.";
	else
		throw "Stopwatch has already stopped.";
}

int Stopwatch::getTimeInMicroseconds()
{
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds> (endTime - startTime);
	return (int) elapsed.count();
}