#include "../header/Utils.h"

// Pixels between (left, right) are added with val
void Utils::MoveHist(Image &image, int val, int left, int right) {
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            unsigned char pixel = image.matrix[i][j];
            if (pixel > left && pixel < right) {
                image.matrix[i][j] += val;
            }
        }
    }
}

