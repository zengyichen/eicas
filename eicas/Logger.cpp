#include "Logger.h"
#include "Utils.h"
#include <iostream>
#include <format>
#include <iomanip>

Logger::Logger(std::string filename) {
	logFile.open(filename);
	if (!logFile.is_open()) {
		// todo: throw exception
		throw("Logger: cannot open file");
	}

	logFile << "Logging started at [" << Timer::getTime() << "].\n";
	std::cerr << "Logging started at [" << Timer::getTime() << "].\n";
}

Logger::~Logger() {
	logFile.close();
}

void Logger::log(const ExceptionType& error) {
	Timer::updateTime();
	if (std::chrono::steady_clock::now() - lastLogTp[error] < LOG_INTERVAL) {
		return;
	}
	logFile << "[" << Timer::getTime() << "] " << errorMessageMap.find(error)->second << "\n";
	lastLogTp[error] = std::chrono::steady_clock::now();
}

void Logger::log(const std::vector<ExceptionType>& errors) {
	for (auto error : errors) {
		log(error);
	}
}
