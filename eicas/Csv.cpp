#include "Csv.h"
#include "Utils.h"
#include <fstream>

Csv::Csv(std::string filename) {
	csvFile.open(filename);
	if (!csvFile.is_open()) {
		// todo
	}
	
	csvFile << "Time, N1, N2, EGT1, EGT2, Fuel Level, Fuel Flow\n";
}

Csv::~Csv() {
	csvFile.close();
}

void Csv::addLine(DataGenerator& data) {
	csvFile << Timer::getTime() << ", "
		<< data.getEngineSpeedLVal() << ", "
		<< data.getEngineSpeedRVal() << ", "
		<< data.getExhaustGasTempLVal() << ", "
		<< data.getExhaustGasTempRVal() << ", "
		<< data.getFuelRemain() << ", "
		<< data.getFuelFlow() << "\n";
}
