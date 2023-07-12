#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>


struct pixel{
    uint64_t r;
    uint64_t g;
    uint64_t b;
};


class Image{
    public:
        std::string filename;
        std::vector<std::vector<pixel>> rawimage;
        std::vector<std::vector<std::vector<int>>> bwimage;

        void ImageToPPM(std::string filename){
            FILE *f;
            f = fopen("write.ppm", "w");

            fprintf(f, "P3\n");
            
            size_t width = rawimage[0].size();
            size_t height = rawimage.size();

            fprintf(f,"%s %s %s" ,std::to_string(width).c_str(), std::to_string(height).c_str(), "\n");

            for(size_t i = 0; i < height; i++){
                for(size_t j = 0; j < width; j++){
                    fprintf(f, "%ld %ld %ld %s", rawimage[i][j].r, rawimage[i][j].g, rawimage[i][j].b, " ");
                }
                fprintf(f, "\n");

            }
        }

        void PPMToArray(std::string filename){
            
            FILE *f;
            f = fopen(filename.c_str(), "r");

            char magic_number[3];
            int width, height, Maxval;

            fscanf(f, "%s %d %d %d", magic_number, &width, &height, &Maxval);
            printf("magic_number = %s, width = %d, height = %d, Maxval = %d\n", magic_number, width, height, Maxval);

            rawimage.resize(height, std::vector<pixel>(width));

            int num_bytes = Maxval < 256 ? 1 : 2;
            unsigned char current_pixel[3];

            for(size_t i = 0; i < height; i++){
                for(size_t j = 0; j < width; j++){
                    fread(&current_pixel, num_bytes, 3 , f);

                    uint64_t red = current_pixel[0];
                    red *= 2;
                    uint64_t blue = current_pixel[1];
                    blue *= 2;
                    uint64_t green = current_pixel[2];
                    green *= 2;

                    rawimage[i][j] = {red, green, blue};
                }
            }
        }



    public:
        Image(){
            
        }

        void ColorToBW(){
            PPMToArray("out.ppm");
        }

        void FFT();

        void InverseFFT();











};