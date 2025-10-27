#include "MathCalculator.h"
#include <cmath>

double MathCalculator::performHeavyCalculations() {
    double result = 0.0;
    
    // Ağır riyazi əməliyyatlar
    for (int i = 1; i < 1000; i++) {
        result += std::sqrt(i * 3.14159265359);
        result *= std::sin(static_cast<double>(i));
        result /= std::cos(static_cast<double>(i + 1));
        result = std::pow(std::abs(result), 0.5);
    }
    
    // Prime ədəd yoxlaması
    for (int n = 2; n < 500; n++) {
        bool isPrime = true;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                isPrime = false;
                break;
            }
        }
        result += isPrime ? 1.0 : 0.0;
    }
    
    // Matrix əməliyyatları
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            result += std::sin(i) * std::cos(j);
        }
    }
    
    return result;
}
