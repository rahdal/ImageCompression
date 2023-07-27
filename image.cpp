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

    bwimage.resize(rawimage.size(), std::vector<uint16_t>(rawimage[0].size()));

    for(size_t i = 0; i < bwimage.size(); i++){
        for(size_t j = 0; j < bwimage[0].size(); j++){
            pixel currpixel = rawimage[i][j];
            bwimage[i][j] = (uint16_t) (0.216 * currpixel.r + 0.7152 * currpixel.g + 0.0722 * currpixel.b);
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
            fprintf(f, "%d %s", bwimage[i][j], "  ");
        }
        fprintf(f, "\n");
    }

        printf("%s\n", "Image successfully converted to BW.");

}

void Image::FFT(){
    
}