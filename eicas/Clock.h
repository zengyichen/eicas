#pragma once
#include "Widget.h"
#include "DataGenerator.h"
#include <functional>

const double EPS = 0.01;

class Clock : public Widget {

public:
	static const int R = 60;
	static const int TEXT_HEIGHT = 15;
	Clock(int x, int y);
	Clock(int x, int y, std::function<double()> valSrc, double min, double max, std::function<ExceptionLevel()> warningSrc);
	~Clock() {}

	void bind(std::function<double()> src, double min, double max);
	void draw();
	void update();

private:
	std::function<double()> valSrc;
	double min, max;
	double valMem;

	std::function<ExceptionLevel()> exptSrc;
	ExceptionLevel exptMem;
	COLORREF colorMem;
};
