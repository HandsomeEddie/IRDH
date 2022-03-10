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

#define __LOGTIME__  Logger::GetInstance()->GetLogCoutTime()          //ʱ���
#define __LOGPID__   Logger::GetInstance()->GetLogCoutProcessId()     //���̺�
#define __LOGTID__   Logger::GetInstance()->GetLogCoutThreadId()      //�̺߳�
#define __LOGFILE__  __FILE__          //�ļ�����
#define __LOGFUNC__  __func__          //��������
#define __LOGLINE__  __LINE__          //������

const std::string SWITCH_ON = "on";
const std::string SWITCH_OFF = "off";

struct Log {
    std::string logSwitch;           //��־����
    std::string logFileSwitch;       //�Ƿ�д���ļ�
    std::string logTerminalSwitch;   //�Ƿ��ӡ���ն�
    std::string logName;             //��־�ļ�����
    std::string logFilePath;         //��־�ļ�����·��
    std::string logMixSize;          //��־�ļ�����С
    std::string logBehavior;         //��־�ļ��ﵽ����С��Ϊ
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