#pragma once
#include <iostream>
#include <chrono>

// ����� � �������������
// 0.001 (�) = 1 (��)

class Time final
{
private:
	static double minUpdateDuration;
	static double deltaTime;

	static std::chrono::_V2::system_clock::time_point previousTime;
	static std::chrono::_V2::system_clock::time_point currentTime;

//	static std::chrono::steady_clock::time_point previousTime;
//	static std::chrono::steady_clock::time_point currentTime;

public:
	Time() = delete;
	~Time() = delete;
	Time(const Time&) = delete;
	Time& operator = (const Time&) = delete;

	static void setUpdateFrequency(const double fps);
	static void start();
	static void update();

	static bool CheckFPS();

	static double getDeltaTime();
	static double getTime();
};