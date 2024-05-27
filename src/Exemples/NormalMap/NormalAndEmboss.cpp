#include <Bitmap.hpp>

#include <cmath>


const int kernelSize = 5;
//std::vector<double> kernel = { 0, 1, 0, 0, 0, 0, 0, -1, 0 };
//std::vector<double> kernel = { 1, 0, 0, 0, 0, 0, 0, 0, -1 };
//std::vector<double> kernel = { 0, 0, 0, 1, 0, -1, 0, 0, 0 };
//std::vector<double> kernel = { 0, 0, 1, 0, 0, 0, -1, 0, 0 };
//std::vector<double> kernel = { -1, 0, 0, 0, 0, 0, 0, 0, 1 };
//std::vector<double> kernel = { 0, 0, -1, 0, 0, 0, 1, 0, 0 };
//std::vector<double> kernel = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1 };


int main(int argc, char **argv) {
    Bitmap bmp("circle.bmp");
    Bitmap bmpSave(bmp.getWidth(), bmp.getHeight(), false);

    double EmbossIntensity = 4.5;
    std::vector<double> kernel = { EmbossIntensity, 0, 0, 0, 0, 0, EmbossIntensity, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -EmbossIntensity, 0, 0, 0, 0, 0, -EmbossIntensity };

    
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
            yAcc = std::max(0.0, std::min(255.0, yAcc));

            bmpSave.normalize(&yAcc);
            bmpSave.set_pixel(yAcc, yAcc, yAcc, 1, x, y);
        }
    }

    
    // Transformation en ndg
    for (int y = 0; y < bmpSave.getHeight(); y++) {
        for (int x = 0; x < bmpSave.getWidth(); x++) {
            double r = bmpSave.getPixel(x, y, 'R', false);
            double g = bmpSave.getPixel(x, y, 'G', false);
            double b = bmpSave.getPixel(x, y, 'B', false);
            double a = bmpSave.getPixel(x, y, 'A', true);

            double Y = bmpSave.getPixel(x, y, 'R', false);

            // Pour (i+1), etc.., appliqué sur RGB (imaeg couleur)
            double YApresHori = Y;
            double YApresVerti = Y;

            if (x + 1 < bmpSave.getWidth()) {
                double rApresHori = bmpSave.getPixel(x + 1, y, 'R', false);
                double gApresHori = bmpSave.getPixel(x + 1, y, 'G', false);
                double bApresHori = bmpSave.getPixel(x + 1, y, 'B', false);
                YApresHori = 0.299 * rApresHori + 0.587 * gApresHori + 0.114 * bApresHori;
            }

            if (y + 1 < bmpSave.getHeight()) {
                double rApresVerti = bmpSave.getPixel(x, y + 1, 'R', false);
                double gApresVerti = bmpSave.getPixel(x, y + 1, 'G', false);
                double bApresVerti = bmpSave.getPixel(x, y + 1, 'B', false);
                YApresVerti = 0.299 * rApresVerti + 0.587 * gApresVerti + 0.114 * bApresVerti;
            }

            double GradientX = (YApresHori - Y) + 128;
            double GradientY = (YApresVerti - Y) + 128;
            double GradientZ = sqrt(GradientX * GradientX + GradientY * GradientY);

            GradientX = std::max(0.0, std::min(255.0, GradientX));
            GradientY = std::max(0.0, std::min(255.0, GradientY));
            GradientZ = std::max(0.0, std::min(255.0, GradientZ));

            // Normalisation [0. - 1.]
            bmpSave.normalize(&GradientX);
            bmpSave.normalize(&GradientY);
            bmpSave.normalize(&GradientZ);

            // Norme OpenGL :   1. - y
            // Norme DirectX :  y


            // 3-channel tangent space normal map
            bmpSave.set_pixel(GradientX, 1. - GradientY, GradientZ, a, x, y);

            // 2-channel tangent space normal map
            //bmpSave.set_pixel(GradientX, 1. - GradientY, 0., a, x, y);
        }
    }

    bmpSave.write("ecriture.bmp");


    return 0;
}