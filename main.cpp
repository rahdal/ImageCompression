#include "image.h"
#include <iostream>

int main(){
    Image test = Image("out.ppm");
    test.Compress();
    test.ImageToPPM();

}

