#include <stdio.h>
#include <string.h>
#include "metrics.h"
#include "text_analysis.h"
#include "cipher.h"

#define MAX_TEXT 4096

void display_menu() {
    printf("\n---  Caesar Cipher  ---\n");
    printf("1. Read text from keyboard\n");
    printf("2. Read text from a file\n");
    printf("3. Encrypt text with a shift\n");
    printf("4. Decrypt text with a known shift\n");
    printf("5. Display frequency distribution\n");
    printf("6. Break cipher (Frequency Analysis)\n");
    printf("7. Exit\n");
    printf("Choose an option: ");
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    char current_text[MAX_TEXT] = {0};
    int choice;

    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 1) {
            printf("Enter text: ");
            fgets(current_text, MAX_TEXT, stdin);
            current_text[strcspn(current_text, "\n")] = 0;
            printf("Text stored.\n");
        }
        else if (choice == 2) {
            char filename[256];
            printf("Enter filename: ");
            fgets(filename, 256, stdin);
            filename[strcspn(filename, "\n")] = 0;

            if (read_text_from_file(filename, current_text, MAX_TEXT)) {
                printf("Text loaded.\n");
            } else {
                printf("Error loading file.\n");
            }
        }
        else if (choice == 3) {
            int shift;
            char encrypted[MAX_TEXT];
            printf("Enter shift amount: ");
            scanf("%d", &shift);
            shift_string(current_text, encrypted, shift);
            printf("Encrypted text: %s\n", encrypted);
            strcpy(current_text, encrypted);
        }
        else if (choice == 4) {
            int shift;
            char decrypted[MAX_TEXT];
            printf("Enter known shift amount: ");
            scanf("%d", &shift);
            shift_string(current_text, decrypted, -shift);
            printf("Decrypted text: %s\n", decrypted);
            strcpy(current_text, decrypted);
        }
        else if (choice == 5) {
            double hist[ALPHABET_SIZE];
            compute_histogram(current_text, hist);
            printf("Frequency Distribution:\n");
            for(int i = 0; i < ALPHABET_SIZE; i++) {
                printf("%c: %.2f%%\n", 'A' + i, hist[i]);
            }
        }
        else if (choice == 6) {
            if (strlen(current_text) == 0) {
                printf("Please enter or load some text first.\n");
                continue;
            }

            int metric;
            printf("Select metric (1-Chi-Squared, 2-Euclidean, 3-Cosine): ");
            scanf("%d", &metric);

            int top_shifts[TOP_N];
            double top_distances[TOP_N];
            DistanceFunc func = chi_squared_distance; // Default

            if (metric == 2) func = euclidean_distance;
            else if (metric == 3) func = cosine_distance;

            break_caesar_cipher(current_text, top_shifts, top_distances, func);

            printf("\nTop 3 probable shifts:\n\n");
            for (int i = 0; i < TOP_N; i++) {
                char decoded[MAX_TEXT];
                shift_string(current_text, decoded, -top_shifts[i]);

                // Truncation removed! The full string will now print.
                // Added an extra \n at the end so the 3 results are separated by a blank line.
                printf("%d. Shift %d (Distance: %.4f) -> \n%s\n\n", i+1, top_shifts[i], top_distances[i], decoded);
            }
        }
    } while (choice != 7);

    printf("Exiting...\n");
    return 0;
}