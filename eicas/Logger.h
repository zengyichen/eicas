#pragma once
#include "DataGenerator.h"
#include <map>
#include <string>
#include <fstream>
#include <vector>

#define LOG_INTERVAL ms_t(5000)

static const std::map<ExceptionType, const char*> errorMessageMap = {
	{ExceptionType::NoError, "No error."},
	{ExceptionType::N1S1fail, "N1 single sensor failure."},
	{ExceptionType::N1S2fail, "N1 single engine failure."},
	{ExceptionType::EGTS1fail, "EGT single sensor failure."},
	{ExceptionType::EGTS2fail, "EGT single engine failure."},
	{ExceptionType::N1Sfail, "N1 both engine failure. STOP"},
	{ExceptionType::EGTSfail, "EGT both engine failure. STOP"},
	{ExceptionType::LowFuel, "Low fuel."},
	{ExceptionType::FuelSfail, "Fuel failure. STOP"},
	{ExceptionType::OverSpd1, "Overspeed warning."},
	{ExceptionType::OverSpd2, "Overspeed failure. STOP"},
	{ExceptionType::OverFF, "Over fuel flow."},
	{ExceptionType::OverTemp1, "(START) Overtemp warning."},
	{ExceptionType::OverTemp2, "(START) Overtemp failure. STOP"},
	{ExceptionType::OverTemp3, "(RUN) Overtemp warning."},
	{ExceptionType::OverTemp4, "(RUN) Overtemp failure. STOP"}
};

// singleton
class Logger {
public:
	void log(const ExceptionType &error);
	void log(const std::vector<ExceptionType> &errors);
    Logger(std::string filename = "log.txt");
	~Logger();
private:
	std::ofstream logFile;
	std::map<ExceptionType, tp_t> lastLogTp;
	const size_t MAX_BUFFER_SIZE = 35;
};
