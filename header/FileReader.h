#include <iostream>
#include "Common.h"

#ifndef FILEREADER_H
#define FILEREADER_H

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