#include <iostream>
#include "Image.h"
using namespace std;

int main(void)
{
    // init 
    Image a(256, 256, "noise.txt");

    a.filter(Gauss3x3, "gauss3x3Out.txt");

    return 0;
}