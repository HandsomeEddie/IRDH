#include "Common.h"
#include "Utils.h"

#ifndef RECEIVER_H
#define RECEIVER_H

class Receiver {
public:
    void Init(const Image &maskImage, const HidingKey &hidingKey);
    void Extract();
    Image GetImage() { return mRecoverImage; }
    std::string GetData() { return mData; }

private:
    void Modify();
    void ShiftHist();
    Image mRecoverImage;
    std::string mData;
    HidingKey mHidingKey;
};

#endif // RECEIVER_H