#include <iostream>
#include <direct.h>
#include "../header/FileReader.h"
#include "../header/DataHider.h"
#include "../header/Receiver.h"

int main() {
    FileReader fileReader;
    
    char buffer[256];
    char *val = getcwd(buffer, sizeof(buffer));
    fileReader.ReadImage("C:/Users/fudis/Documents/GitHub/IRDH/image/Airplane.bmp");
    //fileReader.ReadImage("C:/Users/fudis/Documents/GitHub/IRDH/image/Airplane.bmp");
    Image originImage = fileReader.GetImage();

    std::string data = "0101010101010101001001010101010100101010101001010101010010101011101010100110";
    DataHider dataHider(originImage);
    dataHider.EmbedData(data);
    Image maskImage = dataHider.GetImage();
    fileReader.WriteImage(maskImage, "C:/Users/fudis/Documents/GitHub/IRDH/image/MaskImage.bmp");
    HidingKey hidingKey = dataHider.GetKey();

    Receiver receiver(maskImage, hidingKey);
    receiver.Extract();
    std::string extractData = receiver.GetData();
    Image recoverImage = receiver.GetImage();
    fileReader.WriteImage(recoverImage, "C:/Users/fudis/Documents/GitHub/IRDH/image/RecoverImage.bmp");

    std::cout << data << std::endl;
    std::cout << extractData << std::endl;

    char ch = 'H';
    std::string tempStr;
    for (int i = 0; i < 8; ++i) {
        tempStr += (((ch >> i) & 1) + '0');
    }

    short num = 0;
    for (int i = 0; i < 8; ++i) {
        num = num << 1;
        num ^= (tempStr[8 - i - 1] - '0'); 
    }
    char tempCh = num;
    std::cout << tempCh;
}