#include <iostream>
#include "Common.h"
#include "Logger.h"
#ifndef FILEREADER_H
#define FILEREADER_H

class FileReader {
public:
    void ReadImage(const std::string& imageName);

    Image GetImage();

    void WriteImage(Image image, const std::string& imageName);

private:
    void ReadBitmap(FILE* fp);

private:
    Image mImage;
    ImageHeader mImageHeader;
};

#endif // FILEREADER_H