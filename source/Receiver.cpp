#include "../header/Receiver.h"

Receiver::Receiver(const Image &maskImage, const HidingKey &hidingKey) {
    mRecoverImage = maskImage;
    mHidingKey = hidingKey;
}

void Receiver::Extract() {
    Modify();
    ShiftHist();
}

void Receiver::ShiftHist() {
    Utils utils;
    if (mHidingKey.peakPoint < mHidingKey.zeroPoint) {
        utils.MoveHist(mRecoverImage, -1, mHidingKey.peakPoint + 1, mHidingKey.zeroPoint + 1);
    }
    else {
        utils.MoveHist(mRecoverImage, 1, mHidingKey.zeroPoint - 1, mHidingKey.peakPoint - 1);
    }
}

// 1.'0' is extracted while pixel == peak
// 2.'1' is extracted while pixel == peak + 1 and peak < zero
// 3.'1' is extracted while pixel == peak - 1 and zero < peak
void Receiver::Modify() {
    int count = 0;

    for (int i = 0; i < mRecoverImage.width; ++i) {
        for (int j = 0; j < mRecoverImage.height; ++j) {
            unsigned char pixel = mRecoverImage.matrix[i][j];
            if (pixel == mHidingKey.peakPoint) {
                mData += '0';
                count++;
            }
            else if (pixel == mHidingKey.peakPoint + 1 && mHidingKey.peakPoint < mHidingKey.zeroPoint) {
                mRecoverImage.matrix[i][j]--;
                mData += '1';
                count++;
            } 
            else if (pixel == mHidingKey.peakPoint - 1 && mHidingKey.peakPoint > mHidingKey.zeroPoint) {
                mRecoverImage.matrix[i][j]++;
                mData += '1';
                count++;
            }

            if (count >= mHidingKey.dataLength) {
                return;
            }
        }
    }
}