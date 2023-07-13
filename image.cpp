#include "image.h"

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
            fprintf(f, "%ld %ld %ld %s", rawimage[i][j].r, rawimage[i][j].g, rawimage[i][j].b, "  ");
        }
        fprintf(f, "\n");

    }
}

void Image::PPMToArray(std::string filename){
    FILE *f;
    f = fopen(filename.c_str(), "rb");

    char magic_number[4];
    int width, height, Maxval;

    fscanf(f, "%s %d %d %d", magic_number, &width, &height, &Maxval);
    printf("magic_number = %s, width = %d, height = %d, Maxval = %d\n", magic_number, width, height, Maxval);

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

    std::cout << rawimage[0][0].r << " " << rawimage[0][0].g << " " << rawimage[0][0].b;
}