#include "Utils.h"

Utils* Utils::singleObject = nullptr;

std::mutex* Utils::mutexUtil = new std::mutex;

Utils* Utils::GetInstance() {
    mutexUtil->lock();
    if (singleObject == nullptr) {
        singleObject = new Utils();
    }
    mutexUtil->unlock();
    return singleObject;
}

// Pixels between (left, right) are added with val
void Utils::MoveHist(Image& image, int val, int left, int right) {
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            unsigned char pixel = image.matrix[i][j];
            if (pixel > left && pixel < right) {
                image.matrix[i][j] += val;
            }
        }
    }
}

std::vector<std::bitset<8>> Utils::DataStrToBits(const std::string& data) {
    std::vector<std::bitset<8>> bits(data.size());
    for (int i = 0; i < data.size(); ++i) {
        bits[i] = std::bitset<8>(data[i]);
    }

    return bits;
}

std::string Utils::BitsToDataStr(const std::vector<std::bitset<8>>& bits) {
    std::string data = "";
    for (int i = 0; i < bits.size(); ++i) {
        unsigned long num = bits[i].to_ulong();
        data += (char)num;
    }
    return data;
}

std::string Utils::GetPath() {
    char* buffer;
    buffer = _getcwd(NULL, 0);
    return buffer;
}