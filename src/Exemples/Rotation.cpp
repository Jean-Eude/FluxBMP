#include <Bitmap.hpp>

#include <cmath>

int main(int argc, char **argv) {
    Bitmap bmp("lena.bmp");
    Bitmap bmpMAL(bmp.getWidth(), bmp.getHeight(), false);

    double angle = 50.;

    for (int y = 0; y < bmpMAL.getHeight(); y++) {
        for (int x = 0; x < bmpMAL.getWidth(); x++) {
            bmpMAL.set_pixel(0, 0, 0, 1, x, y);
        }
    }

    // Transformation géométrique (rotation)
    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double a = bmp.getPixel(x, y, 'A', true);
            double r = bmp.getPixel(x, y, 'R', true);
            double g = bmp.getPixel(x, y, 'G', true);
            double b = bmp.getPixel(x, y, 'B', true);

            double angleRadians = angle * M_PI / 180.;

            int x_geom = static_cast<int>(cos(angleRadians) * (x - bmp.getWidth() / 2.) + -sin(angleRadians) * (y - bmp.getHeight() / 2.) + bmp.getWidth() / 2.);
            int y_geom = static_cast<int>(sin(angleRadians) * (x - bmp.getWidth() / 2.) + cos(angleRadians) * (y - bmp.getHeight() / 2.) + bmp.getHeight() / 2.);

            if (x_geom >= 0 && x_geom < bmp.getWidth() && y_geom >= 0 && y_geom < bmp.getHeight()) {
                bmpMAL.set_pixel(r, g, b, a, x_geom, y_geom);
            }
        }
    }


    bmpMAL.write("ecriture.bmp");


    return 0;
}