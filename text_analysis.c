#define _CRT_SECURE_NO_WARNINGS
#include "text_analysis.h"
#include <stdio.h>
#include <ctype.h>

void read_distribution(const char *filename, double distribution[ALPHABET_SIZE]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Warning: Could not open %s. Using default English frequencies.\n", filename);
        double default_dist[ALPHABET_SIZE] = {8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.2, 0.8, 4.0, 2.4, 6.7, 7.5, 1.9, 0.1, 6.0, 6.3, 9.1, 2.8, 1.0, 2.4, 0.2, 2.0, 0.1};
        for(int i = 0; i < ALPHABET_SIZE; i++) distribution[i] = default_dist[i];
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        fscanf(file, "%lf", &distribution[i]);
    }
    fclose(file);
}

void compute_histogram(const char *text, double histogram[ALPHABET_SIZE]) {
    int counts[ALPHABET_SIZE] = {0};
    int total_letters = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            int index = tolower(text[i]) - 'a';
            counts[index]++;
            total_letters++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (total_letters > 0) {
            histogram[i] = ((double)counts[i] / total_letters) * 100.0;
        } else {
            histogram[i] = 0.0;
        }
    }
}

int read_text_from_file(const char *filename, char *buffer, int max_length) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    size_t newLen = fread(buffer, sizeof(char), max_length - 1, file);
    if (ferror(file) != 0) {
        fclose(file);
        return 0;
    }
    buffer[newLen++] = '\0'; // Null-terminate
    fclose(file);
    return 1;
}