#pragma once

#include <iostream>
#include <vector>
// boost库中字符串操作
#include <boost/algorithm/string.hpp>
#include <sys/socket.h>
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

class FileUtil
{
public:
    // 读取文件描述符中一行
    // 在line中不包含\n界定符
    static int  ReadLine(int fd, std::string* line)
    {
        // {
        //      string tmp;
        //      str.swap(tmp);
        // }
        line->clear();
        while (true)
        {
            char c = '\0';
            ssize_t read_size = recv(fd, &c, 1, 0);
            if (read_size <= 0)
            {
                return -1;
            }

            if (c == '\r')
            {
                // 偷窥下一个数据
                recv(fd, &c, 1, MSG_PEEK);
                if (c == '\n')
                {
                    // 再进行读
                    recv(fd, &c, 1, 0);
                }
                else
                {
                    // 不是\n
                    c = '\n';
                }
            }
            // 把所有的\n \r \r\n 都转换成\n
            if (c == '\n')
            {
                break;
            }
            line->push_back(c);
        }
        return 0;
    }

    
    static bool IsDir(const std::string& file_path)
    {
        return boost::filesystem::is_directory(file_path);
    }

    int ReadN(int fd, size_t len, std::string* output)
    {
        // TODO
    }
};


class StringUtil
{
public:
    // 把split_char切分成n个output中
    static int Split(const std::string& input, const std::string& split_char, std::vector<std::string>* output)
    {
        // 1 输出参数， 2 输入， 3 见到什么字符切腹 4 进行切分结果打开 off叫压缩
        boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_on);
        return 0;
    }
};
