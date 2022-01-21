#include <iostream>
#include <thread>
#include "../runtime/SPLog.h"

void runBenchmark();
void getLog(int i);

int main()
{
	SWLog::Init(false, "test_asyn.txt");
	runBenchmark();
	SWLog::UnInit();
	std::cout << "Hello CMake." << std::endl;
	return 0;
}

void runBenchmark()
{
	const uint64_t RECORDS = 1000;

	std::chrono::high_resolution_clock::time_point start, stop;
	double time_span;

	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < RECORDS; ++i)
	{
		LOG_INFO("Simple log message with 0 parameters");
	}
	stop = std::chrono::high_resolution_clock::now();

	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
					stop - start)
					.count();
	printf("The total time spent invoking LOG_INFO with no parameters %lu "
		   "times took %0.2lf seconds (%0.2lf ns/message average)\r\n",
		   RECORDS, time_span, (time_span / RECORDS) * 1e9);
}

void getLog(int i)
{
	switch (i)
	{
	case 1:
		LOG_INFO("广州广州广州广州广州");
		break;
	case 2:
		LOG_INFO("深圳深圳深圳深圳深圳");
		break;
	case 3:
		LOG_INFO("西安西安西安西安西安");
		break;
	case 4:
		LOG_INFO("长沙长沙长沙长沙长沙");
		break;
	case 5:
		LOG_INFO("南京南京南京南京南京");
		break;
	default:
		break;
	}
}
