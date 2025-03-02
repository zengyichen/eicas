#include "FuelValue.h"
#include <sstream>
#include <iomanip>

FuelValue::FuelValue(int x, int y, std::function<const double& ()> src, std::function<ExceptionLevel()> exptSrc
	) : Widget(x, y) {
	this->valSrc = src;
	this->exptSrc = exptSrc;

	this->valMem = 0;
	this->exptMem = ExceptionLevel::INFO;
}

FuelValue::~FuelValue() {}

void FuelValue::draw() {
	int x = getX();
	int y = getY();

	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(x, y, x + WIDTH, y + HEIGHT);

	static char buf[10];

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << valSrc();
	ss >> buf;

	settextcolor(WHITE);
	settextstyle(HEIGHT - 2, 0, "Courier");  // 设置字体为某种字体（这里48是枚举值对应的字体），水平方向，字符大小为4
	setbkmode(TRANSPARENT);

	outtextxy(x + 1, y + 2, buf);
}

void FuelValue::update() {
	if (!valSrc || !exptSrc) return;

	if (std::isnan(valSrc())) return;

	if (valSrc() == valMem && exptSrc() == exptMem) return;

	int x = getX();
	int y = getY();

	auto color = exceptionColorMap.find(exptSrc())->second;

	static char buf[10];

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << valSrc();
	ss >> buf;

	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(x, y, x + WIDTH, y + HEIGHT);

	settextcolor(color);
	settextstyle(HEIGHT - 2, 0, "Courier");  // 设置字体为某种字体（这里48是枚举值对应的字体），水平方向，字符大小为4
	setbkmode(TRANSPARENT);

	outtextxy(x + 1, y + 2, buf);

	valMem = valSrc();
	exptMem = exptSrc();
}
