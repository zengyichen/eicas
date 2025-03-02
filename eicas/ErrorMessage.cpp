#include "ErrorMessage.h"

ErrorMessage::ErrorMessage(int x, int y, const char* text, ExceptionLevel level, std::function<bool()> src) : Widget(x, y) {
	size_t len = strlen(text);
	this->text = new char[len + 3];
	strcpy_s(this->text, len + 3, text);
	this->level = level;
	this->src = src;
	this->timeMem = std::chrono::steady_clock::now();
	this->mem = false;
}

ErrorMessage::~ErrorMessage() {
	delete[] text;
}

void ErrorMessage::draw() {
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(getX(), getY(), getX() + WIDTH, getY() + HEIGHT);
	/*
	mem = src();

	if (!mem) return;
	if (std::chrono::steady_clock::now() - timeMem < SHOW_MESSAGE_INTERVAL) return;

	settextcolor(exceptionColorMap.find(level)->second);
	settextstyle(HEIGHT, 0, "Courier");
	setbkmode(TRANSPARENT);
	outtextxy(getX(), getY(), text);
	*/
}

void ErrorMessage::update() {
	if (!src) return;

	int x = getX(), y = getY();

	// show message if status changed
	if (!mem && src()) {
		mem = src();
		timeMem = std::chrono::steady_clock::now();
		settextcolor(exceptionColorMap.find(level)->second);
		settextstyle(HEIGHT, 0, "Courier");
		setbkmode(TRANSPARENT);
		outtextxy(x, y, text);

		return;
	}

	// hide message if: 1. status changed; 2. time interval passed
	if (std::chrono::steady_clock::now() - timeMem < SHOW_MESSAGE_INTERVAL) return;
	if (mem == src()) return;

	mem = src();

	if (!mem) {
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillrectangle(x, y, x + WIDTH, y + HEIGHT);
	}
}
