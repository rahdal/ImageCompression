#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

struct pixel{
    uint16_t r;
    uint16_t g;
    uint16_t b;
};

class Image{
    private:
        std::string filename;
        std::vector<std::vector<pixel>> rawimage;
        std::vector<std::vector<std::vector<int>>> bwimage;

        void ImageToPPM();

        void PPMToArray(std::string filename);

    public:
        Image();

        void ColorToBW(){
            PPMToArray("out.ppm");
            ImageToPPM();
        }

        void FFT();

        void InverseFFT();

};