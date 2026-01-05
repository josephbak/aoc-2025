#include <fstream>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
    std::vector<std::vector<char>> matrix;
    std::ifstream file(argv[1]);
    std::string line;

    while (std::getline(file, line)) {
        matrix.push_back(std::vector<char>(line.begin(), line.end()));
    }

    int rows = matrix.size();
    int cols = matrix[0].size();

    // Find all operator positions in last row
    std::vector<int> op_positions;
    for (int c = 0; c < cols; c++) {
        if (matrix[rows-1][c] == '+' || matrix[rows-1][c] == '*') {
            op_positions.push_back(c);
        }
    }

    long long ans = 0;

    // Process each operator group
    for (size_t i = 0; i < op_positions.size(); i++) {
        int op_col = op_positions[i];
        char op = matrix[rows-1][op_col];
        
        // Column range: from this operator to just before next operator
        int start_col = op_col;
        int end_col = (i + 1 < op_positions.size()) 
                      ? op_positions[i+1] - 1 
                      : cols - 1;

        // Collect numbers from each column
        std::vector<long long> nums;
        
        for (int c = start_col; c <= end_col; c++) {
            long long num = 0;
            bool has_digit = false;
            
            // Read top to bottom
            for (int r = 0; r < rows - 1; r++) {
                if (c < (int)matrix[r].size() && matrix[r][c] != ' ') {
                    num = num * 10 + (matrix[r][c] - '0');
                    has_digit = true;
                }
            }
            
            if (has_digit) {
                nums.push_back(num);
            }
        }

        // Apply operator
        long long result;
        if (op == '+') {
            result = std::accumulate(nums.begin(), nums.end(), 0LL);
        } else {
            result = std::accumulate(nums.begin(), nums.end(), 1LL, std::multiplies<long long>());
        }

        // debugging
        // std::cout << "Op '" << op << "' cols " << start_col << "-" << end_col << ": ";
        // for (auto n : nums) std::cout << n << " ";
        // std::cout << "= " << result << "\n";

        ans += result;
    }

    std::cout << "Final answer: " << ans << "\n";
    return 0;
}