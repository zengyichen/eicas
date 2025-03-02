#include "Clock.h"
#include <graphics.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

Clock::Clock(int x, int y) : Widget(x, y) {
	this->valSrc = nullptr;
	this->exptSrc = nullptr;
	this->max = 0;
	this->min = 0;
	this->valMem = 0;
	this->colorMem = LIGHTGRAY;
}


Clock::Clock(int x, int y, std::function<double()> src, double min, double max, std::function<ExceptionLevel()> exptSrc) : Widget(x, y) {
	this->valSrc = src;
	this->min = min;
	this->max = max;
	this->exptSrc = exptSrc;
	this->exptMem = exptSrc();
	this->colorMem = exceptionColorMap.find(exptSrc())->second;
}

void Clock::draw() {
	if (!valSrc) return;

	auto color = exceptionColorMap.find(exptSrc())->second;

	int x = getX();
	int y = getY();

	double val = (valSrc() - min) / (max - min);
	if (val > EPS && val < 210 - EPS) {
		setlinestyle(PS_SOLID, 0); // no edge
		setlinecolor(color);
		setfillcolor(color);
		fillpie(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210 * val, 0);
	}

	// draw text
	static char buffer[10];

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << valSrc();
	ss >> buffer;

	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(x + R, y + R - TEXT_HEIGHT, x + 2 * R, y + R);

	settextcolor(WHITE);
	settextstyle(TEXT_HEIGHT, 0, "Courier");
	setbkmode(TRANSPARENT);
	outtextxy(x + R + 2, y + R - TEXT_HEIGHT - 2, buffer);

	// draw outline
	setlinestyle(PS_SOLID, 1);
	setcolor(WHITE);
	arc(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210, 0);

	valMem = valSrc();
	colorMem = color;
	exptMem = exptSrc();
}


void Clock::update() {
	if (!valSrc || !exptSrc) return;

	if (std::isnan(valSrc())) return;

	if (exptSrc() == exptMem && fabs(valSrc() - valMem) < EPS) return;

	int x = getX();
	int y = getY();

	if (exptSrc() != exptMem) {
		exptMem = exptSrc();
		colorMem = exceptionColorMap.find(exptSrc())->second;
		double ratio = (valMem - min) / (max - min);

		setlinestyle(PS_SOLID, 0); // no edge
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillpie(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210, 0);

		if (ratio > EPS && ratio < 210 - EPS) {
			setlinestyle(PS_SOLID, 0); // no edge
			setlinecolor(colorMem);
			setfillcolor(colorMem);
			fillpie(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210 * ratio, 0);
		}

		setlinestyle(PS_SOLID, 1);
		setcolor(WHITE);
		arc(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210, 0);
	}

	static char buffer[10];

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << valSrc();
	ss >> buffer;

	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(x + R, y + R - TEXT_HEIGHT, x + 2 * R, y + R);

	settextcolor(WHITE);
	settextstyle(TEXT_HEIGHT, 0, "Courier");  // 设置字体为某种字体（这里48是枚举值对应的字体），水平方向，字符大小为4
	setbkmode(TRANSPARENT);
	outtextxy(x + R + 2, y + R - TEXT_HEIGHT - 2, buffer);

	double ratio = (valSrc() - min) / (max - min);

	if (ratio > EPS && ratio < 210 - EPS) {
		setlinestyle(PS_SOLID, 0); // no edge
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillpie(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210, -PI / 180 * 210 * ratio);

		setlinestyle(PS_SOLID, 0); // no edge
		setlinecolor(colorMem);
		setfillcolor(colorMem);
		fillpie(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210 * ratio, 0);
	}

	valMem = valSrc();

	setlinestyle(PS_SOLID, 1);
	setcolor(WHITE);
	arc(x, y, x + 2 * R, y + 2 * R, -PI / 180 * 210, 0);
}
