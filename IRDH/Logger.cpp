#include "Logger.h"

Logger* Logger::mSingleObject = nullptr;
std::mutex* Logger::mMutexLog = new std::mutex;

Logger::Logger() {
    InitLogConfig();
}

Logger* Logger::GetInstance() {
    mMutexLog->lock();
    if (mSingleObject == nullptr) {
        mSingleObject = new Logger();
    }
    mMutexLog->unlock();
    return mSingleObject;
}

void Logger::InitLogConfig() {
    std::map<std::string, std::string*> logConfInfo;
    logConfInfo["logSwitch"] = &this->log.logSwitch;
    logConfInfo["logFileSwitch"] = &this->log.logFileSwitch;
    logConfInfo["logTerminalSwitch"] = &this->log.logTerminalSwitch;
    logConfInfo["logName"] = &this->log.logName;
    logConfInfo["logFilePath"] = &this->log.logFilePath;
    logConfInfo["logMixSize"] = &this->log.logMixSize;
    logConfInfo["logBehavior"] = &this->log.logBehavior;

    mLogTypeMap[LogType::ErrorType] = "Error";
    mLogTypeMap[LogType::WarnType] = "Warn";
    mLogTypeMap[LogType::InfoType] = "Info";
    mLogTypeMap[LogType::DebugType] = "Debug";
    mLogTypeMap[LogType::TraceType] = "Trace";

    bool isOpen = true;
    std::ifstream file;
    std::string filePath = Utils::GetInstance()->GetPath();
    file.open(filePath + "/../config/LogConfig.conf");
    if (!file.is_open()) {
        std::cout << "Failed" << std::endl;
        isOpen = false;
    }

    std::string str;
    char strC[100] = { 0 };

    while (std::getline(file, str)) {
        if (!str.size()) {
            continue;
        }
        std::string strTemp = str;
        int j = 0;
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == ' ') {
                continue;
            }
            strTemp[j] = str[i];
            j++;
        }
        strTemp.erase(j);
        if (strTemp[0] != '#') {
            sscanf_s(strTemp.data(), "%[^=]", strC, (unsigned int)strTemp.size());
            auto iter = logConfInfo.find(strC);
            if (iter != logConfInfo.end()) {
                sscanf_s(strTemp.data(), "%*[^=]=%s", strC, (unsigned int)strTemp.size());
                *iter->second = strC;
            }
        }
    }
    file.close();

    if (isOpen) {
        PrintLogConfig();
    }
}

void Logger::PrintLogConfig() {
    for (int i = 0; i < log.logFilePath.size() + 15; ++i) {
        std::cout << "-";
        if (i == (log.logFilePath.size() + 15) / 2) {
            std::cout << "Logger";
        }
    }

    std::cout << std::endl;
    std::cout << std::left << std::setw(25) << "  日志开关　　" << log.logSwitch << std::endl;
    std::cout << std::left << std::setw(25) << "  文件输出　　" << log.logFileSwitch << std::endl;
    std::cout << std::left << std::setw(25) << "  终端输出开关" << log.logTerminalSwitch << std::endl;
    std::cout << std::left << std::setw(25) << "  日志文件名称" << log.logName << ".log" << std::endl;
    std::cout << std::left << std::setw(25) << "  日志保存路径" << log.logFilePath << std::endl;
    std::cout << std::left << std::setw(25) << "  日志文件大小" << log.logMixSize << "M" << std::endl;
    for (int i = 0; i < log.logFilePath.size() + 25; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
}

std::string Logger::GetLogCoutTime() {
    struct tm t;
    time_t now;
    time(&now);
    localtime_s(&t, &now);

    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", &t);
    std::string tmpStr = tmp;
    return "[" + tmpStr + "]";
}

std::string Logger::GetLogCoutProcessId() {
    return std::to_string(_getpid());
}

std::string Logger::GetLogCoutThreadId() {
    return std::to_string(GetCurrentThreadId());
}

std::string Logger::GetLogCoutUserName() {
    const int MAX_LEN = 100;
    TCHAR szBuffer[MAX_LEN];
    DWORD len = MAX_LEN;
    GetUserName(szBuffer, &len);

    int iLen = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL);
    char* chRtn = new char[iLen * sizeof(char)];
    WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, chRtn, iLen, NULL, NULL);
    std::string str(chRtn);
    return " " + str + " ";
}

std::string Logger::GetFilePathAndName() {
    return log.logFilePath + "/" + log.logName;
}

void Logger::WriteLogIntoFile(LogType logType, std::string message) {
    std::string messageAll = __LOGTIME__ + mLogTypeMap[logType] + " " + message;
    messageAll = messageAll + " PID: " + __LOGPID__ + " TID: " + __LOGTID__;
    messageAll = messageAll + "[" + __LOGFUNC__ + ":" + std::to_string(__LOGLINE__) + "]";
    std::string fileName = GetFilePathAndName();
    FILE* fp;
    fopen_s(&fp, fileName.c_str(), "rb");
    if (nullptr == fp) {
        return;
    }
    fwrite(&messageAll, messageAll.size(), 1, fp);
}

void Logger::WriteLogIntoTerminal(LogType logType, std::string message) {
    std::string messageAll = __LOGTIME__ + mLogTypeMap[logType] + " " + message;
    messageAll = messageAll + " PID: " + __LOGPID__ + " TID: " + __LOGTID__;
    messageAll = messageAll + "[" + __LOGFUNC__ + ":" + std::to_string(__LOGLINE__) + "]";
    std::cout << messageAll << std::endl;
}

void Logger::WriteLog(LogType logType, std::string message) {
    if (SWITCH_ON == log.logSwitch) {
        if (SWITCH_OFF != log.logFileSwitch) {
            WriteLogIntoFile(logType, message);
        }
        if (SWITCH_OFF != log.logTerminalSwitch) {
            WriteLogIntoTerminal(logType, message);
        }
    }
}

void Logger::LogWarn(std::string message) {
    WriteLog(LogType::WarnType, message);
}