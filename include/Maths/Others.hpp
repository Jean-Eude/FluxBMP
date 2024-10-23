#pragma once 

#include <iostream>

/* ----------------------------------------------------------- //
Description :

. Nombre flottant aléatoire entre MIN et MAX

// ----------------------------------------------------------- */

float RandomFloat(float min, float max)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float range = max - min;  
    return (random*range) + min;
}

/* ----------------------------------------------------------- //
Description :

. Produit de matrices

// ----------------------------------------------------------- */

void Mul2Mat(std::vector<std::vector<float>> &a, std::vector<std::vector<float>> &b, std::vector<std::vector<float>> &c) {
    int m = a.size();
    int n = b[0].size();
    int p = a[0].size();

    if (p != b.size()) {
        return;
    }

    c.assign(m, std::vector<float>(n, 0));

    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < p; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

/* ----------------------------------------------------------- //
Description :

. Produit d'Hadamard

// ----------------------------------------------------------- */
void Mul2Mat_Hadamard(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b, std::vector<std::vector<float>>& c) {
    int m = a.size();
    int n = a[0].size();

    if (m != b.size() || n != b[0].size()) {
        return; 
    }

    c.resize(m, std::vector<float>(n));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = a[i][j] * b[i][j];
        }
    }
}