#include <Bitmap.hpp>

#include <iostream>
#include <cmath>

// https://forums.developer.nvidia.com/t/algorithm-behind-converting-a-diffuse-texture-to-a-normal-texture/203220/2

int main(int argc, char **argv) {
    Bitmap bmp("rock.bmp");
    
    // Transformation en ndg
    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double r = bmp.getPixel(x, y, 'R', false);
            double g = bmp.getPixel(x, y, 'G', false);
            double b = bmp.getPixel(x, y, 'B', false);
            double a = bmp.getPixel(x, y, 'A', true);

            //double Y = std::max(r, std::max(g, b));
            //double Y = 0.299 * r + 0.587 * g + 0.114 * b;
            //double Y = (r + g + b)/ 3;
         
            //double Y = 255 - (255 - r) * (255 - g * (255 * b));
            //Y = std::max(0.0, std::min(255.0, Y));
            //bmp.normalize(&Y);

            //double Y = a;

            bmp.set_pixel(a, a, a, a, x, y);
        }
    }

    bmp.write("ecriture.bmp");

    return 0;
}