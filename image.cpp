#include "image.h"

Image::Image(){
    return;
}

void Image::ImageToPPM(){
    FILE *f;
    f = fopen("write.ppm", "w");

    fprintf(f, "P3\n");
    
    size_t width = rawimage[0].size();
    size_t height = rawimage.size();

    fprintf(f,"%s %s %s" ,std::to_string(width).c_str(), std::to_string(height).c_str(), "\n");
    fprintf(f, "255\n");

    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            fprintf(f, "%d %d %d %s", rawimage[i][j].r, rawimage[i][j].g, rawimage[i][j].b, "  ");
        }
        fprintf(f, "\n");

    }

    printf("%s\n", "Array successfully converted to PPM.");
}

void Image::PPMToArray(std::string filename){
    FILE *f;
    f = fopen(filename.c_str(), "rb");

    char magic_number[4];
    uint64_t width, height, Maxval;

    fscanf(f, "%s %lu %lu %lu", magic_number, &width, &height, &Maxval);
    printf("magic_number = %s, width = %lu, height = %lu, Maxval = %lu\n", magic_number, width, height, Maxval);

    char newline;
    fread(&newline, 1, 1, f);

    rawimage.resize(height, std::vector<pixel>(width));

    int num_bytes = Maxval < 256 ? 1 : 2;
    unsigned char current_pixel[3];

    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            fread(&current_pixel, 3, num_bytes, f);

            uint16_t red = current_pixel[0];
            uint16_t green = current_pixel[1];
            uint16_t blue = current_pixel[2];

            rawimage[i][j] = {red, green, blue};
        }
    }

    printf("%s\n", "PPM file successfully converted to an array.");
}

void Image::ColorToBW(){
    PPMToArray("out.ppm");

    bwimage.resize(rawimage.size(), std::vector<std::complex<float>>(rawimage[0].size()));

    for(size_t i = 0; i < bwimage.size(); i++){
        for(size_t j = 0; j < bwimage[0].size(); j++){
            pixel currpixel = rawimage[i][j];
            bwimage[i][j] = (0.216f * currpixel.r + 0.7152f * currpixel.g + 0.0722f * currpixel.b);
        }
    }

    FILE *f;
    f = fopen("writebw.ppm", "w");

    fprintf(f, "P2\n");
    
    size_t width = bwimage[0].size();
    size_t height = bwimage.size();

    fprintf(f,"%s %s %s" ,std::to_string(width).c_str(), std::to_string(height).c_str(), "\n");
    fprintf(f, "255\n");

    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            fprintf(f, "%d %s", (int) bwimage[i][j].real(), "  ");
        }
        fprintf(f, "\n");
    }

        printf("%s\n", "Image successfully converted to BW.");

}

void Image::fft2(){
    //transform rows first
    for(size_t row = 0; row < bwimage.size(); row++){
        bwimage[row] = ditfft(bwimage[row]);
    }

    //transform columns
    
}

complex_vec Image::ditfft(complex_vec &slice){
    size_t num_elements = slice.size();

    if(num_elements == 1){
        return slice;
    }

    complex_vec evens;
    evens.reserve(num_elements/2);
    complex_vec odds;
    odds.reserve(num_elements/2);


    for(size_t i = 0; i < num_elements; i++){
        if(i % 2 == 0){
            evens.push_back(slice[i]);
        }
        else{
            odds.push_back(slice[i]);
        }
    }

    complex_vec first_half = ditfft(evens);
    complex_vec second_half = ditfft(odds);
    complex_vec result(num_elements, 0);

    for(size_t k = 0; k < num_elements / 2; k++){
        complex_num twiddle_factor = std::exp(complex_num(0,(-2 * PI *k) / num_elements));

        complex_num p = first_half[k];
        complex_num q = second_half[k] * twiddle_factor;

        result[k] = p + q;
        result[k + (num_elements/2)] = p - q;
    }

    return result;
}
