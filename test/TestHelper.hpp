#pragma once

#include <chrono>
#include <list>

struct TestHelper{
	static int randInt(int a, int b);

	static float randFloat(float a, float b);

	static double randDouble(double a, double b);

	class Timer{
		using Clock = std::chrono::high_resolution_clock;
		using Units = std::chrono::milliseconds;

		using TimePoint = std::chrono::time_point<Clock, Units>;
		using Duration = std::chrono::duration<long long, std::milli>;

		TimePoint _start;

		Duration _dt;
		std::list<Duration> _log;

		TimePoint _now();

	public:
		Timer();

		void start();
		void stop();

		void clear();

		long long delta();
	};
};