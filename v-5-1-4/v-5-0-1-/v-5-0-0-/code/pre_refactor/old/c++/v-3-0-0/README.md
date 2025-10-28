# caca
 Cellular Automata denoising based Crypt Analysis - CACA.

This project combines cellular automata techniques with cryptanalysis, specifically using techniques from image denoising research to analyze patterns in encrypted data.

#######


# #CACA - Cellular Automata CryptAnalysis

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

## Performance Considerations

Multi-threading support
Configurable test parameters
Efficient big number arithmetic implementations

## Usage

Usage:  << programName <<  [options]
    std::cerr << Options:
    std::cerr <<   -f, --file <file>        Input file to analyze
    std::cerr <<   -a, --ascii              Treat input as ASCII (default: binary)
    std::cerr <<   -o, --output <file>      Output file prefix
    std::cerr <<   -g, --generator <name>   Test specific random number generator
    std::cerr <<   -G, --all-generators     Test all available generators
    std::cerr <<   -l, --list               List available generators
    std::cerr <<   -i, --iterations <n>     Number of CA iterations (default: 5)
    std::cerr <<   -L, --length <n>         Squence length for generator tests (default: 1000000)
    std::cerr <<   -r, --ca-rules <r1,r2>   Comma-separated CA rules (default: 30,82,110,150)
    std::cerr <<   -v, --verbose            Verbose output
    std::cerr <<   -h, --help               Show this help message

## Examples
:: Run the analysis on the file `encrypted_data.bin` with 10 iterations of CA processing.
`caca_app.exe -f encrypted_data.bin -i 4`

:: Analyze an encrypted binary file with default CA rules
./caca_app -f encrypted.bin

:: Analyze an ASCII file with specific CA rules
./caca_app -f input.txt -a -r 30,110

:: Test a specific random number generator
./caca_app -g 'Linear Congruential' -L 500000

:: Test all available generators
./caca_app -G

:: List available generators
./caca_app -l


## References

This project is based on research from several key papers:
1. Suresh A, et al. "An improved cellular automata (ca) based image denoising method for biometric applications." Biomedical Research, 2018.
2. Rosin PL. "Image processing using 3-state cellular automata." Computer Vision Image Understanding, 2010.
3. Various papers on the NIST Statistical Test Suite (NIST STS) for randomness testing

## License
 
 This project is open source and available under the [MIT License](LICENSE). Feel free to use, modify, and distribute it as per the terms of the license.
 
## Acknowledgements

Claude 3.7 Sonnet, Claude 3.5 Sonnet, Claude 3.5 Haiku, chatGPTo1, chatGPT4.5preview.