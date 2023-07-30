#include "image.h"
#include <iostream>

int main(){
    Image test = Image();
    test.ColorToBW();
    test.fft2();
    test.ifft2();
    test.BWToPPM();
}

