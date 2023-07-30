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

enum Mode {forward, inverse};

typedef std::vector<std::complex<float>> ComplexVec;
typedef std::complex<float> ComplexNum;
constexpr float PI = 3.141592F;

class Image{
    private:
        std::string filename;
        std::vector<std::vector<pixel>> rawimage;
        std::vector<ComplexVec> bwimage;

        void ImageToPPM();

        void PPMToArray(std::string filename);

        ComplexVec ditfft(ComplexVec &slice, Mode mode);

        ComplexVec fft(ComplexVec &slice);

        ComplexVec ifft(ComplexVec &slice);


    public:
        Image();

        void ColorToBW();

        void BWToPPM();

        void fft2();

        void ifft2();
};