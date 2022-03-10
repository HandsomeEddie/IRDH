#include <iostream>
#include <windows.h>
#include <vector>

#ifndef COMMON_H
#define COMMON_H

struct HidingKey {
    unsigned char peakPoint;
    unsigned char zeroPoint;
    int dataLength;
};

struct Image {
    int width = 0;
    int height = 0;
    std::vector<std::vector<unsigned char>> matrix;
};

struct ImageHeader {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD rgbTable[256];
};



#endif // COMMON_H