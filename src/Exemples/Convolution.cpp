#include <Bitmap.hpp>

#include <cmath>

const int kernelSize = 3;
std::vector<double> kernel(kernelSize * kernelSize,  1.0 / (kernelSize * kernelSize));

int main(int argc, char **argv) {
    Bitmap bmp("lena.bmp");

    // Application d'une convolution
    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double rAcc = 0.0, gAcc = 0.0, bAcc = 0.0;
            double a = bmp.getPixel(x, y, 'A', false);

            for (int ky = 0; ky < kernelSize; ky++) {
                for (int kx = 0; kx < kernelSize; kx++) {
                    int ni = x + kx - kernelSize / 2;
                    int nj = y + ky - kernelSize / 2;

                    if (ni >= 0 && ni < bmp.getWidth() && nj >= 0 && nj < bmp.getHeight()) {
                        double r = bmp.getPixel(ni, nj, 'R', false);
                        double g = bmp.getPixel(ni, nj, 'G', false);
                        double b = bmp.getPixel(ni, nj, 'B', false);
                        int kernelIndex = ky * kernelSize + kx;

                        rAcc += kernel[kernelIndex] * r;
                        gAcc += kernel[kernelIndex] * g;
                        bAcc += kernel[kernelIndex] * b;
                    }
                }
            }

            bmp.normalize(&rAcc);
            bmp.normalize(&gAcc);
            bmp.normalize(&bAcc);

            bmp.set_pixel(rAcc, gAcc, bAcc, a, x, y);
        }
    }

    bmp.write("ecriture.bmp");


    return 0;
}