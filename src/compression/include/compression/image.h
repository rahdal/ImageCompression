#include <stdio.h>

#include <complex>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>

using namespace std::complex_literals;

enum Mode { forward, inverse };
enum Color { red, green, blue };

typedef std::vector<std::complex<float>> ComplexVec;
typedef std::complex<float> ComplexNum;
constexpr float PI = 3.141592F;

class Image {
 private:
  std::string filename;
  std::vector<std::vector<std::vector<ComplexNum>>> rawimage;
  std::vector<ComplexVec> bwimage;

  void PPMToArray(std::string filename);

  ComplexVec ditfft(ComplexVec &slice, Mode mode);

  ComplexVec fft(ComplexVec &slice);

  ComplexVec ifft(ComplexVec &slice);

  float GetThreshold(Color channel, int ratio);

  void ZeroOut(Color channel, float threshold);

  void fft2(std::vector<ComplexVec> &channel);

  void ifft2(std::vector<ComplexVec> &channel);

 public:
  Image(std::string filename);

  void Compress(int ratio);

  void ImageToPPM();
};