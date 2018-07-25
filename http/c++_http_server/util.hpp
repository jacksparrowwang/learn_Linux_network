#pragma once

#include <iostream>
#include <sys/time.h>

class TimeUtil
{
public:
    static int64_t TimeStamp()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec;
    }

    static int64_t TimeStampUS()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return 1000*1000*tv.tv_sec+tv.tv_usec;
    }
};

enum LogLevel
{
    INFO,
    WARNING,
    ERROR,
    CRITIAL,
};

inline std::ostream& Log(LogLevel level, const char* file, int line)
{
    std::string prefix = "I";
    if (level == WARNING)
    {
        prefix = "W";
    }
    else if (level == ERROR)
    {
        prefix = "E";
    }
    else if (level == CRITIAL)
    {
        prefix = "C";
    }
    std::cout << "[" << prefix << TimeUtil::TimeStamp()<< " " \
        << file << ":" << line << "]";
    return std::cout;
}

#define LOG(level) Log(level, __FILE__, __LINE__)
