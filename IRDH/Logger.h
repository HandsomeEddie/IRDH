#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <mutex>

#ifndef LOGGER_H
#define LOGGER_H

struct Log {
    std::string logSwitch;           //日志开关
    std::string logFileSwitch;       //是否写入文件
    std::string logTerminalSwitch;   //是否打印到终端
    std::string logName;             //日志文件名字
    std::string logFilePath;         //日志文件保存路径
    std::string logMixSize;          //日志文件最大大小
    std::string logBehavior;         //日志文件达到最大大小行为
};

class Logger {
public:
    static Logger* GetInstance();

    void InitLogConfig();

    void PrintLogConfig();

    void LogError();

    void LogWarn();

    void LogInfo();

    void LogDebug();

    void LogTrace();
    void t();
private:

    Log log;
    // static std::mutex * mutex_log;
    // static std::mutex * mutex_file;
     //static std::mutex * mutex_queue;

private:
    Logger();
};



#endif // LOGGER_H