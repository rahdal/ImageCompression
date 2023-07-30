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

    bwimage.resize(rawimage.size(), ComplexVec(rawimage[0].size()));

    for(size_t i = 0; i < bwimage.size(); i++){
        for(size_t j = 0; j < bwimage[0].size(); j++){
            pixel currpixel = rawimage[i][j];
            bwimage[i][j] = (0.216f * currpixel.r + 0.7152f * currpixel.g + 0.0722f * currpixel.b);
        }
    }

    printf("%s\n", "Image successfully converted to BW.");
}

void Image::BWToPPM(){
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

    printf("%s\n", "BW Image successfully written as PPM.");

}

void Image::fft2(){
    printf("%s\n", "FFT2 Started.");

    //transform rows first
    for(size_t row = 0; row < bwimage.size(); row++){
        bwimage[row] = fft(bwimage[row]);
    }

    //transform columns
    ComplexVec column;
    column.reserve(bwimage[0].size());

    for(size_t col = 0; col < bwimage[0].size(); col++){
        for(size_t row = 0; row < bwimage.size(); row++){
            column.push_back(bwimage[row][col]);
        }
        ComplexVec transformed_column = fft(column);
        for(size_t row = 0; row < bwimage.size(); row++){
            bwimage[row][col] = transformed_column[row];
        }
        column.clear();
    }

    printf("%s\n", "FFT2 Finished.");
}

void Image::ifft2(){
    printf("%s\n", "IFFT2 Started.");

    //transform rows first
    for(size_t row = 0; row < bwimage.size(); row++){
        bwimage[row] = ifft(bwimage[row]);
    }

    //transform columns
    ComplexVec column;
    column.reserve(bwimage[0].size());

    for(size_t col = 0; col < bwimage[0].size(); col++){
        for(size_t row = 0; row < bwimage.size(); row++){
            column.push_back(bwimage[row][col]);
        }
        ComplexVec transformed_column = ifft(column);
        for(size_t row = 0; row < bwimage.size(); row++){
            bwimage[row][col] = transformed_column[row];
        }
        column.clear();
    }

    printf("%s\n", "IFFT2 Finished.");
}


ComplexVec Image::ditfft(ComplexVec &slice, Mode mode){
    size_t num_elements = slice.size();

    if(num_elements == 1){
        return slice;
    }

    ComplexVec evens;
    evens.reserve(num_elements/2);
    ComplexVec odds;
    odds.reserve(num_elements/2);


    for(size_t i = 0; i < num_elements; i++){
        if(i % 2 == 0){
            evens.push_back(slice[i]);
        }
        else{
            odds.push_back(slice[i]);
        }
    }

    ComplexVec first_half = ditfft(evens, mode);
    ComplexVec second_half = ditfft(odds, mode);
    ComplexVec result(num_elements, 0);

    for(size_t k = 0; k < num_elements / 2; k++){
        ComplexNum twiddle_factor;
        if(mode == forward){
            twiddle_factor = std::exp(ComplexNum(0,(-2 * PI * (float) k) / (float) num_elements));
        }
        else{
            twiddle_factor = std::exp(ComplexNum(0,(2 * PI * (float) k) / (float) (num_elements)));
        }
        
        ComplexNum p = first_half[k];
        ComplexNum q = second_half[k] * twiddle_factor;

        result[k] = (p + q);
        result[k + (num_elements/2)] = p - q;
    }

    return result;
}

ComplexVec Image::fft(ComplexVec &slice){
    return ditfft(slice, Mode::forward);
}

ComplexVec Image::ifft(ComplexVec &slice){
    ComplexVec result = ditfft(slice, Mode::inverse);
    size_t num_elements = result.size();

    //Apply scale factor
    for(size_t k = 0; k < num_elements; k++){
        result[k] /= num_elements;
    }

    return result;  
}

