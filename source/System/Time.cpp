#include "Time.h"

void Time::setUpdateFrequency(const double fps)
{
	minUpdateDuration = 1000.0 / fps;
}

void Time::start()
{
	auto start = std::chrono::high_resolution_clock::now();
	
	previousTime = std::chrono::high_resolution_clock::now();
}

void Time::update()
{
	currentTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration<double, std::milli>(currentTime - previousTime).count();
}

bool Time::CheckFPS()
{
	if (deltaTime >= minUpdateDuration)
	{
		previousTime = currentTime;
		return true;
	}
	else
	{
		return false;
	}
}

double Time::getDeltaTime() { return deltaTime; }

double Time::getTime() { return std::chrono::duration<double, std::milli>(std::chrono::system_clock::now().time_since_epoch()).count(); }

std::chrono::_V2::system_clock::time_point Time::previousTime;
std::chrono::_V2::system_clock::time_point Time::currentTime;
double Time::minUpdateDuration;
double Time::deltaTime;
