#include "TestUtils.hpp"

#include <random>

int randInt(int a, int b){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(a, b);

	return dis(gen);
}

float randFloat(float a, float b){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis((double)a, (double)b);

	return (float)dis(gen);
}

double randDouble(double a, double b){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> dis(a, b);

	return dis(gen);
}

Timer::Timer(){
	_start = _now();
}

Timer::TimePoint Timer::_now(){
	return std::chrono::time_point_cast<Units>(Clock::now());
}

void Timer::start(){
	_start = _now();
}

void Timer::stop(){
	_dt = Duration(_now() - _start);
	_log.push_back(_dt);
}

void Timer::clear(){
	_log.clear();
}

long long Timer::delta(){
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