#include <Bitmap.hpp>
#include <Maths.hpp>




float Qstep(int QP) {
    if (QP == 0) {
        return 0.625f;
    }
    float result = 0.625f;
    for (int i = 1; i <= QP; ++i) {
        result *= pow(2.0f, 1.0f / 6.0f);
    }
    return result;
}

int Q = 1;


/*
void intDCT(ImageBase &imIn, int width, int height, std::vector<std::vector<float>> &MatRefNoTranspose, std::vector<std::vector<float>> &MatRefTranspose, std::vector<std::vector<float>> &MatScaling, std::vector<float> &MatDCTFinal) {
    for (int y = 0; y < height; y += 4) {
        for (int x = 0; x < width; x += 4) {
            std::vector<std::vector<float>> subBlock;
            FourXFour(imIn, width, height, x, y, subBlock);

            std::vector<std::vector<float>> MatDCT, MatDCT2, MatDCTResult;
            Mul2Mat(MatRefNoTranspose, subBlock, MatDCT);
            Mul2Mat(MatDCT, MatRefTranspose, MatDCT2);
            Mul2Mat_Hadamard(MatDCT2, MatScaling, MatDCTResult);

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (y + i < height && x + j < width) {
                        MatDCTFinal.push_back(round(MatDCTResult[i][j]));
                    }
                }
            }
        }
    }
}*/





int main(int argc, char **argv) {
    Bitmap bmpMAL(512, 512, true);
    Bitmap bmpAppl(bmpMAL.getWidth(), bmpMAL.getHeight(), true);

    //srand(static_cast<unsigned>(time(NULL)));
    unsigned int block = 32;

    int N = bmpMAL.getWidth() * bmpMAL.getHeight();
    std::vector<double> Blocks(N);



    // Données
    std::vector<std::vector<float>> MatRefDCTNoTranspose = {
        {1.f, 1.f, 1.f, 1.f},
        {2.f, 1.f, -1.f, -2.f},
        {1.f, -1.f, -1.f, 1.f},
        {1.f, -2.f, 2.f, -1.f}
    };

    std::vector<std::vector<float>> MatRefDCTTranspose = {
        {1.f, 2.f, 1.f, 1.f},
        {1.f, 1.f, -1.f, -2.f},
        {1.f, -1.f, -1.f, 2.f},
        {1.f, -2.f, 1.f, -1.f}
    };

    std::vector<std::vector<float>> MatRefIDCTTranspose = {
        {1.f, 1.f, 1.f, 0.5f},
        {1.f, 0.5f, -1.f, -1.f},
        {1.f, -0.5f, -1.f, 1.f},
        {1.f, -1.f, 1.f, -0.5f}
    };

    std::vector<std::vector<float>> MatRefIDCTNoTranspose = {
        {1.f, 1.f, 1.f, 1.f},
        {1.f, 0.5f, -0.5f, -1.f},
        {1.f, -1.f, -1.f, 1.f},
        {0.5f, -1.f, 1.f, -0.5f}
    };


    float a = 0.5f;
    float b = sqrt(2.f / 5.f);
    float d = a;

    std::vector<std::vector<float>> MatX;
    std::vector<std::vector<float>> MatScaling = {
        {a*a, (a*b)/2.f, a*a, (a*b)/2.0f},
        {(a*b)/2.f, (b*b)/4.f, (a*b)/2.f, (b*b)/4.f},
        {a*a, (a*b)/2.f, a*a, (a*b)/2.0f},
        {(a*b)/2.f, (b*b)/4.f, (a*b)/2.f, (b*b)/4.f}
    };

    float CoeffQuantif = Qstep(Q);
    std::cout << CoeffQuantif << std::endl;

    std::vector<std::vector<float>> MatScalingQuantifDCT = {
        {(a*a) / CoeffQuantif, ((a*b)/2.f) / CoeffQuantif, (a*a) / CoeffQuantif, ((a*b)/2.f) / CoeffQuantif},
        {((a*b)/2.f) / CoeffQuantif, ((b*b)/4.f) / CoeffQuantif, ((a*b)/2.f) / CoeffQuantif, ((b*b)/4.f) / CoeffQuantif},
        {(a*a) / CoeffQuantif, ((a*b)/2.f) / CoeffQuantif, (a*a) / CoeffQuantif, ((a*b)/2.f) / CoeffQuantif},
        {((a*b)/2.f) / CoeffQuantif, ((b*b)/4.f) / CoeffQuantif, ((a*b)/2.f) / CoeffQuantif, ((b*b)/4.f) / CoeffQuantif}
    };

    std::vector<std::vector<float>> MatScalingQuantifIDCT = {
        {(a*a) * CoeffQuantif, ((a*b)/2.f) * CoeffQuantif, (a*a) * CoeffQuantif, ((a*b)/2.f) * CoeffQuantif},
        {((a*b)/2.f) * CoeffQuantif, ((b*b)/4.f) * CoeffQuantif, ((a*b)/2.f) * CoeffQuantif, ((b*b)/4.f) * CoeffQuantif},
        {(a*a) * CoeffQuantif, ((a*b)/2.f) * CoeffQuantif, (a*a) * CoeffQuantif, ((a*b)/2.f) * CoeffQuantif},
        {((a*b)/2.f) * CoeffQuantif, ((b*b)/4.f) * CoeffQuantif, ((a*b)/2.f) * CoeffQuantif, ((b*b)/4.f) * CoeffQuantif}
    };





    for (int x = 0; x < bmpMAL.getWidth(); x += block) {
        for (int y = 0; y < bmpMAL.getHeight(); y += block) {
            double rr = RandomFloat(0., 1.);

            for (int i = 0; i < block; i++) {
                for (int j = 0; j < block; j++) {
                    int newX = x + i;
                    int newY = y + j;

                    bmpMAL.set_pixel(rr, rr, rr, 1., newX, newY);
                    //Blocks[newX * bmpMAL.getWidth() + newY] = rr;
                    //std::cout << Blocks[newX * bmpMAL.getWidth() + newY] << std::endl;
                }
            }

            SubBlocks_Div(x, y, block, Blocks, bmpMAL);

            /*
            for (int i = 0; i < block; i++) {
                for (int j = 0; j < block; j++) {
                    int newX = x + i;
                    int newY = y + j;

                    double value = Blocks[newX * bmpMAL.getWidth() + newY];
                    bmpAppl.set_pixel(value, value, value, 1., newX, newY);
                }
            }*/
        }
    }

    for (int x = 0; x < bmpMAL.getWidth(); x += block) {
        for (int y = 0; y < bmpMAL.getHeight(); y += block) { 
            for (int i = 0; i < block; i++) {
                for (int j = 0; j < block; j++) {
                    int newX = x + i;
                    int newY = y + j;

                    double value = Blocks[newX * bmpMAL.getWidth() + newY];
                    bmpAppl.normalize(&value);
                    bmpAppl.set_pixel(value, value, value, 1., newX, newY);
                }
            }
        }
    }


    bmpAppl.write("ecriture1.bmp");


    std::cout << Blocks.size() << std::endl;
    

    return 0;
}