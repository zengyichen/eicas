#include "Widget.h"

Widget::Widget() {
	WidgetHandler::add(this);
}

Widget::Widget(int x, int y) : Widget() {
	this->x = x;
	this->y = y;
}

const int& Widget::getX() const {
	return x;
}

const int& Widget::getY() const {
	return y;
}

static std::vector<Widget*> WidgetHandler::widgets;

void WidgetHandler::add(Widget* widget) {
	widgets.push_back(widget);
}

void WidgetHandler::draw() {
	settextstyle(20, 0, "Courier");
	outtextxy(360, 40 + 2, "TOTAL FUEL");
	outtextxy(360, 120 + 2, "FUEL FLOW");

	for (auto widget : widgets) {
		widget->draw();
	}
}

void WidgetHandler::update() {
	for (auto widget : widgets) {
		widget->update();
	}
}
