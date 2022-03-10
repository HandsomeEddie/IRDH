#include "DataHider.h"

#pragma region class DataHider

void DataHider::Init(const Image& image, const std::string& data) {
    mMaskImage = image;
    mBits = Utils::GetInstance()->DataStr2Bits(data);
}

void DataHider::Embed() {
    CalculatePoints();
    ShiftHist();
    Modify();

    SetKey();
}

Image DataHider::GetImage() {
    return mMaskImage;
}

HidingKey DataHider::GetKey() {
    return mHidingKey;
}

void DataHider::CalculatePoints() {
    // Calculate the histogram of image
    std::vector<int> hist(256, 0);
    for (int i = 0; i < mMaskImage.width; ++i) {
        for (int j = 0; j < mMaskImage.height; ++j) {
            unsigned char pixel = mMaskImage.matrix[i][j];
            hist[pixel]++;
        }
    }

    for (int i = 0; i < 256; ++i) {
        if (hist[i] < mZeroNum) {
            mZeroNum = hist[i];
            mZeroPoint = i;
        }
        else if (hist[i] > mPeakNum) {
            mPeakNum = hist[i];
            mPeakPoint = i;
        }
    }
}

// 1.turn right while peak < zero
// 2.turn left while zero < peak
void DataHider::ShiftHist() {
    //Utils utils;
    if (mPeakPoint < mZeroPoint) {
        Utils::GetInstance()->MoveHist(mMaskImage, 1, mPeakPoint, mZeroPoint);
    }
    else {
        Utils::GetInstance()->MoveHist(mMaskImage, -1, mZeroPoint, mPeakPoint);
    }
}

// 1.Pixels are added with val while peak < zero
// 2.pixels are subtracted with val while zero < peak
void DataHider::Modify() {
    int count = 0;

    for (int i = 0; i < mMaskImage.width; ++i) {
        for (int j = 0; j < mMaskImage.height; ++j) {
            unsigned char pixel = mMaskImage.matrix[i][j];
            if (pixel == mPeakPoint) {
                int sets = count / 8;
                int index = count % 8;
                int value = mBits[sets][index];
                count++;

                if (mPeakPoint < mZeroPoint) {
                    mMaskImage.matrix[i][j] += value;
                }
                if (mZeroPoint < mPeakPoint) {
                    mMaskImage.matrix[i][j] -= value;
                }

                if (count >= mBits.size() * 8) {
                    return;
                }
            }
        }
    }
}

void DataHider::SetKey() {
    mHidingKey.dataLength = (int)mBits.size() * 8;
    mHidingKey.peakPoint = mPeakPoint;
    mHidingKey.zeroPoint = mZeroPoint;
}

#pragma endregion