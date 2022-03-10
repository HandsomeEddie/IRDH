#include "Common.h"
#include <bitset>

#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
    void MoveHist(Image& image, int val, int left, int right);

    std::vector<std::bitset<8>> DataStr2Bits(const std::string& data);

    std::string Bits2DataStr(const std::vector<std::bitset<8>>& bits);

private:

};




#endif // UTILS_H