#include <iostream>
#include <direct.h>
#include "FileReader.h"
#include "DataHider.h"
#include "Receiver.h"
#include "Utils.h"
#include "Logger.h"

int main() {
    // ---------------------------- Content owner ----------------------------
    FileReader fileReader;
    std::string imageName = "C:/Users/fudis/Documents/GitHub/IRDH/image/Airplane.bmp";
    std::string data = "Hello World";

    if (false == fileReader.ReadImage(imageName)) {
        std::cout << "Error file reading" << std::endl;
        return 1;
    }

    Image originImage = fileReader.GetImage();

    // ---------------------------- Data hider ----------------------------
    DataHider dataHider;
    dataHider.Init(originImage, data);
    dataHider.Embed();
    Image maskImage = dataHider.GetImage();
    HidingKey hidingKey = dataHider.GetKey();

    fileReader.WriteImage(maskImage, "C:/Users/fudis/Documents/GitHub/IRDH/image/MaskImage.bmp");

    // ---------------------------- Receiver ----------------------------
    Receiver receiver;
    receiver.Init(maskImage, hidingKey);
    receiver.Extract();
    std::string extractData = receiver.GetData();
    Image recoverImage = receiver.GetImage();

    fileReader.WriteImage(recoverImage, "C:/Users/fudis/Documents/GitHub/IRDH/image/RecoverImage.bmp");

    // ------------------------------ test ----------------------------------

    std::cout << data << std::endl;
    std::cout << extractData << std::endl;

    //Logger logger;
    //logger.InitLogConfig();
    //logger.PrintLogConfig();


    std::cout << "Over~";
}