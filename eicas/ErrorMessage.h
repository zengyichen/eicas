#include "Widget.h"
#include "DataGenerator.h"
#include <functional>

#define SHOW_MESSAGE_INTERVAL ms_t(5000)

class ErrorMessage : public Widget {
public:
	static const int HEIGHT = 24;
	static const int WIDTH = 450;

	ErrorMessage(int x, int y, const char* text, ExceptionLevel level, std::function<bool()> src);
	~ErrorMessage();

	void draw();
	void update();
private:
	std::function<bool()> src;
	bool mem;
	tp_t timeMem;
	ExceptionLevel level;
	char* text;
};
