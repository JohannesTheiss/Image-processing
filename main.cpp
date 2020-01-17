#include <iostream>
#include "Image.h"
using namespace std;

int main(void)
{
    // init 
   // Image image(256, 256, "noise.txt");
    Image image(3, 3, "test.txt");

    //image.filter(Gauss5x5, "newSx.txt");

    // medianFilter(dimensions, "outputFile.txt");
    // image.medianFilter(3, "out3.txt");
    // image.medianFilter(5, "out5.txt");

    image.histo("hist.txt");


    return 0;
}