#include "test.h"

int main() {
    // ---------------------------- Content owner ----------------------------
    FileReader fileReader;
    std::string filePath = Utils::GetInstance()->GetPath();
    std::string imageName = filePath + "/../image/Airplane.bmp";
    //std::string imageName = "C:/Users/fudis/Documents/GitHub/IRDH/image/Airplane.bmp";
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

    fileReader.WriteImage(maskImage, filePath + "/../image/MaskImage.bmp");

    // ---------------------------- Receiver ----------------------------
    Receiver receiver;
    receiver.Init(maskImage, hidingKey);
    receiver.Extract();
    std::string extractData = receiver.GetData();
    Image recoverImage = receiver.GetImage();

    fileReader.WriteImage(recoverImage, filePath + "/../image/RecoverImage.bmp");

    // ------------------------------ test ----------------------------------

    std::cout << "The embedded data is: " << data << std::endl;
    std::cout << "The extracted data is: " << extractData << std::endl;


    Logger::GetInstance()->PrintLogConfig();



    std::cout << "Over~";
}