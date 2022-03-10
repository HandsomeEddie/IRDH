#include "Logger.h"

Logger* Logger::singleObject = nullptr;
std::mutex* Logger::mutexLog = new std::mutex;

Logger::Logger() {
    InitLogConfig();
}

Logger* Logger::GetInstance() {
    mutexLog->lock();
    if (singleObject == nullptr) {
        singleObject = new Logger();
    }
    mutexLog->unlock();
    return singleObject;
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

    std::ifstream file;
    std::string filePath = Utils::GetInstance()->GetPath();
    file.open(filePath + "/../config/LogConfig.conf");
    if (!file.is_open()) {
        std::cout << "Failed" << std::endl;
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

