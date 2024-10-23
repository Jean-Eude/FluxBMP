#pragma once

#include <Bitmap.hpp>

/* ----------------------------------------------------------- //
Description :

. Sous divisions de blocs pour DCT ou autre
. Contigu en mémoire

// ----------------------------------------------------------- */

void SubBlocks_Div(int x, int y, unsigned int block, std::vector<double> &Blocks, Bitmap &bmp) {
    for (int i = 0; i < block; i++) {
        for (int j = 0; j < block; j++) {
            int newX = x + i;
            int newY = y + j;

            Blocks[newX * bmp.getWidth() + newY] = bmp.getPixel(newX, newY, 'R', false);
            //std::cout << Blocks[newX * bmp.getWidth() + newY] << std::endl;
        }
    }
}

/* ----------------------------------------------------------- //
Description :

. DFT 2D (normalisée)

// ----------------------------------------------------------- */


void DFT_2D(std::vector<std::vector<std::complex<double>>>& output, std::vector<std::vector<std::complex<double>>>& input, int N, int M) {
    std::complex<double> sum;

    for (int u = 0; u < N; ++u) {
        for (int v = 0; v < M; ++v) {
            sum.real(0.);
            sum.imag(0.);

            for (int x = 0; x < N; ++x) {
                for (int y = 0; y < M; ++y) {
                    double angle = 2 * M_PI * ((u * x / static_cast<double>(N)) + (v * y / static_cast<double>(M)));
                    sum += input[x][y] * std::exp(std::complex<double>(0, -angle));
                }
            }

            output[u][v] = sum / sqrt(N*M);
        }
    }
}