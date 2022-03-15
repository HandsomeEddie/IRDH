#include "FileReader.h"

void FileReader::ReadImage(const std::string& imageName) {
    std::string imageNameSuffix = imageName.substr(imageName.find_last_of('.') + 1);
    if (imageNameSuffix != "bmp") {
        LOGWARN("Only support bmp file");
    }

    FILE* fp;
    fopen_s(&fp, imageName.c_str(), "rb");
    if (nullptr == fp) {
        LOGERROR("Read file error: " + imageName);
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
}

Image FileReader::GetImage() {
    return mImage;
}

void FileReader::WriteImage(Image image, const std::string& imageName) {
    FILE* fp;
    fopen_s(&fp, imageName.c_str(), "wb");
    if (nullptr == fp) {
        LOGERROR("Write file error: " + imageName);
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

    int len = mImage.width * mImage.height;
    unsigned char (*data) = new unsigned char[len];

    unsigned char* point = data;

    for (int i = 0; i < mImage.width; ++i) {
        for (int j = 0; j < mImage.width; ++j) {
            if (isUpwards) {
                int index = abs(image.height) - i - 1;
                *point = image.matrix[index][j];
               // data[count++] = image.matrix[index][j];
            }
            else {
                *point = image.matrix[i][j];
                //data[count++] = image.matrix[i][j];
            }
            point += 1;
        }
    }

    fwrite(&mImageHeader.fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&mImageHeader.infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    if (mImageHeader.infoHeader.biBitCount == 8) {
        fwrite(&mImageHeader.rgbTable, sizeof(RGBQUAD), 256, fp);
    }

    len = mImage.height * linebyte;
    fwrite(data, len, 1, fp);
    delete[] data;

    fclose(fp);
}

void FileReader::ReadBitmap(FILE* fp) {
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
                unsigned index = abs(mImage.height) - i - 1;
                fread(&mImage.matrix[index][j], sizeof(unsigned char), 1, fp);
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