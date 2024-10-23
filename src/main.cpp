#include <Bitmap.hpp>
#include <Maths.hpp>


int main(int argc, char **argv) {
    Bitmap bmpMAL(512, 512, true);
    Bitmap bmpAppl(bmpMAL.getWidth(), bmpMAL.getHeight(), true);

    int N = bmpMAL.getWidth();
    int M = bmpMAL.getHeight();

    // Initialiser les vecteurs pour l'entrée et la sortie
    std::vector<std::vector<std::complex<double>>> input(N, std::vector<std::complex<double>>(M));
    std::vector<std::vector<std::complex<double>>> output(N, std::vector<std::complex<double>>(M));

    // Remplir le vecteur d'entrée avec les valeurs de l'image
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < M; ++y) {
            double pixelValue = static_cast<double>(bmpMAL.getPixel(x, y, 'R', true));
            input[x][y] = std::complex<double>(pixelValue, 0);
        }
    }

    // Appeler la fonction DFT_2D
    DFT_2D(output, input, N, M);

    // Vous pouvez maintenant utiliser les données dans 'output'
    // Par exemple, écrire les valeurs réelles dans une nouvelle image
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < M; ++y) {
            double magnitude = std::abs(output[x][y]);
            bmpAppl.set_pixel(magnitude, magnitude, magnitude, 1., x, y);
        }
    }

    bmpAppl.write("ecriture1.bmp");

    return 0;
}
