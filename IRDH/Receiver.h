#include "Common.h"
#include "Utils.h"

#ifndef RECEIVER_H
#define RECEIVER_H

class Receiver {
public:
    void Init(const Image& maskImage, const HidingKey& hidingKey);

    void Extract();

    Image GetImage();

    std::string GetData();

private:
    void Modify();
    void ShiftHist();

private:
    Image mRecoverImage;
    std::string mData;
    HidingKey mHidingKey;
};

#endif // RECEIVER_H