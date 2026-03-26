#include "metrics.h"
#include <math.h>

double chi_squared_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double distance = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (hist2[i] > 0) {
            distance += pow(hist1[i] - hist2[i], 2) / hist2[i];
        }
    }
    return distance;
}

double euclidean_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double distance = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        distance += pow(hist1[i] - hist2[i], 2);
    }
    return sqrt(distance);
}

double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double dot_product = 0.0, mag1 = 0.0, mag2 = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        dot_product += hist1[i] * hist2[i];
        mag1 += pow(hist1[i], 2);
        mag2 += pow(hist2[i], 2);
    }
    if (mag1 == 0 || mag2 == 0) return 1.0; 
    return 1.0 - (dot_product / (sqrt(mag1) * sqrt(mag2)));
}