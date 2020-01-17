#include <iostream>
#include "Image.h"
using namespace std;

int main(void)
{
    // init 
    Image image(256, 256, "problemml.txt");

    // image.filter(Gauss3x3, "newSx.txt");

    // medianFilter(dimensions, "outputFile.txt");
    image.medianFilter(3, "out3.txt");
    image.medianFilter(5, "out5.txt");

    return 0;
}