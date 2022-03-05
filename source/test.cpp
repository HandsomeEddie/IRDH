#include <iostream>
#include "../header/FileReader.h"

int main() {
    FileReader fileReader;
    fileReader.ReadImage("C:/Users/fudis/Documents/GitHub/IRDH/image/Airplane.bmp");
    Image image = fileReader.GetImage();
    std::cout << "bye~";
}