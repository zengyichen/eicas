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

    // ʹ��stringstream�����ַ�����ʽ������ʱ��ͺ������������
    std::stringstream ss;
	std::string ret;
    ss << buffer << "." << std::setfill('0') << std::setw(3) << milliseconds;
	ss >> ret;
	return ret;
}

double Random::random3To5Percent() {
    double randomDouble = static_cast<double>(std::rand()) / RAND_MAX;
    // ����ӳ�䵽[3%, 5%]�ķ�Χ���ȳ���2����Ӧ0��2֮�䣩���ټ���3����Ӧ3��5֮�䣩��������100��ת��Ϊ�ٷֱȣ�
    double percentage = (randomDouble * 2 + 3) / 100;
    return percentage;
}

// random: p true (1 - p) false
bool Random::binary(double p) {
    double randomDouble = static_cast<double>(std::rand()) / RAND_MAX;
    return randomDouble < p;
}
