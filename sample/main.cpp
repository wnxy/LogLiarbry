#include <iostream>
#include <chrono>
#include "../runtime/SWLog.h"

void runBenchmark();

int main()
{
	SWLog::Init(true, false, "test.txt");
	LOG_INFO("This is test message!");
	LOG_WARNING("这是一条测试警告日志！");
	LOG_ERROR("这是一条测试错误日志！");
	SWLog::UnInit();
	std::cout << "Hello CMake." << std::endl;
	return 0;
}

void runBenchmark() {
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
                                                        stop - start).count();
    printf("The total time spent invoking LOG_INFO with no parameters %lu "
            "times took %0.2lf seconds (%0.2lf ns/message average)\r\n",
            RECORDS, time_span, (time_span/RECORDS)*1e9);
/*
    start = std::chrono::high_resolution_clock::now();
    // Flush all pending log messages to disk
    NanoLog::sync();
    stop = std::chrono::high_resolution_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
                                                        stop - start).count();
    printf("Flushing the log statements to disk took an additional "
                "%0.2lf secs\r\n", time_span);
*/
}