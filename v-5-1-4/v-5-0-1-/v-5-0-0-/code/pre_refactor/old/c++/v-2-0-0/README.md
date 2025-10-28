# caca
 Cellular Automata denoising based Crypt Analysis - CACA.


# CACA - Cellular Automata CryptAnalysis

CACA (Cellular Automata-based Cryptanalysis) is a project that explores the use of Cellular Automata (CA) techniques for potential cryptanalysis applications. The project adapts methods from image denoising research to analyze patterns in encrypted data.

## Overview

This project combines two main approaches:
1. NIST Statistical Test Suite (NIST STS) for evaluating randomness in bit sequences
2. Cellular Automata (CA) for pattern detection and noise filtering

The idea is to test whether CA-based denoising techniques that are effective for recovering signals from noisy images (particularly salt-and-pepper noise in biometric applications) can also reveal patterns or weaknesses in encrypted data.

## Features

- Implementation of the NIST STS randomness testing suite (15 statistical tests)
- Various random number generator implementations for comparison
- Cellular Automata processing with Moore neighborhood rules
- Performance-optimized C++ implementation with multi-threading support

## Statistical Tests Included

- Frequency (Monobit) Test
- Block Frequency Test
- Runs Test
- Longest Run Test
- Binary Matrix Rank Test
- Discrete Fourier Transform (Spectral) Test
- Non-overlapping Template Matching Test
- Overlapping Template Matching Test
- Maurer's Universal Statistical Test
- Linear Complexity Test
- Serial Test
- Approximate Entropy Test
- Cumulative Sums Test
- Random Excursions Test
- Random Excursions Variant Test

## Random Number Generators

- Linear Congruential Generator (LCG)
- Blum-Blum-Shub (BBS)
- SHA-1 based generator
- XOR shift register
- Quadratic Congruential (two types)
- Cubic Congruential
- Micali-Schnorr
- Modular Exponentiation

## Theoretical Basis

The project is inspired by research showing that mirrored CA with Moore neighborhood (8-cell) rules can effectively denoise images, particularly salt-and-pepper noise in biometric applications. These CA techniques have shown superior performance compared to traditional methods, achieving better PSNR (Peak Signal-to-Noise Ratio), MSE (Mean Squared Error), and SSIM (Structural Similarity) metrics.

The hypothesis is that if encryption has minor imperfections, CA-based filtering might help reveal underlying patterns or biases.

# Performance Considerations

Multi-threading support
Configurable test parameters
Efficient big number arithmetic implementations

## Usage

Usage: `C:\Users\phine\Documents\GitHub\cascade\test_data\caca_app.exe [options]`

`File Input Options:`
`  -f <input_file>  Input file to test`
`  -a               ASCII mode (default is binary)`
`  -o <output_file> Output file (default is stdout)`
`  -l <length>      Length of bit sequence to test`

`Generator Options:`
`  -g <generator>   Test a specific generator`
`  -G               Test all available generators`
`  -L <length>      Length of sequence to generate (default: 1000000)`

`Performance Options:\n`
`  -i <iterations>  Number of iterations to run (default: 5)`

`Other Options:`
`  -h               Show this help message`
`  -list            List available generators`

## Example

`caca_app.exe -f encrypted_data.bin -i 4`

This command will run the analysis on the file `encrypted_data.bin` with 10 iterations of CA processing.


## References

This project is based on research from several key papers:
1. Suresh A, et al. "An improved cellular automata (ca) based image denoising method for biometric applications." Biomedical Research, 2018.
2. Rosin PL. "Image processing using 3-state cellular automata." Computer Vision Image Understanding, 2010.
3. Various papers on the NIST Statistical Test Suite (NIST STS) for randomness testing

## License
 
 This project is open source and available under the [MIT License](LICENSE). Feel free to use, modify, and distribute it as per the terms of the license.
 
## Acknowledgements

Claude 3.7 Sonnet, Claude 3.5 Sonnet, chatGPTo1, chatGPT4.5preview.