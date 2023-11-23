#include <iostream>
#include <cmath>
#include <iomanip>

void printTable(double corr_ab, double corr_ac, double corr_bc, double norm_corr_ab, double norm_corr_ac, double norm_corr_bc) {
    std::cout << "\n\nCorrelation:\n";
    std::cout << std::setw(3) << std::left << "  ";
    std::cout << std::setw(10) << std::left << "a";
    std::cout << std::setw(10) << std::left << "b";
    std::cout << std::setw(10) << std::left << "c" << "\n";
    
    std::cout << std::setw(3) << std::left << "a";
    std::cout << std::setw(10) << std::left << "-";
    std::cout << std::setw(10) << std::left << corr_ab;
    std::cout << std::setw(10) << std::left << corr_ac << "\n";
    
    std::cout << std::setw(3) << std::left << "b";
    std::cout << std::setw(10) << std::left << corr_ab;
    std::cout << std::setw(10) << std::left << "-";
    std::cout << std::setw(10) << std::left << corr_bc << "\n";
    
    std::cout << std::setw(3) << std::left << "c";
    std::cout << std::setw(10) << std::left << corr_ac;
    std::cout << std::setw(10) << std::left << corr_bc;
    std::cout << std::setw(10) << std::left << "-";
    
    std::cout << "\n\nNormalized Correlation:\n";
    std::cout << std::setw(3) << std::left << "  ";
    std::cout << std::setw(10) << std::left << "a";
    std::cout << std::setw(10) << std::left << "b";
    std::cout << std::setw(10) << std::left << "c" << "\n";
    
    std::cout << std::setw(3) << std::left << "a";
    std::cout << std::setw(10) << std::left << "-";
    std::cout << std::setw(10) << std::left << norm_corr_ab;
    std::cout << std::setw(10) << std::left << norm_corr_ac << "\n";
    
    std::cout << std::setw(3) << std::left << "b";
    std::cout << std::setw(10) << std::left << norm_corr_ab;
    std::cout << std::setw(10) << std::left << "-";
    std::cout << std::setw(10) << std::left << norm_corr_bc << "\n";
    
    std::cout << std::setw(3) << std::left << "c";
    std::cout << std::setw(10) << std::left << norm_corr_ac;
    std::cout << std::setw(10) << std::left << norm_corr_bc;
    std::cout << std::setw(10) << std::left << "-";
}

double correlation(const double* a, const double* b, int N) {
    double corr = 0.0;

    for (int n = 0; n < N; ++n) {
        corr += a[n] * b[n];
    }

    return corr;
}

double normalizedCorrelation(const double* a, const double* b, int N) {
    double corrxy = correlation(a, b, N);
    double normx = 0.0;
    double normy = 0.0;

    for (int n = 0; n < N; ++n) {
        normx += std::pow(a[n], 2);
        normy += std::pow(b[n], 2);
    }

    return corrxy / (std::sqrt(normx) * std::sqrt(normy));
}

int main() {
    const int N = 8;
    double array_a[N] = {2, 3, 6, -1, -4, -2, 2, 3};
    double array_b[N] = {4, 6, 8, -2, -6, -4, 2, 7};
    double array_c[N] = {-3, -1, 3, -7, 2, -8, 3, -1};

    double corr_ab = correlation(array_a, array_b, N);
    double norm_corr_ab = normalizedCorrelation(array_a, array_b, N);

    double corr_ac = correlation(array_a, array_c, N);
    double norm_corr_ac = normalizedCorrelation(array_a, array_c, N);

    double corr_bc = correlation(array_b, array_c, N);
    double norm_corr_bc = normalizedCorrelation(array_b, array_c, N);

    std::cout << "Correlation (a, b): " << corr_ab << std::endl;
    std::cout << "Normalized Correlation (a, b): " << norm_corr_ab << std::endl;

    std::cout << "Correlation (a, c): " << corr_ac << std::endl;
    std::cout << "Normalized Correlation (a, c): " << norm_corr_ac << std::endl;

    std::cout << "Correlation (b, c): " << corr_bc << std::endl;
    std::cout << "Normalized Correlation (a, c): " << norm_corr_bc << std::endl;

    printTable(corr_ab, corr_ac, corr_bc, norm_corr_ab, norm_corr_ac, norm_corr_bc);


    return 0;
}
