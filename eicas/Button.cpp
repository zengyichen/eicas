#include "Button.h"

Button::Button(int x, int y, const char* text, ExMessage* mouse, std::function<void()> onClick) : Widget(x, y) {
	size_t len = strlen(text);
	this->text = new char[len + 3];
	strcpy_s(this->text, len + 3, text);
	this->mouse = mouse;
	this->status = OUTSIDE;
	this->onClick = onClick;
}

Button::~Button() {
	delete[] text;
}

void Button::draw() {
	setlinecolor(WHITE);
	setfillcolor(OUTSIDE_COLOR);
	fillrectangle(getX(), getY(), getX() + WIDTH, getY() + HEIGHT);

	settextcolor(WHITE);
	settextstyle(TEXT_HEIGHT, 0, "Courier");
	setbkmode(TRANSPARENT);
	outtextxy(getX() + 2, getY() + (HEIGHT - TEXT_HEIGHT) / 2, text);
}

void Button::update() {
	ButtonStatus cur;
	// if not in the button, return
	if (mouse->x < getX() || mouse->x > getX() + WIDTH || mouse->y < getY() || mouse->y > getY() + HEIGHT) {
		cur = OUTSIDE;
	} else if (mouse->message == WM_LBUTTONDOWN) {
		cur = PRESSED;
	} else {
		cur = HOVER;
	}

	if (cur == status) {
		return;
	}

	status = cur;

	setlinecolor(WHITE);
	
	if (status == OUTSIDE) {
		setfillcolor(OUTSIDE_COLOR);
	} else if (status == HOVER) {
		setfillcolor(HOVER_COLOR);
	} else if (status == PRESSED) {
		setfillcolor(PRESSED_COLOR);

		if (onClick) {
			onClick();
		}
	}

	fillrectangle(getX(), getY(), getX() + WIDTH, getY() + HEIGHT);

	settextcolor(WHITE);
	settextstyle(TEXT_HEIGHT, 0, "Courier");  // 设置字体为某种字体（这里48是枚举值对应的字体），水平方向，字符大小为4
	setbkmode(TRANSPARENT);

	outtextxy(getX() + 2, getY() + (HEIGHT - TEXT_HEIGHT) / 2, text);
}

ButtonStatus Button::getStatus() const {
	return status;
}
