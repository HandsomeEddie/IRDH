#include "../header/Receiver.h"

Receiver::Receiver(const Image &maskImage, const HidingKey &hidingKey) {
    mRecoverImage = maskImage;
    mHidingKey = hidingKey;
}

void Receiver::Extract() {
    Modify();
    ShiftHist();
}

void Receiver::ShiftHist(){
    for (int i = 0; i < mRecoverImage.width; ++i) {
        for (int j = 0; j < mRecoverImage.height; ++j) {
            unsigned char pixel = mRecoverImage.matrix[i][j];
            // shift left while peakPoint is located at the left of zeroPoint
            if (mHidingKey.peakPoint < mHidingKey.zeroPoint && pixel > mHidingKey.peakPoint + 1 && pixel < mHidingKey.zeroPoint + 1) {
                mRecoverImage.matrix[i][j]--;
            }
            // shift left while peakPoint is located at the right of zeroPoint
            if (mHidingKey.zeroPoint < mHidingKey.peakPoint && pixel > mHidingKey.zeroPoint - 1 && pixel < mHidingKey.peakPoint - 1) {
                mRecoverImage.matrix[i][j]++;
            }
        }
    }
}

void Receiver::Modify() {
    int count = 0;

    for (int i = 0; i < mRecoverImage.width; ++i) {
        for (int j = 0; j < mRecoverImage.height; ++j) {
            unsigned char pixel = mRecoverImage.matrix[i][j];
            // Keep pixel unchanged and extract '0'
            if (pixel == mHidingKey.peakPoint) {
                mData += '0';
                count++;
            }
            // Modify pixel and extract '1'
            else if (pixel == mHidingKey.peakPoint + 1 && mHidingKey.peakPoint < mHidingKey.zeroPoint) {
                // Decrement while peakPoint is located at the left of zeroPoint
                mRecoverImage.matrix[i][j]--;
                mData += '1';
                count++;
            } 
            else if (pixel == mHidingKey.peakPoint - 1 && mHidingKey.peakPoint > mHidingKey.zeroPoint) {
                // Increment while peakPoint is located at the right of zeroPoint
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