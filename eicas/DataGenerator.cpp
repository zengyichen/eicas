#include "DataGenerator.h"
#include <cmath>
#include <iostream>
#include "Utils.h"

DataGenerator::DataGenerator() {
	status = OFF;

	engineSpeedL1 = engineSpeedL2 = engineSpeedR1 = engineSpeedR2 = 0;
	exhaustGasTempL1 = exhaustGasTempL2 = exhaustGasTempR1 = exhaustGasTempR2 = 20;
	fuelRemain = 20000;
	fuelFlow = 0;
}

const engineStatus& DataGenerator::getStatus() const { return status; }
double DataGenerator::getEngineSpeedL1() const { return engineSpeedL1; }
double DataGenerator::getEngineSpeedL2() const { return engineSpeedL2; }
double DataGenerator::getEngineSpeedLVal() const {
	if (engineSpeedAvailable(engineSpeedL1) && engineSpeedAvailable(engineSpeedL2)) {
		return (engineSpeedL1 + engineSpeedL2) / 2;
	} else if (engineSpeedAvailable(engineSpeedL1)) {
		return engineSpeedL1;
	} else if (engineSpeedAvailable(engineSpeedL2)) {
		return engineSpeedL2;
	} else {
		return std::nan("");
	}
}

double DataGenerator::getEngineSpeedR1() const { return engineSpeedR1; }
double DataGenerator::getEngineSpeedR2() const { return engineSpeedR2; }
double DataGenerator::getEngineSpeedRVal() const {
	if (engineSpeedAvailable(engineSpeedR1) && engineSpeedAvailable(engineSpeedR2)) {
		return (engineSpeedR1 + engineSpeedR2) / 2;
	} else if (engineSpeedAvailable(engineSpeedR1)) {
		return engineSpeedR1;
	} else if (engineSpeedAvailable(engineSpeedR2)) {
		return engineSpeedR2;
	} else {
		return std::nan("");
	}
}

double DataGenerator::getExhaustGasTempL1() const { return exhaustGasTempL1; }
double DataGenerator::getExhaustGasTempL2() const { return exhaustGasTempL2; }
double DataGenerator::getExhaustGasTempLVal() const {
	if (exhaustGasTempAvailable(exhaustGasTempL1) && exhaustGasTempAvailable(exhaustGasTempL2)) {
		return (exhaustGasTempL1 + exhaustGasTempL2) / 2;
	} else if (exhaustGasTempAvailable(exhaustGasTempL1)) {
		return exhaustGasTempL1;
	} else if (exhaustGasTempAvailable(exhaustGasTempL2)) {
		return exhaustGasTempL2;
	} else {
		return std::nan("");
	}
}

double DataGenerator::getExhaustGasTempR1() const { return exhaustGasTempR1; }
double DataGenerator::getExhaustGasTempR2() const { return exhaustGasTempR2; }
double DataGenerator::getExhaustGasTempRVal() const {
	if (exhaustGasTempAvailable(exhaustGasTempR1) && exhaustGasTempAvailable(exhaustGasTempR2)) {
		return (exhaustGasTempR1 + exhaustGasTempR2) / 2;
	} else if (exhaustGasTempAvailable(exhaustGasTempR1)) {
		return exhaustGasTempR1;
	} else if (exhaustGasTempAvailable(exhaustGasTempR2)) {
		return exhaustGasTempR2;
	} else {
		return std::nan("");
	}
}

double DataGenerator::getFuelRemain() const { return fuelRemain; }
double DataGenerator::getFuelFlow() const { return fuelFlow; }

bool DataGenerator::queryException(ExceptionType exception) const {
	auto res = currentExceptions.find(exception);
	if (res == currentExceptions.end()) return false;
	return res->second;
}

ExceptionLevel DataGenerator::getEngineSpeedLExceptionLevel() const {
	if (!engineSpeedAvailable(getEngineSpeedLVal())) {
		return ExceptionLevel::FATAL;
	}

	if (getEngineSpeedLVal() > ENGINE_SPEED_ERROR) {
		return ExceptionLevel::FATAL;
	}

	if (getEngineSpeedLVal() > ENGINE_SPEED_WARNING) {
		return ExceptionLevel::WARNING;
	}

	return ExceptionLevel::INFO;
}

ExceptionLevel DataGenerator::getEngineSpeedRExceptionLevel() const {
	if (!engineSpeedAvailable(getEngineSpeedRVal())) {
		return ExceptionLevel::FATAL;
	}

	if (getEngineSpeedRVal() > ENGINE_SPEED_ERROR) {
		return ExceptionLevel::FATAL;
	}

	if (getEngineSpeedRVal() > ENGINE_SPEED_WARNING) {
		return ExceptionLevel::WARNING;
	}

	return ExceptionLevel::INFO;
}

ExceptionLevel DataGenerator::getExhaustGasTempLExceptionLevel() const {
	if (!exhaustGasTempAvailable(getExhaustGasTempLVal())) {
		return ExceptionLevel::FATAL;
	}

	if (getExhaustGasTempLVal() > EXHAUST_GAS_TEMP_STABLE_ERROR) {
		return ExceptionLevel::FATAL;
	}

	if (getExhaustGasTempLVal() > EXHAUST_GAS_TEMP_STABLE_WARNING) {
		return ExceptionLevel::WARNING;
	}

	return ExceptionLevel::INFO;
}

ExceptionLevel DataGenerator::getExhaustGasTempRExceptionLevel() const {
	if (!exhaustGasTempAvailable(getExhaustGasTempRVal())) {
		return ExceptionLevel::FATAL;
	}

	if (getExhaustGasTempRVal() > EXHAUST_GAS_TEMP_STABLE_ERROR) {
		return ExceptionLevel::FATAL;
	}

	if (getExhaustGasTempRVal() > EXHAUST_GAS_TEMP_STABLE_WARNING) {
		return ExceptionLevel::WARNING;
	}

	return ExceptionLevel::INFO;
}


ExceptionLevel DataGenerator::getFuelRemainExceptionLevel() const {
	if (getFuelRemain() < FUEL_REMAIN_MIN || std::isnan(getFuelRemain())) {
		return ExceptionLevel::FATAL;
	}

	if (getFuelRemain() < FUEL_REMAIN_WARNING) {
		return ExceptionLevel::WARNING;
	}

	return ExceptionLevel::INFO;
}

ExceptionLevel DataGenerator::getFuelFlowExceptionLevel() const {
	if (getFuelFlow() > FUEL_FLOW_WARNING) {
		return ExceptionLevel::WARNING;
	}

	return ExceptionLevel::INFO;
}

bool DataGenerator::needUpdate() {
	if (status == OFF) return false;
	return currentTime() - lastUpdatedTime >= UPDATE_INTERVAL;
}


tp_t DataGenerator::currentTime() const {
	return std::chrono::steady_clock::now();
}


void DataGenerator::update() {
	if (status == OFF) return;

	tp_t cur = currentTime();
	ms_t updateTime = castToMs(cur - lastUpdatedTime);

	if (status == STARTING) {
		ms_t totalTime = castToMs(cur - startTime);

		if (totalTime <= STARTUP_TIME) {
			engineSpeedL1 += 1.0 * updateTime.count() * 10000 / 1000;
			engineSpeedL2 += 1.0 * updateTime.count() * 10000 / 1000;

			engineSpeedR1 += 1.0 * updateTime.count() * 10000 / 1000;
			engineSpeedR2 += 1.0 * updateTime.count() * 10000 / 1000;

			fuelFlow += 1.0 * updateTime.count() * 5 / 1000;
		} else {
			double timeFactor = log10(1.0 * (totalTime.count() - 1000) / (totalTime.count() - updateTime.count() - 1000));
			engineSpeedL1 += 1.0 * 23000 * timeFactor;
			engineSpeedL2 += 1.0 * 23000 * timeFactor;
			engineSpeedR1 += 1.0 * 23000 * timeFactor;
			engineSpeedR2 += 1.0 * 23000 * timeFactor;

			exhaustGasTempL1 += 1.0 * 900 * timeFactor;
			exhaustGasTempL2 += 1.0 * 900 * timeFactor;
			exhaustGasTempR1 += 1.0 * 900 * timeFactor;
			exhaustGasTempR2 += 1.0 * 900 * timeFactor;

			fuelFlow += 1.0 * 42 * timeFactor;
		}

		if (engineSpeedL1 >= ratedEngineSpeed * 0.95 || engineSpeedL2 >= ratedEngineSpeed * 0.95
			|| engineSpeedR1 >= ratedEngineSpeed * 0.95 || engineSpeedR2 >= ratedEngineSpeed * 0.95) {
			status = STABLE;
		}
	} else if (status == STABLE) {
		// data remain stable
		randomFluctuate(engineSpeedL1);
		randomFluctuate(engineSpeedL2);
		randomFluctuate(engineSpeedR1);
		randomFluctuate(engineSpeedR2);

		randomFluctuate(exhaustGasTempL1);
		randomFluctuate(exhaustGasTempL2);
		randomFluctuate(exhaustGasTempR1);
		randomFluctuate(exhaustGasTempR2);

		randomFluctuate(fuelFlow);
	} else if (status == STOPPING) {
		fuelFlow = 0;

		ms_t timeSinceStop = castToMs(cur - stopTime);

		double timeFactor = log10(1.0 * (1 + 0.9 * (timeSinceStop.count()) / 1000)
			/ (1 + 0.9 * (timeSinceStop.count() - updateTime.count()) / 1000));

		std::cout << updateTime.count() << std::endl;
		std::cout << timeFactor << std::endl;

		engineSpeedL1 -= 1.0 * 50000 * timeFactor;
		engineSpeedL2 -= 1.0 * 50000 * timeFactor;
		engineSpeedR1 -= 1.0 * 50000 * timeFactor;
		engineSpeedR2 -= 1.0 * 50000 * timeFactor;

		if (engineSpeedL1 < 0) engineSpeedL1 = 0;
		if (engineSpeedL2 < 0) engineSpeedL2 = 0;
		if (engineSpeedR1 < 0) engineSpeedR1 = 0;
		if (engineSpeedR2 < 0) engineSpeedR2 = 0;

		exhaustGasTempL1 -= 1.0 * 1600 * timeFactor;
		exhaustGasTempL2 -= 1.0 * 1600 * timeFactor;
		exhaustGasTempR1 -= 1.0 * 1600 * timeFactor;
		exhaustGasTempR2 -= 1.0 * 1600 * timeFactor;

		if (exhaustGasTempL1 < EXHAUST_GAS_TEMP_STD) exhaustGasTempL1 = EXHAUST_GAS_TEMP_STD;
		if (exhaustGasTempL2 < EXHAUST_GAS_TEMP_STD) exhaustGasTempL2 = EXHAUST_GAS_TEMP_STD;
		if (exhaustGasTempR1 < EXHAUST_GAS_TEMP_STD) exhaustGasTempR1 = EXHAUST_GAS_TEMP_STD;
		if (exhaustGasTempR2 < EXHAUST_GAS_TEMP_STD) exhaustGasTempR2 = EXHAUST_GAS_TEMP_STD;

		// can stop even invalid
		if ((fabs(getEngineSpeedLVal() - ENGINE_SPEED_MIN) < EPS || std::isnan(getEngineSpeedLVal()))
			&& (fabs(getEngineSpeedRVal() - ENGINE_SPEED_MIN) < EPS || std::isnan(getEngineSpeedRVal()))
			&& (fabs(getExhaustGasTempLVal() - EXHAUST_GAS_TEMP_STD) < EPS || std::isnan(getExhaustGasTempLVal()))
			&& (fabs(getExhaustGasTempRVal() - EXHAUST_GAS_TEMP_STD) < EPS || std::isnan(getExhaustGasTempRVal()))) {
			status = OFF;
		}
	}

	fuelRemain -= 1.0 * fuelFlow * updateTime.count() / 1000;
	lastUpdatedTime = cur;
}


void DataGenerator::start() {
	auto updateExceptions = this->updateExceptions();

	// cant start if fatal exception exists
	for (auto& exception : updateExceptions) {
		if (exceptionLevelMap.find(exception)->second == ExceptionLevel::FATAL) {
			return;
		}
	}

	lastUpdatedTime = startTime = currentTime();
	status = STARTING;
}


// -50000lg(0.1 + t / 10 * 0.9)
void DataGenerator::stop() {
	if (status == STOPPING) return;
	status = STOPPING;
	fuelFlow = 0;
	lastUpdatedTime = stopTime = currentTime();
}


ms_t DataGenerator::castToMs(dms_t duration) const {
	return std::chrono::duration_cast<ms_t>(duration);
}

std::vector<ExceptionType> DataGenerator::updateExceptions() {
	std::vector<ExceptionType> errors;

	// single sensor invalid
	if (!engineSpeedAvailable(engineSpeedL1) || !engineSpeedAvailable(engineSpeedL2)
		|| !engineSpeedAvailable(engineSpeedR1) || !engineSpeedAvailable(engineSpeedR2)) {
		errors.push_back(ExceptionType::N1S1fail);
	}

	// single sensor invalid
	if (!engineSpeedAvailable(exhaustGasTempL1) || !engineSpeedAvailable(exhaustGasTempL2)
		|| !engineSpeedAvailable(exhaustGasTempR1) || !engineSpeedAvailable(exhaustGasTempR2)) {
		errors.push_back(ExceptionType::EGTS1fail);
	}

	bool engineSpeedLFail = !engineSpeedAvailable(engineSpeedL1) && !engineSpeedAvailable(engineSpeedL2);
	bool engineSpeedRFail = !engineSpeedAvailable(engineSpeedR1) && !engineSpeedAvailable(engineSpeedR2);

	bool exhaustGasTempLFail = !exhaustGasTempAvailable(exhaustGasTempL1) && !exhaustGasTempAvailable(exhaustGasTempL2);
	bool exhaustGasTempRFail = !exhaustGasTempAvailable(exhaustGasTempR1) && !exhaustGasTempAvailable(exhaustGasTempR2);

	// single engine invalid
	if (engineSpeedLFail || engineSpeedRFail) {
		errors.push_back(ExceptionType::N1S2fail);
	}
	if (exhaustGasTempLFail || exhaustGasTempRFail) {
		errors.push_back(ExceptionType::EGTS2fail);
	}

	// double engine invalid
	if (engineSpeedLFail && engineSpeedRFail) {
		errors.push_back(ExceptionType::N1Sfail);
	}
	if (exhaustGasTempLFail && exhaustGasTempRFail) {
		errors.push_back(ExceptionType::EGTSfail);
	}

	// fuel remain invalid
	if (fuelRemain < FUEL_REMAIN_WARNING) {
		errors.push_back(ExceptionType::LowFuel);
	}

	// fuel flow invalid
	if (fuelRemain < FUEL_REMAIN_MIN) {
		errors.push_back(ExceptionType::FuelSfail);
	}

	// exhaust gas temp invalid
	if (fuelFlow > FUEL_FLOW_WARNING) {
		errors.push_back(ExceptionType::OverFF);
	}

	// check engine speed
	if (getEngineSpeedLVal() > ENGINE_SPEED_WARNING || getEngineSpeedRVal() > ENGINE_SPEED_WARNING) {
		errors.push_back(ExceptionType::OverSpd1);
	}

	if (getEngineSpeedLVal() > ENGINE_SPEED_ERROR || getEngineSpeedRVal() > ENGINE_SPEED_ERROR) {
		errors.push_back(ExceptionType::OverSpd2);
	}

	// check EGT
	if (getStatus() == STARTING && (getExhaustGasTempLVal() > EXHAUST_GAS_TEMP_STARTING_WARNING
		|| getExhaustGasTempRVal() > EXHAUST_GAS_TEMP_STARTING_WARNING)) {
		errors.push_back(ExceptionType::OverTemp1);
	}

	if (getStatus() == STARTING && (getExhaustGasTempLVal() > EXHAUST_GAS_TEMP_STARTING_ERROR
		|| getExhaustGasTempRVal() > EXHAUST_GAS_TEMP_STARTING_ERROR)) {
		errors.push_back(ExceptionType::OverTemp2);
	}

	if (getStatus() == STABLE && (getExhaustGasTempLVal() > EXHAUST_GAS_TEMP_STABLE_WARNING
		|| getExhaustGasTempRVal() > EXHAUST_GAS_TEMP_STABLE_WARNING)) {
		errors.push_back(ExceptionType::OverTemp3);
	}

	if (getStatus() == STABLE && (getExhaustGasTempLVal() > EXHAUST_GAS_TEMP_STABLE_ERROR
		|| getExhaustGasTempRVal() > EXHAUST_GAS_TEMP_STABLE_ERROR)) {
		errors.push_back(ExceptionType::OverTemp4);
	}

	currentExceptions.clear();

	for (auto& error : errors) {
		currentExceptions[error] = 1;
	}

	return errors;
}

void DataGenerator::triggerException(ExceptionType exception) {
	if (exception == ExceptionType::N1S1fail) {
		engineSpeedL1 = std::nan("");
	} else if (exception == ExceptionType::N1S2fail) {
		engineSpeedR1 = std::nan("");
		engineSpeedR2 = std::nan("");
	} else if (exception == ExceptionType::EGTS1fail) {
		exhaustGasTempR1 = std::nan("");
	} else if (exception == ExceptionType::EGTS2fail) {
		exhaustGasTempL1 = std::nan("");
		exhaustGasTempL2 = std::nan("");
	} else if (exception == ExceptionType::N1Sfail) {
		engineSpeedL1 = std::nan("");
		engineSpeedL2 = std::nan("");
		engineSpeedR1 = std::nan("");
		engineSpeedR2 = std::nan("");
	} else if (exception == ExceptionType::EGTSfail) {
		exhaustGasTempL1 = std::nan("");
		exhaustGasTempL2 = std::nan("");
		exhaustGasTempR1 = std::nan("");
		exhaustGasTempR2 = std::nan("");
	} else if (exception == ExceptionType::LowFuel) {
		fuelRemain = 1000;
	} else if (exception == ExceptionType::FuelSfail) {
		fuelRemain = std::nan("");
	} else if (exception == ExceptionType::OverSpd1) {
		engineSpeedL1 = ENGINE_SPEED_WARNING + 50;
		engineSpeedL2 = ENGINE_SPEED_WARNING + 50;
		engineSpeedR1 = ENGINE_SPEED_WARNING + 50;
		engineSpeedR2 = ENGINE_SPEED_WARNING + 50;
	} else if (exception == ExceptionType::OverSpd2) {
		engineSpeedL1 = ENGINE_SPEED_ERROR + 50;
		engineSpeedL2 = ENGINE_SPEED_ERROR + 50;
		engineSpeedR1 = ENGINE_SPEED_ERROR + 50;
		engineSpeedR2 = ENGINE_SPEED_ERROR + 50;
	} else if (exception == ExceptionType::OverFF) {
		fuelFlow = 100;
	} else if (exception == ExceptionType::OverTemp1 && status == STARTING) {
		exhaustGasTempL1 = EXHAUST_GAS_TEMP_STARTING_WARNING + 50;
		exhaustGasTempL2 = EXHAUST_GAS_TEMP_STARTING_WARNING + 50;
		exhaustGasTempR1 = EXHAUST_GAS_TEMP_STARTING_WARNING + 50;
		exhaustGasTempR2 = EXHAUST_GAS_TEMP_STARTING_WARNING + 50;
	} else if (exception == ExceptionType::OverTemp2 && status == STARTING) {
		exhaustGasTempL1 = EXHAUST_GAS_TEMP_STARTING_ERROR + 100;
		exhaustGasTempL2 = EXHAUST_GAS_TEMP_STARTING_ERROR + 100;
		exhaustGasTempR1 = EXHAUST_GAS_TEMP_STARTING_ERROR + 100;
		exhaustGasTempR2 = EXHAUST_GAS_TEMP_STARTING_ERROR + 100;
	} else if (exception == ExceptionType::OverTemp3 && status == STABLE) {
		exhaustGasTempL1 = EXHAUST_GAS_TEMP_STABLE_WARNING + 50;
		exhaustGasTempL2 = EXHAUST_GAS_TEMP_STABLE_WARNING + 50;
		exhaustGasTempR1 = EXHAUST_GAS_TEMP_STABLE_WARNING + 50;
		exhaustGasTempR2 = EXHAUST_GAS_TEMP_STABLE_WARNING + 50;
	} else if (exception == ExceptionType::OverTemp4 && status == STABLE) {
		exhaustGasTempL1 = EXHAUST_GAS_TEMP_STABLE_ERROR + 50;
		exhaustGasTempL2 = EXHAUST_GAS_TEMP_STABLE_ERROR + 50;
		exhaustGasTempR1 = EXHAUST_GAS_TEMP_STABLE_ERROR + 50;
		exhaustGasTempR2 = EXHAUST_GAS_TEMP_STABLE_ERROR + 50;
	}
	// add 50 to see warning message easier
}

bool DataGenerator::engineSpeedAvailable(double engineSpeed) const {
	return engineSpeed >= ENGINE_SPEED_MIN && engineSpeed <= ENGINE_SPEED_MAX && !std::isnan(engineSpeed);
}

bool DataGenerator::exhaustGasTempAvailable(double exhaustGasTemp) const {
	return exhaustGasTemp >= EXHAUST_GAS_TEMP_MIN && exhaustGasTemp <= EXHAUST_GAS_TEMP_MAX && !std::isnan(exhaustGasTemp);
}

void DataGenerator::handleExceptions(std::vector<ExceptionType> exceptions) {
	for (auto& exception : exceptions) {
		// const std::map dont have operator[]
		auto itr = exceptionLevelMap.find(exception);
		if (itr->second == ExceptionLevel::FATAL) {
			stop();
		}
	}
}

void DataGenerator::speedUp() {
	if (status == OFF) return;
	fuelFlow += 1;
	
	engineSpeedL1 *= (1 + Random::random3To5Percent());
	engineSpeedL2 *= (1 + Random::random3To5Percent());
	engineSpeedR1 *= (1 + Random::random3To5Percent());
	engineSpeedR2 *= (1 + Random::random3To5Percent());

	exhaustGasTempL1 *= (1 + Random::random3To5Percent());
	exhaustGasTempL2 *= (1 + Random::random3To5Percent());
	exhaustGasTempR1 *= (1 + Random::random3To5Percent());
	exhaustGasTempR2 *= (1 + Random::random3To5Percent());
}

void DataGenerator::speedDown() {
	if (status == OFF) return;
	fuelFlow -= 1;
	if (fuelFlow < 0) fuelFlow = 0;

	engineSpeedL1 *= (1 - Random::random3To5Percent());
	engineSpeedL2 *= (1 - Random::random3To5Percent());
	engineSpeedR1 *= (1 - Random::random3To5Percent());
	engineSpeedR2 *= (1 - Random::random3To5Percent());

	exhaustGasTempL1 *= (1 - Random::random3To5Percent());
	exhaustGasTempL2 *= (1 - Random::random3To5Percent());
	exhaustGasTempR1 *= (1 - Random::random3To5Percent());
	exhaustGasTempR2 *= (1 - Random::random3To5Percent());
}

void DataGenerator::randomFluctuate(double& val) {
	if (!Random::binary(0.02)) return;
	if (Random::binary(0.5)) {
		val *= (1 + Random::random3To5Percent() / 10);
	} else {
		val *= (1 - Random::random3To5Percent() / 10);
	}
}
