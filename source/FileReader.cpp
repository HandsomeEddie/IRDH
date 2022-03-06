#include "../header/FileReader.h"


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
    fclose(fp);
    return true;
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

    mImage.matrix.resize(mImage.height, std::vector<unsigned char>(mImage.width));
    for (int i = 0; i < abs(mImage.height); ++i) {
        for (int j = 0; j < mImage.width; ++j) {
            if (isUpwards) {
                fread(&mImage.matrix[abs(mImage.height) - i - 1][j], sizeof(unsigned char), 1, fp);
            }
            else {
                fread(&mImage.matrix[i][j], sizeof(unsigned char), 1, fp);
            }
        }

        for (int j = 0; j < offset; ++j) {
            unsigned char temp;
            fread(&temp, sizeof(unsigned char), 1, fp);
        }
    }
}

bool FileReader::WriteImage(Image image, const std::string &imageName) {
    FILE *fp = fopen(imageName.c_str(), "wb");
    if (nullptr == fp) {
        return false;
    }

    int linebyte = (mImageHeader.infoHeader.biHeight * mImageHeader.infoHeader.biBitCount / 8 + 3) / 4 * 4;
    int offset = linebyte % 4;
    if (offset != 0) {
        offset = 4 - offset; // "linebyte" must be the mutiple of 4
    }   

    bool isUpwards = false;
    if (mImage.height > 0) {
        isUpwards = true; // Read the pixel data from bottom to up
    }

    unsigned char *data = new unsigned char[mImage.width * mImage.height];
    int count = 0;
    for (int i = 0; i < mImage.width; ++i) {
        for (int j = 0; j < mImage.width; ++j) {
            if (isUpwards) {
                data[count++] = image.matrix[abs(mImage.height) - i - 1][j];
            }
            else {
                data[count++] = image.matrix[i][j];
            }
        }
    }

    fwrite(&mImageHeader.fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&mImageHeader.infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    if (mImageHeader.infoHeader.biBitCount == 8) {
        fwrite(&mImageHeader.rgbTable, sizeof(RGBQUAD), 256, fp);
    }
    fwrite(data, mImage.height * linebyte, 1, fp);
    delete data;

    fclose(fp);
    return true;
}
