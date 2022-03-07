#include "../header/DataHider.h"

#pragma region class DataHider

DataHider::DataHider(const Image &image) {
    mMaskImage = image;
}

void DataHider::EmbedData(const std::string &data) {
    CalculatePoints();
    ShiftHist();
    Modify(data);

    mHidingKey.dataLength = data.size();
    mHidingKey.peakPoint = mPeakPoint;
    mHidingKey.zeroPoint = mZeroPoint;
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
    Utils utils;
    if (mPeakPoint < mZeroPoint) {
        utils.MoveHist(mMaskImage, 1, mPeakPoint, mZeroPoint);
    }
    else {
        utils.MoveHist(mMaskImage, -1, mZeroPoint, mPeakPoint);
    }
}

// 1.Pixels are added with val while peak < zero
// 2.pixels are subtracted with val while zero < peak
void DataHider::Modify(const std::string &data) {
    int count = 0;

    for (int i = 0; i < mMaskImage.width; ++i) {
        for (int j = 0; j < mMaskImage.height; ++j) {
            unsigned char pixel = mMaskImage.matrix[i][j];
            if (pixel == mPeakPoint) {
                int value = data[count++] - '0';
                if (mPeakPoint < mZeroPoint) {
                    mMaskImage.matrix[i][j] += value;
                }
                if (mZeroPoint < mPeakPoint) {
                    mMaskImage.matrix[i][j] -= value;
                }

                if (count >= data.size()) {
                    return;
                }
            }
        }
    }
}

#pragma endregion