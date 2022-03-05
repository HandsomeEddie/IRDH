#include <iostream>
#include <vector>
#include <Windows.h>

struct Image {
    int width;
    int height;
    std::vector<std::vector<unsigned char>> data;
};

struct ImageHeader {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD *rgbTable;
};

class FileReader {
public:
    bool ReadImage(const std::string &imageName);
    Image GetImage() { return mImage; }
    void SetImage(Image image);

private:
    void ReadBitmap(FILE *fp);
    Image mImage;
    ImageHeader mImageHeader;

};