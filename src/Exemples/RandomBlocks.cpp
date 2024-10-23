#include <Bitmap.hpp>
#include <Maths.hpp>

int main(int argc, char **argv) {
    Bitmap bmpMAL(512, 512, true);

    srand(static_cast<unsigned>(time(NULL)));
    unsigned int block = 32;

    for (int x = 0; x < bmpMAL.getWidth(); x += block) {
        for (int y = 0; y < bmpMAL.getHeight(); y += block) {
            double rr = RandomFloat(0., 1.);
            double rg = RandomFloat(0., 1.);
            double rb = RandomFloat(0., 1.);

            for (int i = 0; i < block; i++) {
                for (int j = 0; j < block; j++) {
                    int newX = x + i;
                    int newY = y + j;

                    if (newX < bmpMAL.getHeight() && newY < bmpMAL.getWidth()) {
                        bmpMAL.set_pixel(rr, rg, rb, 0., newX, newY);
                    }
                }
            }
        }
    }

    bmpMAL.write("ecriture.bmp");

    return 0;
}