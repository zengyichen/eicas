#include "Utils.h"
#include <sstream>
#include <iomanip>

static char Timer::buffer[80];
void Timer::updateTime() {
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);

	errno_t result = localtime_s(&timeinfo, &rawtime);
	strftime(buffer, MAX_BUFFER_SIZE * sizeof(char), "%Y-%m-%d-%H:%M:%S", &timeinfo);

}

std::string Timer::getTime() {
	updateTime();

	auto now = std::chrono::system_clock::now();
    auto duration_since_epoch = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_since_epoch);
    auto remainder_nanoseconds = duration_since_epoch - seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(remainder_nanoseconds).count();

    // 使用stringstream进行字符串格式化，将时间和毫秒数组合起来
    std::stringstream ss;
	std::string ret;
    ss << buffer << "." << std::setfill('0') << std::setw(3) << milliseconds;
	ss >> ret;
	return ret;
}

double Random::random3To5Percent() {
    double randomDouble = static_cast<double>(std::rand()) / RAND_MAX;
    // 将其映射到[3%, 5%]的范围，先乘以2（对应0到2之间），再加上3（对应3到5之间），最后除以100（转换为百分比）
    double percentage = (randomDouble * 2 + 3) / 100;
    return percentage;
}

// random: p true (1 - p) false
bool Random::binary(double p) {
    double randomDouble = static_cast<double>(std::rand()) / RAND_MAX;
    return randomDouble < p;
}
