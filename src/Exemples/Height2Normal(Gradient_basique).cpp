#include <Bitmap.hpp>

#include <cmath>

int main(int argc, char **argv) {
    Bitmap bmp("test.bmp");

    // Transformation en ndg
    for (int y = 0; y < bmp.getHeight(); y++) {
        for (int x = 0; x < bmp.getWidth(); x++) {
            double r = bmp.getPixel(x, y, 'R', false);
            double g = bmp.getPixel(x, y, 'G', false);
            double b = bmp.getPixel(x, y, 'B', false);
            double a = bmp.getPixel(x, y, 'A', true);

            double Y = 0.299 * r + 0.587 * g + 0.114 * b;

            // Pour (i+1), etc.., appliqué sur RGB (imaeg couleur)
            double YApresHori = Y;
            double YApresVerti = Y;

            if (x + 1 < bmp.getWidth()) {
                double rApresHori = bmp.getPixel(x + 1, y, 'R', false);
                double gApresHori = bmp.getPixel(x + 1, y, 'G', false);
                double bApresHori = bmp.getPixel(x + 1, y, 'B', false);
                YApresHori = 0.299 * rApresHori + 0.587 * gApresHori + 0.114 * bApresHori;
            }

            if (y + 1 < bmp.getHeight()) {
                double rApresVerti = bmp.getPixel(x, y + 1, 'R', false);
                double gApresVerti = bmp.getPixel(x, y + 1, 'G', false);
                double bApresVerti = bmp.getPixel(x, y + 1, 'B', false);
                YApresVerti = 0.299 * rApresVerti + 0.587 * gApresVerti + 0.114 * bApresVerti;
            }

            double GradientX = (YApresHori - Y) + 128;
            double GradientY = (YApresVerti - Y) + 128;
            double GradientZ = sqrt(GradientX * GradientX + GradientY * GradientY);

            //std::cout << GradientX << std::endl;

            // Normalisation [0. - 1.]
            bmp.normalize(&GradientX);
            bmp.normalize(&GradientY);
            bmp.normalize(&GradientZ);

            // Norme OpenGL :   1. - y
            // Norme DirectX :  y

            // 3-channel tangent space normal map
            bmp.set_pixel(GradientX, 1. - GradientY, GradientZ, a, x, y);

            // 2-channel tangent space normal map
            //bmp.set_pixel(GradientX, 1. - GradientY, 0., a, x, y);
        }
    }


    bmp.write("ecriture.bmp");


    return 0;
}