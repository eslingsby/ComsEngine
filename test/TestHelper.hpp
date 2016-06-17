#pragma once

#include <chrono>
#include <list>
#include <random>
#include <cstdint>

struct TestHelper{
	inline static int randInt(int a, int b);

	inline static float randFloat(float a, float b);

	inline static double randDouble(double a, double b);

	class Timer{
		using Clock = std::chrono::high_resolution_clock;
		using Units = std::chrono::milliseconds;

		using TimePoint = std::chrono::time_point<Clock, Units>;
		using Duration = std::chrono::duration<long long, std::milli>;

		TimePoint _start;

		Duration _dt;
		std::list<Duration> _log;

		inline TimePoint _now();

	public:
		Timer();

		inline void start();
		inline void stop();

		inline void clear();

		inline long long delta();
	};
};

inline int TestHelper::randInt(int a, int b){
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(a, b);

	return dis(gen);
}

inline float TestHelper::randFloat(float a, float b){
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis((double)a, (double)b);

	return (float)dis(gen);
}

inline double TestHelper::randDouble(double a, double b){
	static 	std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(a, b);

	return dis(gen);
}

inline TestHelper::Timer::TimePoint TestHelper::Timer::_now(){
	return std::chrono::time_point_cast<Units>(Clock::now());
}

inline void TestHelper::Timer::start(){
	_start = _now();
}

inline void TestHelper::Timer::stop(){
	_dt = Duration(_now() - _start);
	_log.push_back(_dt);
}

inline void TestHelper::Timer::clear(){
	_log.clear();
}

inline long long TestHelper::Timer::delta(){
	if (_log.size() == 1){
		return _dt.count();
	}
	else if (_log.size() > 1){
		long long average = 0;

		for (auto i : _log){
			average += i.count();
		}

		return average / _log.size();
	}

	return 0;
}