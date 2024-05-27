#include <Bitmap.hpp>

#include <cmath>

const int kernelSize = 3;
std::vector<double> kernel = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
//std::vector<double> kernel = { -1, -1, -1, -1, 8, -1, -1, -1, -1};
//std::vector<double> kernel = { 0, 0, -1, 0, 0, 0, -1, -2, -1, 0, -1, -2, 16, -2, -1, 0, -1, -2, -1, 0, 0, 0, -1, 0, 0 };


int main(int argc, char **argv) {
    Bitmap bmp("lena.bmp");
    Bitmap bmpSave(bmp.getWidth(), bmp.getHeight(), false);

    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double r = bmp.getPixel(x, y, 'R', false);
            double g = bmp.getPixel(x, y, 'G', false);
            double b = bmp.getPixel(x, y, 'B', false);
            double a = bmp.getPixel(x, y, 'A', true);

            double Y = 0.299 * r + 0.587 * g + 0.114 * b;

            bmp.normalize(&Y);
            bmp.set_pixel(Y, Y, Y, a, x, y);
        }
    }


    // Application d'une convolution
    for (int y = 0; y < bmpSave.getHeight(); y++) {
        for (int x = 0; x < bmpSave.getWidth(); x++) {
            double yAcc = 0.;

            for (int ky = 0; ky < kernelSize; ky++) {
                for (int kx = 0; kx < kernelSize; kx++) {
                    int ni = x + kx - kernelSize / 2;
                    int nj = y + ky - kernelSize / 2;

                    ni = std::max(0, std::min(int(bmp.getWidth() - 1), ni));
                    nj = std::max(0, std::min(int(bmp.getHeight() - 1), nj));
                    
                    double YNeighbor = bmp.getPixel(ni, nj, 'R', false);
                    int kernelIndex = ky * kernelSize + kx;
                    yAcc += kernel[kernelIndex] * YNeighbor;
                    
                }
            }
            yAcc += 128;
            bmpSave.normalize(&yAcc);
            bmpSave.set_pixel(yAcc, yAcc, yAcc, 1, x, y);
        }
    }

    bmpSave.write("ecriture.bmp");


    return 0;
}