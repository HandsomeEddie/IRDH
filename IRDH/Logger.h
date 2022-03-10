#include "Utils.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <mutex>

#ifndef LOGGER_H
#define LOGGER_H

enum class LogType: int {
    ErrorType = 1,
    WarnType = 2,
    InfoType = 3,
    DebugType = 4,
    TraceType = 5
};

#define __LOGTIME__  Logger::GetInstance()->GetLogCoutTime()          //时间宏
#define __LOGPID__   Logger::GetInstance()->GetLogCoutProcessId()     //进程宏
#define __LOGTID__   Logger::GetInstance()->GetLogCoutThreadId()      //线程宏
#define __LOGFILE__  __FILE__          //文件名宏
#define __LOGFUNC__  __func__          //函数名宏
#define __LOGLINE__  __LINE__          //行数宏

const std::string SWITCH_ON = "on";
const std::string SWITCH_OFF = "off";

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

    std::string GetLogCoutTime();

    std::string GetLogCoutProcessId();

    std::string GetLogCoutThreadId();

    std::string GetLogCoutUserName();

    std::string GetFilePathAndName();

    void WriteLogIntoFile(LogType logType, std::string message);

    void WriteLogIntoTerminal(LogType logType, std::string message);

    void WriteLog(LogType logType, std::string message);

    void LogWarn(std::string message);

private:
    Log log;
    static Logger* mSingleObject;
    static std::mutex* mMutexLog;
    std::map<LogType, std::string> mLogTypeMap;

private:
    Logger();
};

#define LOGWARN(message) Logger::GetInstance()->LogWarn(message)

#endif // LOGGER_H