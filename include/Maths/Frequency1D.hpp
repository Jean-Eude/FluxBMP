#pragma once

#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include <array>
#include <bitset>  

/* ----------------------------------------------------------- //
Description :

. DFT (normalisée)

// ----------------------------------------------------------- */

void DFT(std::vector<double> &x, std::vector<std::complex<double>> &y) {
    int N = x.size();
    y.resize(N);

    std::complex<double> acc = 0;
    double angleA = -2.0 * M_PI;
    double angleAK = 0;

    for (int k = 0; k < N; k++) {
        acc = 0;
        angleAK = angleA * k;
        for (int n = 0; n < N; n++) {
            double angle = (angleAK * n) / N;
            acc += x[n] * std::polar(1.0, angle);
        }

        y[k] = acc * (1.0 / N);
    }
}

/* ----------------------------------------------------------- //
Description :

. IDFT (normalisée)

// ----------------------------------------------------------- */

void IDFT(std::vector<std::complex<double>> &y, std::vector<double> &x) {
    x.clear();
    int N = y.size();
    x.resize(N);
    std::complex<double> acc = 0;
    double angleA = 2.0 * M_PI;
    double angleAK = 0;

    for (int n = 0; n < N; n++) {
        acc = 0;

        for (int k = 0; k < N; k++) {
            angleAK = angleA * k;
            double angle = (angleAK * n) / N;
            acc += y[k] * std::polar(1.0, angle);
        }

        x[n] = acc.real();
    }
}

/* ----------------------------------------------------------- //
Description :

. DCT-II (normalisée)
. La partie imaginaire n'est pas prise en compte

// ----------------------------------------------------------- */

void DCT(std::vector<double> &x, std::vector<std::complex<double>> &y) {
    int N = x.size();
    y.resize(N);

    double one_square = sqrt(1.0 / N);
    double two_square = sqrt(2.0 / N);

    for (int k = 0; k < N; k++) {
        double acc = 0;
        
        for (int n = 0; n < N; n++) {
            acc += x[n] * cos((M_PI * (n + 0.5) * k) / N);
        }

        if (k == 0) {
            y[k] = acc * one_square;
        } else {
            y[k] = acc * two_square;
        }    
    }
}

/* ----------------------------------------------------------- //
Description :

. DCT-III (inverse de DCT-II)
. La partie imaginaire n'est pas prise en compte
. Normalisée

// ----------------------------------------------------------- */

void IDCT(const std::vector<std::complex<double>>& y, std::vector<double>& x) {
    int N = y.size();
    x.resize(N);

    double p = sqrt(2.0 / N);

    for (int n = 0; n < N; n++) {
        double acc = 0;

        for (int k = 0; k < N; k++) {
            double qn = (k == 0) ? sqrt(1.0 / 2.0) : 1.0;
            acc += qn * y[k].real() * cos(M_PI * k * (n + 0.5) / N);
        }

        x[n] = p * acc;
    }
}

/* ----------------------------------------------------------- //
Description :

. Naive Bit-Reversal Algorithm  

// ----------------------------------------------------------- */

template <unsigned T>
void NaiveBitReversalPermutation(std::array<unsigned, 1 << T> &bits) {
    unsigned int N = bits.size(); 
	for (unsigned i = 0; i < N; ++i) {
		unsigned a = i, b = 0, j = 0;
		while (j++ < T) {
			b = (b << 1) | (a & 1);
			a >>= 1;
		}
		bits[i] = b;
	}
}

/* ----------------------------------------------------------- //
Description :

. Fast Bit-Reversal Algorithm

// ----------------------------------------------------------- */

template <unsigned T> 
void FastBitReversalPermutation(std::array<unsigned, 1 << T> &bits) {
	const unsigned half_n = 1 << (T-1);
	bits = {0, half_n};
	for (unsigned n = 1; n < half_n; ++n) {
		const unsigned index = n << 1;
		bits[index] = bits[n] >> 1;
		bits[index+1] = bits[index] + half_n;
	}
}

/* ----------------------------------------------------------- //
Description :

. FFT Radix 2 DIT

// ----------------------------------------------------------- */

void TwiddleFactors(std::vector<std::complex<double>> &Wn, int N) {
    Wn.resize(N);
    for(size_t k = 0; k < N; k++) {
        Wn[k] = std::polar(1.0, -2.0 * M_PI * k / N);
        // Identité d'Euler
        //Wn[k] = std::complex<double>(cos(-2.0 * M_PI * k / N), sin(-2.0 * M_PI * k / N));
        
        // Approximation pour 0
        if (std::abs(Wn[k].real()) < 1e-15) {
            Wn[k].real(0.0);
        }
        if (std::abs(Wn[k].imag()) < 1e-15) {
            Wn[k].imag(0.0);
        }        
    }
}

template <unsigned T>
void NaiveFFT(std::vector<std::complex<double>>& data, std::array<unsigned, 1 << T>& bitsReversed, bool FFTDir) {
    const unsigned N = data.size();
    double sign = FFTDir ? -1.0 : 1.0; //
    
    for (unsigned s = 1; s <= T; ++s) {
        unsigned m = 1 << s;
        std::complex<double> wm = std::polar(1.0, sign * 2.0 * M_PI / m);
        for (unsigned k = 0; k < N; k += m) {
            std::complex<double> w = 1;
            for (unsigned j = 0; j < m / 2; ++j) {
                std::complex<double> t = w * data[k + j + m / 2];
                std::complex<double> u = data[k + j];
                data[k + j] = u + t;
                data[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
}
