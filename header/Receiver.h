#include "FileReader.h"
#include "DataHider.h"

#ifndef RECEIVER_H
#define RECEIVER_H

class Receiver {
public:
    Receiver(const Image &maskImage, const HidingKey &hidingKey);
    void Extract();
    Image GetImage() { return mRecoverImage; }
    std::string GetData() { return mData; }
private:
    void ShiftHist();
    void Modify();

    Image mRecoverImage;
    std::string mData;
    HidingKey mHidingKey;
};

#endif // RECEIVER_H