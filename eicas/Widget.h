#ifndef WIDGET_H
#define WIDGET_H
#include <graphics.h>
#include <cmath>
#include <vector>
#include <map>
#include <functional>
#include "DataGenerator.h"

const double PI = acos(-1);

const std::map<ExceptionLevel, COLORREF> exceptionColorMap = {
	{ExceptionLevel::INFO, 0xFFFFFF},
	{ExceptionLevel::WARNING, 0x0088FF},
	{ExceptionLevel::FATAL, 0x0000FF}
};

class Widget {
public:
	Widget(int x, int y);
	virtual ~Widget() = default;

	const int& getX() const;
	const int& getY() const;

	virtual void draw() = 0;
	virtual void update() = 0;

private:
	int x, y;
	Widget();
};

namespace WidgetHandler {
	extern std::vector<Widget*> widgets;
	void add(Widget* widget);
	void draw();
	void update();
}

#endif
