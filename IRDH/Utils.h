#include "Common.h"
#include <direct.h>
#include <bitset>
#include <mutex>

#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
    static Utils* GetInstance();

    void MoveHist(Image& image, int val, int left, int right);

    std::vector<std::bitset<8>> DataStrToBits(const std::string& data);

    std::string BitsToDataStr(const std::vector<std::bitset<8>>& bits);

    std::string GetPath();

private:
    static Utils* singleObject;
    static std::mutex* mutexUtil;
    
};




#endif // UTILS_H