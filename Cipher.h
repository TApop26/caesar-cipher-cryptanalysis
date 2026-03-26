#ifndef CIPHER_H
#define CIPHER_H

#define TOP_N 3

// Function pointer typedef for cleaner code
typedef double (*DistanceFunc)(const double[], const double[]);

void shift_string(const char* input, char* output, int shift);
void break_caesar_cipher(const char* text, int top_shifts[TOP_N], double top_distances[TOP_N], DistanceFunc distance_function);

#endif