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

    std::string GetLogSwitch();

    std::string GetLogFileSwitch();

    std::string GetLogTerminalSwitch();

    std::string GetLogType(LogType logType);

    void test();

private:
    Log log;
    static Logger* mSingleObject;
    static std::mutex* mMutexLog;
    std::map<LogType, std::string> mLogTypeMap;

private:
    Logger();
};

#define WRITE_INTO_FILE(logType, message)\
    do {\
        std::string fileName = Logger::GetInstance()->GetFilePathAndName();\
        std::ofstream file; \
        file.open(fileName.c_str(), std::ios::app);\
        if (file) {\
            std::string messageAll = __LOGTIME__ + Logger::GetInstance()->GetLogType(logType) + " ";\
            messageAll = messageAll + "[" + __LOGFUNC__ + ": " + std::to_string(__LOGLINE__) + "]";\
            messageAll = messageAll + " PID: " + __LOGPID__ + " TID : " + __LOGTID__ + "\n";\
            messageAll = messageAll + "Remark: " + message + "\n";\
            file << messageAll;\
            file.close();\
        }\
    } while (0);

#define WRITE_INTO_TERMINAL(logType, message)\
    do {\
        std::string messageAll = __LOGTIME__ + Logger::GetInstance()->GetLogType(logType) + " ";\
        messageAll = messageAll + "[" + __LOGFUNC__ + ": " + std::to_string(__LOGLINE__) + "]";\
        messageAll = messageAll + " PID: " + __LOGPID__ + " TID : " + __LOGTID__ + "\n";\
        messageAll = messageAll + "Remark: " + message + "\n";\
        std::cout << messageAll;\
    } while (0);

#define WRITE_LOG(logType, message)\
    do {\
        if (SWITCH_ON == Logger::GetInstance()->GetLogSwitch()) {\
            if (SWITCH_OFF != Logger::GetInstance()->GetLogFileSwitch()) {\
                WRITE_INTO_FILE(logType, message)\
            }\
            if (SWITCH_OFF != Logger::GetInstance()->GetLogTerminalSwitch()) {\
                WRITE_INTO_TERMINAL(logType, message)\
            }\
        }\
    } while (0);

#define LOGERROR(message) WRITE_LOG(LogType::ErrorType, message)

#define LOGWARN(message) WRITE_LOG(LogType::WarnType, message)

#define LOGINFO(message) WRITE_LOG(LogType::InfoType, message)

#define LOGDEBUG(message) WRITE_LOG(LogType::DebugType, message)

#define LOGTRACE(message) WRITE_LOG(LogType::TraceType, message)



#endif // LOGGER_H