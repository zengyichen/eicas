#pragma once

#include "DataGenerator.h"
#include <string>
#include <fstream>

class Csv {
public:
	Csv(std::string filename = "record.csv");
	~Csv();
	void addLine(DataGenerator& data);
private:
	std::ofstream csvFile;
};
