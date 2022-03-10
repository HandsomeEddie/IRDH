#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <mutex>

#ifndef LOGGER_H
#define LOGGER_H

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