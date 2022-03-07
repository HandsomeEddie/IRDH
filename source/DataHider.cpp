#include "../header/DataHider.h"

#pragma region class DataHider

DataHider::DataHider(const Image &image) {
    mMaskImage = image;
}

void DataHider::EmbedData(const std::string &data) {
    ShiftHist();
    Modify(data);
    mHidingKey.dataLength = data.size();
    mHidingKey.peakPoint = mPeakPoint;
    mHidingKey.zeroPoint = mZeroPoint;
}

std::string DataHider::Transfer(const std::string &data) {
    return "";
}

void DataHider::ShiftHist() {
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

    for (int i = 0; i < mMaskImage.width; ++i) {
        for (int j = 0; j < mMaskImage.height; ++j) {
            unsigned char pixel = mMaskImage.matrix[i][j];
            // shift right while peakPoint is located at the left of zeroPoint
            if (mPeakPoint < mZeroPoint && pixel > mPeakPoint && pixel < mZeroPoint) {
                mMaskImage.matrix[i][j]++;
            }
            // shift left while peakPoint is located at the right of zeroPoint
            if (mZeroPoint < mPeakPoint && pixel > mZeroPoint && pixel < mPeakPoint) {
                mMaskImage.matrix[i][j]--;
            }
        }
    }
}

void DataHider::Modify(const std::string &data) {
    int count = 0;

    int kkk = 0;
    for (int i = 0; i < mMaskImage.width; ++i) {
        for (int j = 0; j < mMaskImage.height; ++j) {
            unsigned char pixel = mMaskImage.matrix[i][j];
            if (pixel == mPeakPoint) {
                int value = data[count++] - '0';
                // Increment while peakPoint is located at the left of zeroPoint
                if (mPeakPoint < mZeroPoint) {
                    mMaskImage.matrix[i][j] += value;
                }
                // Decrement while peakPoint is located at the right of zeroPoint
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