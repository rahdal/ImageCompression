#include "image.h"
#include <iostream>

int main(){
    Image test = Image("out.ppm");
    test.Compress(10);
    test.ImageToPPM();

}

