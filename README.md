# Caesar Cipher Cryptanalysis (C)

A small C tool that encrypts and decrypts Caesar-cipher text and **breaks unknown shifts automatically** with letter-frequency analysis. It compares each candidate decryption against English letter frequencies using one of three distance metrics.

## What & why

A Caesar cipher shifts every letter by a fixed amount. That changes *which* letters appear, but not *how often* each one appears. So if you try all 26 shifts, the correct one is the decryption whose letter histogram looks most like English.

I built this as an extra-credit assignment for an OOP/C course to practise:
- modular C (separate translation units and headers);
- function pointers, so the distance metric is swappable at runtime;
- simple statistical cryptanalysis.

## Results

Run on the test files in this repository:

| Input | Chi-squared | Euclidean | Cosine |
|---|---|---|---|
| `test4.txt`: real ciphertext, 2 sentences | ✅ shift 7 | ✅ shift 7 | ✅ shift 7 |
| `test1.txt` encrypted with shift 11 (1 paragraph) | ✅ 11 | ✅ 11 | ✅ 11 |
| `test2.txt` encrypted with shift 11 ("Meet me at noon.") | ✅ 11 | ✅ 11 | ✅ 11 |
| `test3.txt` encrypted with shift 11 (z-heavy pangram) | ❌ 7 | ❌ 6 | ❌ 6 |

`test4.txt` decrypts to *"Cryptography is the practice and study of techniques for secure communication in the presence of adversarial behavior. …"*

On that file the correct shift wins by a wide margin. Chi-squared distance is 20.5 for shift 7 versus 235.0 for the runner-up.

`test3.txt` is the deliberate failure case: a sentence stuffed with z's has a letter distribution nothing like English, so frequency analysis picks the wrong shift with every metric.

## How it works

1. `compute_histogram()` counts letters case-insensitively and turns the counts into percentages.
2. `break_caesar_cipher()` tries all 26 shifts. For each one it decrypts the text, computes its histogram, and measures the distance to the English reference distribution in `distribution.txt`. If that file is missing, it uses built-in default frequencies.
3. The distance function is passed in as a `DistanceFunc` pointer. Three are available:
   - **chi-squared**: Σ (observed − expected)² / expected
   - **Euclidean**: √Σ (observed − expected)²
   - **cosine distance**: 1 − cos θ between the two histograms
4. The three shifts with the smallest distance are shown, each with its full decrypted text.

The interactive menu covers:
1. Read text from the keyboard
2. Load text from a file
3. Encrypt with a shift
4. Decrypt with a known shift
5. Show the letter distribution
6. Break the cipher
7. Exit

## Tech stack

C11 · CMake · standard library only (`stdio`, `string`, `ctype`, `math`)

## How to build & run

Tested on Windows 11 with MinGW gcc 13.1 (bundled with CLion), CMake 4.1 and Ninja. `CMakeLists.txt` requires **CMake ≥ 4.1**.

```bash
cmake -S . -B build -G Ninja
cmake --build build

# Run from the repository root so distribution.txt and the test files are found
./build/EXTRACREDIT
```

Example session (break `test4.txt` with chi-squared), with the input piped in:

```bash
printf "2\ntest4.txt\n6\n1\n7\n" | ./build/EXTRACREDIT
```

## Project structure

```
main.c                     interactive menu
cipher.c / Cipher.h        shift_string(), break_caesar_cipher(), DistanceFunc
metrics.c / Metrics.h      chi-squared, Euclidean, cosine distance
text_analysis.c / .h       histogram, file reading, reference distribution loader
distribution.txt           English letter frequencies (%) A–Z
test1.txt … test4.txt      sample plaintexts (1–3) and ciphertext (4)
```

## Limitations & next steps

- **Builds on Windows/macOS only.** The sources include `"cipher.h"` and `"metrics.h"`, but the files are named `Cipher.h` and `Metrics.h`. That works on case-insensitive filesystems (Windows, macOS) but fails on Linux.
- **Short or unusual texts fail.** Frequency analysis needs enough ordinary English. `test3.txt` shows the failure mode. Bigram/trigram scoring would be more robust.
- **Fixed 4 KB buffer.** Input is limited to 4,095 characters.
- **ASCII only.** Non-ASCII letters are not handled.
