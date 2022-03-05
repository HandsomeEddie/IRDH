#include "FileReader.h"

bool FileReader::ReadImage(const std::string &imageName) {
    std::string imageNameSuffix = imageName.substr(imageName.find_last_of('.') + 1);
    if (imageNameSuffix != "bmp") {
        return false; // Only support reading bmp image
    }

    FILE *fp = fopen(imageName.c_str(), "rb");
    if (nullptr == fp) {
        return false;; // Cannot open file
    }

    fread(&mImageHeader.fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&mImageHeader.infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    if (mImageHeader.infoHeader.biBitCount == 8) {
        fread(&mImageHeader.rgbTable, sizeof(RGBQUAD), 256, fp);
    }

    mImage.height = mImageHeader.infoHeader.biHeight;
    mImage.width = mImageHeader.infoHeader.biHeight;

    ReadBitmap(fp);
}

void FileReader::ReadBitmap(FILE *fp) {
    int linebyte = mImageHeader.infoHeader.biHeight * mImageHeader.infoHeader.biBitCount / 8;
    int offset = linebyte % 4;
    if (offset != 0) {
        offset = 4 - offset; // "linebyte" must be the mutiple of 4
    }

    bool isUpwards = false;
    if (mImage.height > 0) {
        isUpwards = true; // Read the pixel data from bottom to up
    }

    mImage.data.resize(mImage.height, std::vector<unsigned char>(mImage.width));
    for (int i = 0; i < abs(mImage.height); ++i) {
        for (int j = 0; j < mImage.width; ++j) {
            if (isUpwards) {
                fread(&mImage.data[abs(mImage.height) - i - 1][j], sizeof(unsigned char), 1, fp);
            }
            else {
                fread(&mImage.data[i][j], sizeof(unsigned char), 1, fp);
            }
        }

        for (int j = 0; j < offset; ++j) {
            unsigned char temp;
            fread(&temp, sizeof(unsigned char), 1, fp);
        }
    }
    
}

void FileReader::SetImage(Image image) {

}


