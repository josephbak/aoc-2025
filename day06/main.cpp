#include <fstream>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;

    std::vector<std::vector<std::string>> matrix;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string token;
        while (ss >> token) {
            row.push_back(token);
        }
        matrix.push_back(row);
    }


    // Dimensions
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();  // assumes all rows same length

    long long ans = 0;

    for (size_t col = 0; col < cols; col++) {
        bool is_mult = (matrix[rows - 1][col] == "*");
        long long tmp = is_mult ? 1 : 0;  // identity element

        for (size_t row = 0; row < rows - 1; row++) {
            long long var = std::stoll(matrix[row][col]);
            if (is_mult) {
                tmp *= var;
            } else {
                tmp += var;
            }
        }
    ans += tmp;
    }

    // lambda
//     for (size_t col = 0; col < cols; col++) {
//         bool is_mult = (matrix[rows - 1][col] == "*");
//         auto op = is_mult ? std::multiplies<long long>{}
//                         : std::plus<long long>{};
        
//         long long tmp = is_mult ? 1 : 0;
//         for (size_t row = 0; row < rows - 1; row++) {
//             tmp = op(tmp, std::stoll(matrix[row][col]));
//         }
//         ans += tmp;
// }

    std::cout << "count is: " << ans << std::endl;
    return 0;
}