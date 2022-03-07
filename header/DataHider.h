#include <string>
#include "Common.h"
#include "Utils.h"

#ifndef DATAHIDER_H
#define DATAHIDER_H

class DataHider {
public:
    DataHider(const Image &image);
    void EmbedData(const std::string &data);
    Image GetImage() { return mMaskImage;}
    HidingKey GetKey() { return mHidingKey; }
private:
    void CalculatePoints();
    void ShiftHist();
    void Modify(const std::string &data);

    unsigned char mZeroPoint;
    int mZeroNum = INT_MAX;
    unsigned char mPeakPoint;
    int mPeakNum = 0;

    Image mMaskImage;
    HidingKey mHidingKey;
};

#endif // DATAHIDER_H