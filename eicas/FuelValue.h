#pragma once
#include "Widget.h"
#include "DataGenerator.h"
#include <functional>

class FuelValue : public Widget {

public:
	static const int WIDTH = 100;
	static const int HEIGHT = 22;

	FuelValue(int x, int y, std::function<const double& ()> valSrc, std::function<ExceptionLevel()> exptVal);
	~FuelValue();

	void draw();
	void update();

private:
	std::function<const double& ()> valSrc;
	std::function<ExceptionLevel()> exptSrc;
	double valMem;
	ExceptionLevel exptMem;
};
