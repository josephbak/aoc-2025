#include <fstream>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <numeric>

long long arith(char op, std::vector<long long>& vec) {
    // for (size_t i = 0; i < vec.size(); i++) {
    //     std::cout << i << ": " << vec[i] << "\n";
    // }

    if (op == '+') {
        return std::accumulate(vec.begin(), vec.end(), 0LL);
    }
    return std::accumulate(vec.begin(), vec.end(), 1LL, std::multiplies<int>());
}


int main(int argc, char** argv) {

    std::vector<std::vector<char>> matrix;
    std::ifstream file(argv[1]);
    std::string line;

    while (std::getline(file, line)) {
        matrix.push_back(std::vector<char>(line.begin(), line.end()));
    }

    size_t rows = matrix.size();
    size_t cols = matrix[0].size();  // assumes non-empty, equal-length rows

    std::cout << "rows: " << rows << ", cols: " << cols << std::endl;

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            std::cout << matrix[row][col];
        }
        std::cout << '\n';
    }
    std::cout << '\n';


    long long ans = 0;

    std::vector<long long> tmp;
    char curr_op = ' ';
    long long curr_val = 0;

    int curr_col = cols - 1; // last column
    int curr_row = rows - 1; // last row

    while (curr_col >= 0) {

        curr_row = rows - 2;

        std::cout << rows - 1 << std::endl;

        if ((matrix[rows-1][curr_col] == '+') || (matrix[rows-1][curr_col] == '*')) {
        std::cout << "hihihihi" << std::endl;

            curr_op = matrix[rows-1][curr_col]; // get the operation

            while (curr_row >= 0) {
                if (matrix[curr_row][curr_col] != ' ') {
                    curr_val = static_cast<long long>(std::pow(10, rows - 2 - curr_row))*(matrix[curr_row][curr_col]) + curr_val;
                }

                // if (curr_row == 0) {
                //     break;
                // }

                curr_row--;
            }

            tmp.push_back(curr_val);

            curr_op = ' '; // reset
            curr_val = 0; // reset

            for (size_t i = 0; i < tmp.size(); i++) {
                std::cout << i << ": " << tmp[i] << "\n";
            }

            ans += arith(curr_op, tmp);
            // tmp = {}; // empty the vector
            tmp.clear();

            curr_col -= 2; // 2 cols jump
        } else {
            // skip the last row since it's empty

            std::cout << "hihih" << std::endl;

            while (curr_row >= 0) {
            // std::cout << "sdsafdasf" << std::endl;
            std::cout << "curr row: " << curr_row << ", curr_col: " << curr_col << std::endl;

                if (matrix[curr_row][curr_col] != ' ') {
                    curr_val = static_cast<long long>(std::pow(10, rows - 2 - curr_row))*(matrix[curr_row][curr_col]) + curr_val;
                }

                // if (curr_row == 0) {
                //     break;
                // }
                curr_row--;
            }
            std::cout << "do we get here" << std::endl;

            tmp.push_back(curr_val);
            curr_col -= 1;

            std::cout << "do we get here" << std::endl;
        }

    }

    std::cout << "count is: " << ans << std::endl;
    return 0;
}