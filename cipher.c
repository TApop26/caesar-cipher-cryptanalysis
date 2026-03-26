#include "cipher.h"
#include "text_analysis.h"
#include <ctype.h>
#include <string.h>

void shift_string(const char* input, char* output, int shift) {
    shift = shift % ALPHABET_SIZE;
    if (shift < 0) shift += ALPHABET_SIZE;

    int i = 0;
    while (input[i] != '\0') {
        if (isalpha(input[i])) {
            char base = islower(input[i]) ? 'a' : 'A';
            output[i] = (input[i] - base + shift) % ALPHABET_SIZE + base;
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

void break_caesar_cipher(const char* text, int top_shifts[TOP_N], double top_distances[TOP_N], DistanceFunc distance_function) {
    double english_dist[ALPHABET_SIZE];
    read_distribution("distribution.txt", english_dist);

    double distances[ALPHABET_SIZE];

    // Evaluate all possible 26 shifts
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        // Assume MAX_TEXT is large enough for the context of this function
        char temp_text[4096];
        shift_string(text, temp_text, -shift);

        double current_hist[ALPHABET_SIZE];
        compute_histogram(temp_text, current_hist);

        distances[shift] = distance_function(current_hist, english_dist);
    }

    // Initialize top distances to a very high number
    for (int i = 0; i < TOP_N; i++) top_distances[i] = 999999.0;

    // Rank the top 3 lowest distances
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        double d = distances[shift];
        for (int i = 0; i < TOP_N; i++) {
            if (d < top_distances[i]) {
                for (int j = TOP_N - 1; j > i; j--) {
                    top_distances[j] = top_distances[j-1];
                    top_shifts[j] = top_shifts[j-1];
                }
                top_distances[i] = d;
                top_shifts[i] = shift;
                break;
            }
        }
    }
}