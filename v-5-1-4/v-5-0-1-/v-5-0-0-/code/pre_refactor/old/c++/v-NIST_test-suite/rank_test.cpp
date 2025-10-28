// rank_test.cpp
#include "rank_test.hpp"
#include "../common.hpp"
#include "../math_functions.hpp"
#include <cmath>
#include <vector>

namespace nist_sts {

TestResult RankTest::execute(const BitSequence& data) {
    TestResult result;
    result.testName = getName();
    
    // For this test, we use 32x32 matrices
    const int M = 32;  // Number of rows
    const int Q = 32;  // Number of columns
    
    // Ensure we have enough data
    if (data.size() < M * Q) {
        result.p_value = 0.0;
        result.success = false;
        result.statistics["error"] = 1.0;
        return result;
    }
    
    // Calculate number of matrices
    int N = data.size() / (M * Q);
    
    // Calculate theoretical probabilities
    double p_32 = 0.2888;  // Probability of rank 32
    double p_31 = 0.5776;  // Probability of rank 31
    double p_30 = 0.1336;  // Probability of rank <= 30
    
    // Count matrices of each rank
    int F_32 = 0;
    int F_31 = 0;
    int F_30 = 0;
    
    // Process each matrix
    for (int k = 0; k < N; k++) {
        // Create matrix from the bit sequence
        std::vector<std::vector<bool>> matrix(M, std::vector<bool>(Q));
        
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < Q; j++) {
                matrix[i][j] = data[k * M * Q + i * Q + j];
            }
        }
        
        // Compute the rank
        int R = computeRank(matrix);
        
        // Update rank counts
        if (R == 32) {
            F_32++;
        } else if (R == 31) {
            F_31++;
        } else {
            F_30++;
        }
    }
    
    // Calculate chi-squared statistic
    double chi_squared = std::pow(F_32 - N * p_32, 2) / (N * p_32) +
                        std::pow(F_31 - N * p_31, 2) / (N * p_31) +
                        std::pow(F_30 - N * p_30, 2) / (N * p_30);
    
    // Calculate p-value
    double p_value = std::exp(-chi_squared / 2.0);
    
    // Store results
    result.statistics["num_matrices"] = N;
    result.statistics["F_32"] = F_32;
    result.statistics["F_31"] = F_31;
    result.statistics["F_30"] = F_30;
    result.statistics["p_32"] = p_32;
    result.statistics["p_31"] = p_31;
    result.statistics["p_30"] = p_30;
    result.statistics["chi_squared"] = chi_squared;
    result.statistics["discarded_bits"] = data.size() % (M * Q);
    
    result.p_value = p_value;
    result.success = isSuccess(p_value);
    
    return result;
}

int RankTest::computeRank(const std::vector<std::vector<bool>>& matrix) {
    // Create a copy of the matrix to work with
    std::vector<std::vector<bool>> work = matrix;
    
    // Apply Gaussian elimination to convert to row echelon form
    performForwardElimination(work);
    performBackwardElimination(work);
    
    // Count non-zero rows to determine rank
    int rank = 0;
    for (const auto& row : work) {
        bool isZeroRow = true;
        for (bool bit : row) {
            if (bit) {
                isZeroRow = false;
                break;
            }
        }
        if (!isZeroRow) {
            rank++;
        }
    }
    
    return rank;
}

void RankTest::performForwardElimination(std::vector<std::vector<bool>>& matrix) {
    int numRows = matrix.size();
    int numCols = matrix[0].size();
    
    int row = 0;
    for (int col = 0; col < numCols && row < numRows; col++) {
        // Find a pivot row
        int pivotRow = row;
        while (pivotRow < numRows && !matrix[pivotRow][col]) {
            pivotRow++;
        }
        
        if (pivotRow < numRows) {
            // Swap rows if needed
            if (pivotRow != row) {
                matrix[row].swap(matrix[pivotRow]);
            }
            
            // Eliminate entries below the pivot
            for (int i = row + 1; i < numRows; i++) {
                if (matrix[i][col]) {
                    // XOR with pivot row
                    for (int j = col; j < numCols; j++) {
                        matrix[i][j] = matrix[i][j] ^ matrix[row][j];
                    }
                }
            }
            
            row++;
        }
    }
}

void RankTest::performBackwardElimination(std::vector<std::vector<bool>>& matrix) {
    int numRows = matrix.size();
    int numCols = matrix[0].size();
    
    for (int row = numRows - 1; row > 0; row--) {
        // Find the pivot column in this row
        int pivotCol = 0;
        while (pivotCol < numCols && !matrix[row][pivotCol]) {
            pivotCol++;
        }
        
        if (pivotCol < numCols) {
            // Eliminate entries above the pivot
            for (int i = row - 1; i >= 0; i--) {
                if (matrix[i][pivotCol]) {
                    // XOR with current row
                    for (int j = pivotCol; j < numCols; j++) {
                        matrix[i][j] = matrix[i][j] ^ matrix[row][j];
                    }
                }
            }
        }
    }
}

} // namespace nist_sts