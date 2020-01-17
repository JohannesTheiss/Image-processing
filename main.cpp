#include <iostream>
#include "Image.h"
using namespace std;

int main(void)
{
    // init 
    Image image(256, 256, "noise.txt");

    // print image
    cout<<image;

    image.filter(Gauss5x5, "gauss5x5_out.txt");

    // medianFilter(dimensions, "outputFile.txt");
    image.medianFilter(3, "out3.txt");
    image.medianFilter(5, "out5.txt");

    image.histogram("hist.txt");
    image.histogramEqualization("hq.txt");


    return 0;
}