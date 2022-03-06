#include <iostream>
#include <vector>
#include <Windows.h>

#ifndef FILEREADER_H
#define FILEREADER_H

struct Image {
    int width;
    int height;
    std::vector<std::vector<unsigned char>> matrix;
};

struct ImageHeader {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD rgbTable[256];
};

class FileReader {
public:
    bool ReadImage(const std::string &imageName);
    Image GetImage() { return mImage; }
    bool WriteImage(Image image, const std::string &imageName);

private:
    void ReadBitmap(FILE *fp);
    Image mImage;
    ImageHeader mImageHeader;

};

#endif // FILEREADER_H