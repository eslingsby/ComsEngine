#include "TestHelper.hpp"

#include <random>

int TestHelper::randInt(int a, int b){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(a, b);

	return dis(gen);
}

float TestHelper::randFloat(float a, float b){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis((double)a, (double)b);

	return (float)dis(gen);
}

double TestHelper::randDouble(double a, double b){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(a, b);

	return dis(gen);
}

TestHelper::Timer::Timer(){
	_start = _now();
}

TestHelper::Timer::TimePoint TestHelper::Timer::_now(){
	return std::chrono::time_point_cast<Units>(Clock::now());
}

void TestHelper::Timer::start(){
	_start = _now();
}

void TestHelper::Timer::stop(){
	_dt = Duration(_now() - _start);
	_log.push_back(_dt);
}

void TestHelper::Timer::clear(){
	_log.clear();
}

long long TestHelper::Timer::delta(){
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