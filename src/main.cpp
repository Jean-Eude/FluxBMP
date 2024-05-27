#include <Bitmap.hpp>

#include <cmath>


int main(int argc, char **argv) {
    Bitmap bmp("lena.bmp");
    Bitmap bmpMAL(bmp.getWidth(), bmp.getHeight(), false);

    int centerX = bmp.getWidth() / 2;
    int centerY = bmp.getHeight() / 2;
    double radius = 1.;


    for (int y = 0; y < bmpMAL.getHeight(); y++) {
        for (int x = 0; x < bmpMAL.getWidth(); x++) {
            bmpMAL.set_pixel(0, 0, 0, 1, x, y);
        }
    }

    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double distance = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
            if (distance <= radius) {
                bmpMAL.set_pixel(1, 1, 1, 1, x, y);
            }
        }
    }


    bmpMAL.write("ecriture.bmp");


    return 0;
}