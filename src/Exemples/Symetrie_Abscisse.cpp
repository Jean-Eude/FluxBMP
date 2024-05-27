#include <Bitmap.hpp>

#include <cmath>


int main(int argc, char **argv) {
    Bitmap bmp("lena.bmp");

    // Transformation géométrique (symétrie)
    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double a = bmp.getPixel(x, y, 'A', true);
            double r = bmp.getPixel(x, y, 'R', true);
            double g = bmp.getPixel(x, y, 'G', true);
            double b = bmp.getPixel(x, y, 'B', true);

            int x_geom = static_cast<int>(1. * (x - bmp.getWidth() / 2.) + 0. * (y - bmp.getHeight() / 2.) + bmp.getWidth() / 2.);
            int y_geom = static_cast<int>(0. * (x - bmp.getWidth() / 2.) - 1. * (y - bmp.getHeight() / 2.) + bmp.getHeight() / 2.);

            if (x_geom >= 0 && x_geom < bmp.getWidth() && y_geom >= 0 && y_geom < bmp.getHeight()) {
                bmp.set_pixel(r, g, b, a, x_geom, y_geom);
            }
        }
    }


    bmp.write("ecriture.bmp");


    return 0;
}