#pragma once
#include <chrono>
#include <vector>
#include <map>

/*
 std::chrono::milliseconds stores data in an integer of at least 45 bits,
 which is enough to store the number of milliseconds in 584.9 years.
 so no overflow will happen in this case.
*/
typedef std::chrono::milliseconds ms_t;
typedef std::chrono::steady_clock::time_point tp_t;
typedef std::chrono::duration<double, std::milli> dms_t;

enum engineStatus {
	OFF,
	STARTING,
	STABLE,
	STOPPING
};

enum class ExceptionType {
	NoError,
	N1S1fail,
	N1S2fail,
	EGTS1fail,
	EGTS2fail,
	N1Sfail,
	EGTSfail,
	LowFuel,
	FuelSfail,
	OverSpd1,
	OverSpd2,
	OverFF,
	OverTemp1,
	OverTemp2,
	OverTemp3,
	OverTemp4
};

enum class ExceptionLevel {
	INFO,
	WARNING,
	FATAL
};

const std::map<ExceptionType, const char*> exceptionNameMap = {
	{ExceptionType::NoError, "NoError"},
	{ExceptionType::N1S1fail, "N1S1Fail"},
	{ExceptionType::N1S2fail, "N1S2Fail"},
	{ExceptionType::EGTS1fail, "EGTS1Fail"},
	{ExceptionType::EGTS2fail, "EGTS2Fail"},
	{ExceptionType::N1Sfail, "N1SFail"},
	{ExceptionType::EGTSfail, "EGTSFail"},
	{ExceptionType::LowFuel, "LowFuel"},
	{ExceptionType::FuelSfail, "FuelSFail"},
	{ExceptionType::OverSpd1, "OverSpd1"},
	{ExceptionType::OverSpd2, "OverSpd2"},
	{ExceptionType::OverFF, "OverFF"},
	{ExceptionType::OverTemp1, "OverTemp1"},
	{ExceptionType::OverTemp2, "OverTemp2"},
	{ExceptionType::OverTemp3, "OverTemp3"},
	{ExceptionType::OverTemp4, "OverTemp4"}
};


const std::map<ExceptionType, ExceptionLevel> exceptionLevelMap = {
	{ExceptionType::NoError, ExceptionLevel::INFO},
	{ExceptionType::N1S1fail, ExceptionLevel::INFO},
	{ExceptionType::N1S2fail, ExceptionLevel::WARNING},
	{ExceptionType::EGTS1fail, ExceptionLevel::INFO},
	{ExceptionType::EGTS2fail, ExceptionLevel::WARNING},
	{ExceptionType::N1Sfail, ExceptionLevel::FATAL},
	{ExceptionType::EGTSfail, ExceptionLevel::FATAL},
	{ExceptionType::LowFuel, ExceptionLevel::WARNING},
	{ExceptionType::FuelSfail, ExceptionLevel::FATAL},
	{ExceptionType::OverSpd1, ExceptionLevel::WARNING},
	{ExceptionType::OverSpd2, ExceptionLevel::FATAL},
	{ExceptionType::OverFF, ExceptionLevel::WARNING},
	{ExceptionType::OverTemp1, ExceptionLevel::WARNING},
	{ExceptionType::OverTemp2, ExceptionLevel::FATAL},
	{ExceptionType::OverTemp3, ExceptionLevel::WARNING},
	{ExceptionType::OverTemp4, ExceptionLevel::FATAL}
};


#define ZERO_MS ms_t(0)
#define UPDATE_INTERVAL ms_t(5)
#define STARTUP_TIME ms_t(2000)

class DataGenerator {

public:
	const double ratedEngineSpeed = 40000;

	DataGenerator();
	~DataGenerator() = default;

	bool needUpdate();

	// update all data
	void update();

	// start engine
	void start();
	
	// start stopping process
	void stop();

	std::vector<ExceptionType> updateExceptions();
	void handleExceptions(std::vector<ExceptionType> exceptions);
	
	const engineStatus& getStatus() const;

	const double EPS = 1e-6;

	const double ENGINE_SPEED_STD = 40000;
	const double ENGINE_SPEED_MIN = 0;
	const double ENGINE_SPEED_WARNING = ENGINE_SPEED_STD * 1.05;
	const double ENGINE_SPEED_ERROR = ENGINE_SPEED_STD * 1.2;
	const double ENGINE_SPEED_MAX = ENGINE_SPEED_STD * 1.25;

	const double EXHAUST_GAS_TEMP_MIN = -5;
	const double EXHAUST_GAS_TEMP_STD = 20;
	const double EXHAUST_GAS_TEMP_STARTING_WARNING = 850;
	const double EXHAUST_GAS_TEMP_STARTING_ERROR = 1000;
	const double EXHAUST_GAS_TEMP_STABLE_WARNING = 950;
	const double EXHAUST_GAS_TEMP_STABLE_ERROR = 1100;
	const double EXHAUST_GAS_TEMP_MAX = 1200;

	const double FUEL_REMAIN_MAX = 20000;
	const double FUEL_REMAIN_WARNING = 1000;
	const double FUEL_REMAIN_MIN = 0;

	const double FUEL_FLOW_WARNING = 50;

	double getEngineSpeedL1() const;
	double getEngineSpeedL2() const;
	double getEngineSpeedLVal() const;

	double getEngineSpeedR1() const;
	double getEngineSpeedR2() const;
	double getEngineSpeedRVal() const;

	double getExhaustGasTempL1() const;
	double getExhaustGasTempL2() const;
	double getExhaustGasTempLVal() const;

	double getExhaustGasTempR1() const;
	double getExhaustGasTempR2() const;
	double getExhaustGasTempRVal() const;

	double getFuelRemain() const;
	double getFuelFlow() const;

	bool queryException(ExceptionType exception) const;
	void triggerException(ExceptionType exception);
	
	ExceptionLevel getEngineSpeedLExceptionLevel() const;
	ExceptionLevel getEngineSpeedRExceptionLevel() const;
	ExceptionLevel getExhaustGasTempLExceptionLevel() const;
	ExceptionLevel getExhaustGasTempRExceptionLevel() const;

	ExceptionLevel getFuelRemainExceptionLevel() const;
	ExceptionLevel getFuelFlowExceptionLevel() const;

	void speedUp();
	void speedDown();

private:

	engineStatus status;

	tp_t startTime;
	tp_t stopTime;
	tp_t lastUpdatedTime;

	// N
	double engineSpeedL1, engineSpeedL2;
	double engineSpeedR1, engineSpeedR2;
	// T
	double exhaustGasTempL1, exhaustGasTempL2;
	double exhaustGasTempR1, exhaustGasTempR2;

	// C
	double fuelRemain;
	// V
	double fuelFlow;

	std::map<ExceptionType, bool> currentExceptions;

	// milisecs since start
	// return ms_t(0) if not started
	tp_t currentTime() const;

	ms_t castToMs(dms_t duration) const;

	bool engineSpeedAvailable(double engineSpeed) const;
	bool exhaustGasTempAvailable(double exhaustGasTemp) const;

	void randomFluctuate(double& val);
};
