#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

#define ALPHABET_SIZE 26

void read_distribution(const char *filename, double distribution[ALPHABET_SIZE]);
void compute_histogram(const char *text, double histogram[ALPHABET_SIZE]);
int read_text_from_file(const char *filename, char *buffer, int max_length);

#endif