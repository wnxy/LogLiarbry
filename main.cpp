#include "SWLog.h"

int main()
{
    SWLog::Init(true, false, "./test.txt");
    LOG_INFO("This is test message!");
    SWLog::UnInit();
    return 0;
}