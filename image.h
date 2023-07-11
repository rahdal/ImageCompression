#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>


typedef struct {
    unsigned char r, g, b;
} pixel;


class Image{
    private:
        std::string filename;
        std::vector<std::vector<std::vector<int>>> rawimage;
        std::vector<std::vector<std::vector<int>>> bwimage;

        void ImageToPPM(std::string filename);

        void PPMToArray(std::string filename){
            FILE *f;
            f = fopen(filename.c_str(), "r");

            char magic_number[3];
            int width, height, Maxval;

            fscanf(f, "%s %d %d %d", magic_number, &width, &height, &Maxval);
            printf("magic_n = %s, width = %d, height = %d, max_colour = %d\n", magic_number, width, height, Maxval);

            //int num_bytes = Maxval < 256 ? 1 : 2;
            //pixel currentPix;

            unsigned char clr[7];

            std::cout << clr << '\n';

            int read = fread(&clr, 3, 1, f);

            std::cout << read << '\n';
            
            std::cout << clr;

            // std::cout << pix[0] << '\n';
            // std::cout << pix[1] << '\n';
            // std::cout << pix[2] << '\n';


            for(size_t i = 0; i < height; i++){
                for(size_t j = 0; j < width; j++){
                    break;
                }
            }

        }



    public:
        Image(){
            
        }

        void ColorToBW(){
            PPMToArray("out.ppm");
        }

        void DFT();

        void InverseDFT();











};