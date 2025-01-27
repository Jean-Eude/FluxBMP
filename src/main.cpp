#include <Bitmap.hpp>

int main() {
    Bitmap bmp("test.bmp");


    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double r = bmp.getPixel(x, y, 'R', true);
            double g = bmp.getPixel(x, y, 'G', true);
            double b = bmp.getPixel(x, y, 'B', true);
            double a = bmp.getPixel(x, y, 'A', true);

            bmp.set_pixel(r, g, b, a, x, y);
        }
    }


    bmp.write("ecriture.bmp");


    return 0;
}