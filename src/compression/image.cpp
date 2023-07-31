#include "compression/image.h"

Image::Image(std::string filename) { PPMToArray(filename); }

void Image::ImageToPPM() {
  FILE *f;
  f = fopen("write.ppm", "w");

  fprintf(f, "P3\n");

  size_t width = rawimage[0][0].size();
  size_t height = rawimage[0].size();

  fprintf(f, "%s %s %s", std::to_string(width).c_str(),
          std::to_string(height).c_str(), "\n");
  fprintf(f, "255\n");

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      int red = (int)rawimage[Color::red][i][j].real();
      int green = (int)rawimage[Color::green][i][j].real();
      int blue = (int)rawimage[Color::blue][i][j].real();

      red = std::max(0, std::min(red, 255));
      green = std::max(0, std::min(green, 255));
      blue = std::max(0, std::min(blue, 255));

      fprintf(f, "%d %d %d %s", red, green, blue, "  ");
    }
    fprintf(f, "\n");
  }

  printf("%s\n", "Array successfully converted to PPM.");
}

void Image::PPMToArray(std::string filename) {
  FILE *f;
  f = fopen(filename.c_str(), "rb");

  char magic_number[4];
  uint64_t width, height, Maxval;

  fscanf(f, "%s %llu %llu %llu", magic_number, &width, &height, &Maxval);
  printf("magic_number = %s, width = %llu, height = %llu, Maxval = %llu\n",
         magic_number, width, height, Maxval);

  char newline;
  fread(&newline, 1, 1, f);

  rawimage.resize(3, std::vector<ComplexVec>(height, ComplexVec(width)));

  size_t num_bytes = Maxval < 256 ? 1 : 2;
  unsigned char current_pixel[3];

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      fread(&current_pixel, 3, num_bytes, f);

      uint16_t red = current_pixel[0];
      uint16_t green = current_pixel[1];
      uint16_t blue = current_pixel[2];

      rawimage[Color::red][i][j] = red;
      rawimage[Color::green][i][j] = green;
      rawimage[Color::blue][i][j] = blue;
    }
  }

  printf("%s\n", "PPM file successfully converted to an array.");
}

void Image::fft2(std::vector<ComplexVec> &channel) {
  printf("%s\n", "FFT2 Started.");

  // transform rows first
  for (size_t row = 0; row < channel.size(); row++) {
    channel[row] = fft(channel[row]);
  }

  // transform columns
  ComplexVec column;
  column.reserve(channel[0].size());

  for (size_t col = 0; col < channel[0].size(); col++) {
    for (size_t row = 0; row < channel.size(); row++) {
      column.push_back(channel[row][col]);
    }
    ComplexVec transformed_column = fft(column);
    for (size_t row = 0; row < channel.size(); row++) {
      channel[row][col] = transformed_column[row];
    }
    column.clear();
  }

  printf("%s\n", "FFT2 Finished.");
}

void Image::ifft2(std::vector<ComplexVec> &channel) {
  printf("%s\n", "IFFT2 Started.");

  // transform rows first
  for (size_t row = 0; row < channel.size(); row++) {
    channel[row] = ifft(channel[row]);
  }

  // transform columns
  ComplexVec column;
  column.reserve(channel[0].size());

  for (size_t col = 0; col < channel[0].size(); col++) {
    for (size_t row = 0; row < channel.size(); row++) {
      column.push_back(channel[row][col]);
    }
    ComplexVec transformed_column = ifft(column);
    for (size_t row = 0; row < channel.size(); row++) {
      channel[row][col] = transformed_column[row];
    }
    column.clear();
  }

  printf("%s\n", "IFFT2 Finished.");
}

ComplexVec Image::ditfft(ComplexVec &slice, Mode mode) {
  size_t num_elements = slice.size();

  if (num_elements == 1) {
    return slice;
  }

  ComplexVec evens;
  evens.reserve(num_elements / 2);
  ComplexVec odds;
  odds.reserve(num_elements / 2);

  for (size_t i = 0; i < num_elements; i++) {
    if (i % 2 == 0) {
      evens.push_back(slice[i]);
    } else {
      odds.push_back(slice[i]);
    }
  }

  ComplexVec first_half = ditfft(evens, mode);
  ComplexVec second_half = ditfft(odds, mode);
  ComplexVec result(num_elements, 0);

  for (size_t k = 0; k < num_elements / 2; k++) {
    ComplexNum twiddle_factor;
    if (mode == forward) {
      twiddle_factor =
          std::exp(ComplexNum(0, (-2 * PI * (float)k) / (float)num_elements));
    } else {
      twiddle_factor =
          std::exp(ComplexNum(0, (2 * PI * (float)k) / (float)(num_elements)));
    }

    ComplexNum p = first_half[k];
    ComplexNum q = second_half[k] * twiddle_factor;

    result[k] = (p + q);
    result[k + (num_elements / 2)] = p - q;
  }

  return result;
}

ComplexVec Image::fft(ComplexVec &slice) {
  return ditfft(slice, Mode::forward);
}

ComplexVec Image::ifft(ComplexVec &slice) {
  ComplexVec result = ditfft(slice, Mode::inverse);
  size_t num_elements = result.size();

  // Apply scale factor
  for (size_t k = 0; k < num_elements; k++) {
    result[k] /= static_cast<float>(num_elements);
  }

  return result;
}

float Image::GetThreshold(Color channel, int ratio) {
  std::priority_queue<float, std::vector<float>, std::greater<float>>
      kth_largest;

  size_t height = rawimage[channel].size();
  size_t width = rawimage[channel][0].size();

  size_t num_elements = height * width;
  size_t cutoff =
      static_cast<size_t>((ratio / 100.0) * static_cast<double>(num_elements));

  for (size_t row = 0; row < height; row++) {
    for (size_t col = 0; col < width; col++) {
      if (kth_largest.size() < cutoff) {
        kth_largest.push(abs(rawimage[channel][row][col]));
      } else {
        if (abs(rawimage[channel][row][col]) > kth_largest.top()) {
          kth_largest.pop();
          kth_largest.push(abs(rawimage[channel][row][col]));
        }
      }
    }
  }

  return kth_largest.top();
}

void Image::ZeroOut(Color channel, float threshold) {
  size_t height = rawimage[channel].size();
  size_t width = rawimage[channel][0].size();

  for (size_t row = 0; row < height; row++) {
    for (size_t col = 0; col < width; col++) {
      if (abs(rawimage[channel][row][col]) < threshold) {
        rawimage[channel][row][col] = 0;
      }
    }
  }
}

void Image::Compress(int ratio) {
  // FFT2 each channel
  std::thread fft_one([this]() { fft2(rawimage[Color::red]); });
  std::thread fft_two([this]() { fft2(rawimage[Color::green]); });
  std::thread fft_three([this]() { fft2(rawimage[Color::blue]); });

  fft_one.join();
  fft_two.join();
  fft_three.join();

  // Get thresholds
  float red_threshold = GetThreshold(Color::red, ratio);
  float green_threshold = GetThreshold(Color::green, ratio);
  float blue_threshold = GetThreshold(Color::blue, ratio);

  // Zero out channels
  ZeroOut(Color::red, red_threshold);
  ZeroOut(Color::green, green_threshold);
  ZeroOut(Color::blue, blue_threshold);

  // IFFT2 each channel
  std::thread ifft_one([this]() { ifft2(rawimage[Color::red]); });
  std::thread ifft_two([this]() { ifft2(rawimage[Color::green]); });
  std::thread ifft_three([this]() { ifft2(rawimage[Color::blue]); });

  ifft_one.join();
  ifft_two.join();
  ifft_three.join();
}