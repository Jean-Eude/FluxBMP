#include <Bitmap.hpp>
#include <cmath>


void createGaussianKernel(std::vector<double> &kernel, int KernelSize, double sigma, bool showKernel) {
    double sum = 0.0;
    double sigma2 = sigma * sigma;
    int halfSize = KernelSize / 2;

    // Obligé de centré pour la symétrie
    for (int y = -halfSize; y <= halfSize; y++) {
        for (int x = -halfSize; x <= halfSize; x++) {
            double value = (1.0 / (2.0 * M_PI * sigma2)) * std::exp(-(x * x + y * y) / (2.0 * sigma2));
            kernel[(y + halfSize) * KernelSize + (x + halfSize)] = value;
            sum += value;
        }
    }

    for (double& value : kernel) {
        value /= sum;
    }

    if(showKernel == true) {
        for (int i = 0; i < KernelSize; i++) {
            for (int j = 0; j < KernelSize; j++) {
                std::cout << kernel[i * KernelSize + j] << "\t";
            }
            std::cout << std::endl;
        }
    }

}


int main(int argc, char **argv) {
    int kernelSize = 11;
    double sigma = 2.5;
    std::vector<double> kernel(kernelSize * kernelSize);

    createGaussianKernel(kernel, kernelSize, sigma, true);


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