cascade/
├── .git/ [igniored]
├── .vscode/[ignored]
├── build/
├── docs/
├── previous/ [ignored]
├── src/
│   ├── generators/
│   │   ├── bbs.cpp
│   │   ├── bbs.hpp
│   │   ├── cubic_congruential.cpp
│   │   ├── cubic_congruential.hpp
│   │   ├── lcg.cpp
│   │   ├── lcg.hpp
│   │   ├── micali_schnorr.cpp
│   │   ├── micali_schnorr.hpp
│   │   ├── modular_exponentiation.cpp
│   │   ├── modular_exponentiation.hpp
│   │   ├── quadratic_congruential.cpp
│   │   ├── quadratic_congruential.hpp
│   │   ├── sha1.cpp
│   │   ├── sha1.hpp
│   │   ├── xor_generator.cpp
│   │   └── xor_generator.hpp
│   ├── nist_sts/
│   │   ├── tests/
│   │   │   ├── approximate_entropy_test.cpp
│   │   │   ├── approximate_entropy_test.hpp
│   │   │   ├── block_frequency_test.cpp
│   │   │   ├── block_frequency_test.hpp
│   │   │   ├── cumulative_sums_test.cpp
│   │   │   ├── cumulative_sums_test.hpp
│   │   │   ├── dft_test.cpp
│   │   │   ├── dft_test.hpp
│   │   │   ├── frequency_test.cpp
│   │   │   ├── frequency_test.hpp
│   │   │   ├── linear_complexity_test.cpp
│   │   │   ├── linear_complexity_test.hpp
│   │   │   ├── longest_run_test.cpp
│   │   │   ├── longest_run_test.hpp
│   │   │   ├── non_overlapping_template_test.cpp
│   │   │   ├── non_overlapping_template_test.hpp
│   │   │   ├── overlapping_template_test.cpp
│   │   │   ├── overlapping_template_test.hpp
│   │   │   ├── random_excursions_test.cpp
│   │   │   ├── random_excursions_test.hpp
│   │   │   ├── random_excursions_variant_test.cpp
│   │   │   ├── random_excursions_variant_test.hpp
│   │   │   ├── rank_test.cpp
│   │   │   ├── rank_test.hpp
│   │   │   ├── runs_test.cpp
│   │   │   ├── runs_test.hpp
│   │   │   ├── serial_test.cpp
│   │   │   ├── serial_test.hpp
│   │   │   ├── universal_test.cpp
│   │   │   └── universal_test.hpp
│   │   ├── generator_factory.cpp
│   │   ├── generator_factory.hpp
│   │   ├── test_suite.cpp
│   │   └── test_suite.hpp
│   ├── bitseqence_.cpp
│   ├── ca_analyzer.cpp
│   ├── ca_nist_analyzer.hpp
│   ├── common.cpp
│   ├── core_class_structure.cpp
│   ├── frequency.cpp
│   ├── main.cpp
│   ├── math_functions.hpp
│   ├── nist_sts.hpp
│   ├── random_number_generator.hpp
│   ├── simple_ca_test.exe
│   ├── stat_analyzer.hpp
│   ├── test_suite.cpp
│   ├── visualization_generator.cpp
│   ├── visualization_generator-big.cpp
│   └── visualization_generator-med.cpp
│
├── test_data/
│   ├── dataset_gen/
│   │   ├── caca_dataset_gen.txt
│   │   ├── 1KBr_esearch_paper.bin
│   │   ├── 500KB_Research_paper.bin
│   │   ├── 50MB_research_paper.bin
│   │   ├── jpg_droids_lolly_1.bin
│   │   ├── log_file_bash_history.bin
│   │   ├── ncpt_ceasar.py
│   │   ├── ncpt_cheesy.py
│   │   ├── ncpt_flawed_prng.py
│   │   ├── ncpt_shift_mix.py
│   │   └── ncpt_XOR_short.py
│   │
│   ├── 1KB_paper_aes_cbc_iv.bin
│   ├── 1KB_paper_aes_ctr.bin
│   ├── 1KB_paper_caesar.bin
│   ├── 1KB_paper_des.bin
│   ├── 1KB_paper_SHIFT_MIXd.bin
│   ├── 1KB_paperflawrng.bin
│   ├── 1KB_paperXORd.bin
│   ├── 1KB-0xFF.bin
│   ├── 1KB-cheesy.bin
│   ├── 1KB-DEADBEEF.bin
│   ├── 1KB-paper_aes_ecb.bin
│   ├── 50MB_paper_aes_cbc_iv.bin
│   ├── 50MB_paper_aes_ctr.bin
│   ├── 50MB_paper_caesar.bin
│   ├── 50MB_paper_des.bin
│   ├── 50MB_paper_SHIFT_MIXd.bin
│   ├── 50MB_paperflawrng.bin
│   ├── 50MB_paperXORd.bin
│   ├── 50MB-0xFF.bin
│   ├── 50MB-cheesy.bin
│   ├── 50MB-DEADBEEF.bin
│   ├── 50MB-paper_aes_ecb.bin
│   ├── 500KB_paper_aes_cbc_iv.bin
│   ├── 500KB_paper_aes_ctr.bin
│   ├── 500KB_paper_caesar.bin
│   ├── 500KB_paper_des.bin
│   ├── 500KB_paper_SHIFT_MIXd.bin
│   ├── 500KB_paperflawrng.bin
│   ├── 500KB_paperXORd.bin
│   ├── 500KB-0xFF.bin
│   ├── 500KB-cheesy.bin
│   ├── 500KB-DEADBEEF.bin
│   ├── 500KB-paper_aes_ecb.bin
│   ├── bash_history_ceasar.bin
│   ├── bash_history_flawedprng.bin
│   └── bash_history_shiftmx.bin
│
├── sts-2_1_2.zip [ignored]
├── .gitattributes [ignored]
├── .gitignore [ignored]
├── CMakeLists.txt
├── FILE_STRUCTURE.md [this file]
├── LICENSE
└── README.md


# 1. The NIST Statistical Test Suite (NIST STS) - A comprehensive framework for evaluating the randomness of bit sequences through various statistical tests:

Frequency Test (Monobit)
Block Frequency Test
Runs Test
Longest Run Test
Rank Test
DFT (Spectral) Test
Non-overlapping Template Matching
Overlapping Template Matching
Universal Test
Linear Complexity Test
Serial Test
Approximate Entropy Test
Cumulative Sums Test
Random Excursions Test
Random Excursions Variant Test


# 2. Various Random Number Generators:

Linear Congruential Generator (LCG)
Blum-Blum-Shub (BBS)
SHA-1 based generator
XOR shift register
Quadratic Congruential (two types)
Cubic Congruential
Micali-Schnorr
Modular Exponentiation


#3. Cellular Automata (CA) Implementation:

The paper you provided describes using mirrored CA for image denoising, particularly for biometric applications like fingerprints and iris scans
CA-based techniques can be effective at removing salt and pepper noise while preserving important features


# 4. Project Structure:

Modularly organized C++ codebase
Test suite framework
BitSequence class for bit manipulation
Implementation of mathematical functions needed for statistical tests
Various utility classes


# 5. The project is focused on:

- Applying CA techniques for noise removal in biometric data to encrypted data binaries.
- Comparing different approaches for effectiveness in terms of PSNR, MSE, and SSIM metrics derived from the above generated noise removed encrypted data.
- Testing the randomness of sequences generated by various algorithms and running these same tests upon the noise-removed encrypted data so as to getsome idea of the capabilities of theCA based noise removal functionality as fit for purposein spotting underlying patterns in encrypted data.
