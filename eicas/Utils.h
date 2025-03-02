#pragma once
#include <chrono>
#include <ctime>
#include <string>

namespace Timer {
	const size_t MAX_BUFFER_SIZE = 80;
	extern char buffer[80];
	void updateTime();
	std::string getTime();
}

namespace Random {
	// random: 3% to 5% and -3% to -5%
	double random3To5Percent();

	// random: p true (1 - p) false
	bool binary(double p = 0.1);
}
