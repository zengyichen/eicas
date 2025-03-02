#pragma once

#include "Widget.h"

enum ButtonStatus {
	OUTSIDE,
	HOVER,
	PRESSED
};

class Button : public Widget {
public:

	static const int WIDTH = 100;
	static const int HEIGHT = 30;
	static const int TEXT_HEIGHT = 16;

	static const COLORREF OUTSIDE_COLOR = BLACK;
	static const COLORREF HOVER_COLOR = DARKGRAY;
	static const COLORREF PRESSED_COLOR = BLUE;

	Button(int x, int y, const char* text, ExMessage* mouse, std::function<void()> onClic = nullptr);
	~Button();

	void draw();
	void update();

	ButtonStatus getStatus() const;
private:
	std::function<void()> onClick;
	char* text;
	ExMessage* mouse;
	ButtonStatus status;
};
