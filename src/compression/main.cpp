#include "compression/image.h"

int main() {
  Image test = Image("out.ppm");
  test.Compress(50);
  test.ImageToPPM();
}
