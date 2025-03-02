#include "StatusInfo.h"

StatusInfo::StatusInfo(int x, int y, const char* text, std::function<bool()> src) : Widget(x, y) {
	size_t len = strlen(text);
	this->text = new char[len + 3];
	strcpy_s(this->text, len + 3, text);
	this->src = src;
	this->mem = false;
}

StatusInfo::~StatusInfo() {
	delete[] text;
}

void StatusInfo::draw() {
	setlinecolor(WHITE);
	setfillcolor(DISABLED_COLOR);
	fillrectangle(getX(), getY(), getX() + WIDTH, getY() + HEIGHT);

	settextcolor(WHITE);
	settextstyle(HEIGHT - 2, 0, "Courier");
	setbkmode(TRANSPARENT);
	outtextxy(getX() + 1, getY() + 2, text);
}

void StatusInfo::update() {
	if (!src) return;

	if (src() == mem) return;

	mem = src();

	setlinecolor(WHITE);

	if (mem) {
		setfillcolor(ENABLED_COLOR);
	} else {
		setfillcolor(DISABLED_COLOR);
	}

	fillrectangle(getX(), getY(), getX() + WIDTH, getY() + HEIGHT);

	settextcolor(WHITE);
	settextstyle(HEIGHT- 2, 0, "Courier");
	setbkmode(TRANSPARENT);

	outtextxy(getX() + 1, getY() + 2, text);
}
