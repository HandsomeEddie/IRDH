#include <iostream>
#include "../header/DataHider.h"
#include "../header/Receiver.h"

int main() {
    FileReader fileReader;
    fileReader.ReadImage("C:/Users/fudis/Documents/GitHub/IRDH/image/Airplane.bmp");
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

    std::cout << "bye~" << std::endl;
    std::cout << data << std::endl;
    std::cout << extractData;
}