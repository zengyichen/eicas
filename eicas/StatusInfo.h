#include "Widget.h"

class StatusInfo : public Widget {
public:

	static const int WIDTH = 64;
	static const int HEIGHT = 22;

	static const COLORREF DISABLED_COLOR = BLACK;
	static const COLORREF ENABLED_COLOR = BLUE;

	StatusInfo(int x, int y, const char* text, std::function<bool()> src);
	~StatusInfo();

	void draw();
	void update();
private:
	std::function<bool()> src;
	char* text;
	bool mem;
};