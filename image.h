#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <complex>

using namespace std::complex_literals;
struct pixel{
    uint16_t r;
    uint16_t g;
    uint16_t b;
};

typedef std::vector<std::complex<float>> complex_vec;
typedef std::complex<float> complex_num;
constexpr float PI = 3.141592F;

class Image{
    private:
        std::string filename;
        std::vector<std::vector<pixel>> rawimage;
        std::vector<std::vector<complex_num>> bwimage;

        void ImageToPPM();

        void PPMToArray(std::string filename);

        complex_vec ditfft(complex_vec &slice);

    public:
        Image();

        void ColorToBW();

        void fft2();

        void ifft2();

};