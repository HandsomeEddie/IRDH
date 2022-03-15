#include "Receiver.h"

void Receiver::Init(const Image& maskImage, const HidingKey& hidingKey) {
    mRecoverImage = maskImage;
    mHidingKey = hidingKey;
}

void Receiver::Extract() {
    Modify();
    ShiftHist();
}

Image Receiver::GetImage() {
    return mRecoverImage;
}

std::string Receiver::GetData() {
    return mData;
}

// 1.'0' is extracted while pixel == peak
// 2.'1' is extracted while pixel == peak + 1 and peak < zero
// 3.'1' is extracted while pixel == peak - 1 and zero < peak
void Receiver::Modify() {
    int count = 0;
    std::vector<std::bitset<8>> bits(mHidingKey.dataLength / 8);

    for (int i = 0; i < mRecoverImage.width; ++i) {
        for (int j = 0; j < mRecoverImage.height; ++j) {
            unsigned char pixel = mRecoverImage.matrix[i][j];
            int sets = count / 8;
            int index = count % 8;

            if (pixel == mHidingKey.peakPoint) {
                // bits[sets][index] = '0'; Default 0
                count++;
            }
            else if (pixel == mHidingKey.peakPoint + 1 && mHidingKey.peakPoint < mHidingKey.zeroPoint) {
                mRecoverImage.matrix[i][j]--;
                bits[sets].set(index);
                //mData += '1';
                count++;
            }
            else if (pixel == mHidingKey.peakPoint - 1 && mHidingKey.peakPoint > mHidingKey.zeroPoint) {
                mRecoverImage.matrix[i][j]++;
                bits[sets].set(index);
                //mData += '1';
                count++;
            }

            if (count >= mHidingKey.dataLength * 8) {
                Utils utils;
                mData = utils.BitsToDataStr(bits);
                return;
            }
        }
    }
}

void Receiver::ShiftHist() {
    //Utils utils;
    if (mHidingKey.peakPoint < mHidingKey.zeroPoint) {
        Utils::GetInstance()->MoveHist(mRecoverImage, -1, mHidingKey.peakPoint + 1, mHidingKey.zeroPoint + 1);
    }
    else {
        Utils::GetInstance()->MoveHist(mRecoverImage, 1, mHidingKey.zeroPoint - 1, mHidingKey.peakPoint - 1);
    }
}