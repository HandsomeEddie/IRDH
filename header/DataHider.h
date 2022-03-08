#include <string>
#include "Common.h"
#include "Utils.h"

#ifndef DATAHIDER_H
#define DATAHIDER_H

class DataHider {
public:
    void Init(const Image &image, const std::string &data);
    void Embed();
    Image GetImage() { return mMaskImage;}
    HidingKey GetKey() { return mHidingKey; }

private:
    void CalculatePoints();
    void ShiftHist();
    void Modify();
    void SetKey();

    unsigned char mZeroPoint;
    int mZeroNum = INT_MAX;
    unsigned char mPeakPoint;
    int mPeakNum = 0;

    Image mMaskImage;
    std::vector<std::bitset<8>> mBits;
    HidingKey mHidingKey;
};

#endif // DATAHIDER_H