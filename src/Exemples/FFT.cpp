#include <Maths.hpp>
#include <cmath>

int main(int argc, char **argv) {
    std::vector<double> x;

    x.push_back(1.);
    x.push_back(2.);
    x.push_back(3.);

    std::vector<std::complex<double>> y;
    y.push_back(1.);
    y.push_back(2.);
    y.push_back(3.);

    DFT(x, y);
    for (size_t k = 0; k < y.size(); ++k) {
        std::cout << "dft[" << k << "] = " << y[k] << std::endl;
    }

    IDFT(y, x);
    for (size_t k = 0; k < x.size(); ++k) {
        std::cout << "x[" << k << "] = " << x[k] << std::endl;
    }

    /*
    const unsigned t = 3;
    std::array<unsigned, 1 << t> permutation;
    
    FastBitReversalPermutation<t>(permutation);
*/


    int n1 = 1 << 3;
    int n2 = log2(n1);

    std::cout << n1 << std::endl;
    std::cout << n2 << std::endl;


    std::vector<std::complex<double>> result;
    TwiddleFactors(result, 8);
    for(size_t i= 0; i < 8; i++) {
        std::cout << result[i] << std::endl;
    }

    std::vector<std::complex<double>> z;
    z.push_back(1.);
    z.push_back(2.);
    z.push_back(3.);
    z.push_back(4.);

    const unsigned Nlog2 = 2;
    std::array<unsigned, 1 << Nlog2> permutation;
    FastBitReversalPermutation<Nlog2>(permutation);
    NaiveFFT<Nlog2>(z, permutation, false);

    for(size_t i= 0; i < z.size(); i++) {
        std::cout << z[i].imag() << std::endl;
    }

    NaiveFFT<Nlog2>(z, permutation, true);
    for(size_t i= 0; i < z.size(); i++) {
        std::cout << z[i].real() << std::endl;
    }
    return 0;
}