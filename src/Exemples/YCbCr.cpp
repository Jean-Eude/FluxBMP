#include <Bitmap.hpp>

int main() {
    Bitmap bmp("lena.bmp");


    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double r = bmp.getPixel(x, y, 'R', false);
            double g = bmp.getPixel(x, y, 'G', false);
            double b = bmp.getPixel(x, y, 'B', false);
            double a = bmp.getPixel(x, y, 'A', true);

            double Y = 0.299 * r + 0.587 * g + 0.114 * b;
            double Cb = -0.1687 * r - 0.3313 * g + 0.5 * b + 128.;
            double Cr = 0.5 * r - 0.4187 * g - 0.0813 * b + 128.;

            bmp.normalize(&Y);
            bmp.normalize(&Cb);
            bmp.normalize(&Cr);

            bmp.set_pixel(Cb, Cb, Cb, a, x, y);
        }
    }



    bmp.write("ecriture.bmp");


    return 0;
}
