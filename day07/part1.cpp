#include <fstream>
#include <string>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    std::vector<std::string> grid;
    std::ifstream file(argv[1]);
    std::string line;

    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    int rows = grid.size();
    int cols = grid[0].size();

    std::vector<int> dp(cols, 0);  // 0 = no beam, 1 = has beam

    // Find starting position
    for (int c = 0; c < cols; c++) {
        if (grid[0][c] == 'S') {
            dp[c] = 1;
            break;
        }
    }

    long long splits = 0;

    // Process even rows only (skip row 0, start at row 2)
    for (int r = 2; r < rows; r += 2) {
        std::vector<int> next(cols, 0);

        for (int c = 0; c < cols; c++) {
            if (!dp[c]) continue;

            if (grid[r][c] == '^') {
                splits++;
                if (c > 0) next[c-1] = 1;
                if (c < cols-1) next[c+1] = 1;
            } else {
                next[c] = 1;
            }
        }

        dp = std::move(next);

        // Debug: print current beam positions
        std::cout << "Row " << r << ": ";
        for (int c = 0; c < cols; c++) {
            if (dp[c]) std::cout << c << " ";
        }
        std::cout << "(splits so far: " << splits << ")\n";
    }

    // Count final beams
    int final_beams = 0;
    for (int c = 0; c < cols; c++) {
        if (dp[c]) final_beams++;
    }

    std::cout << "\nTotal splits: " << splits << std::endl;
    std::cout << "Final beam count: " << final_beams << std::endl;

    return 0;
}
// ```

// **Expected output for your input:**
// ```
// Row 2: 6 8 (splits so far: 1)
// Row 4: 5 7 9 (splits so far: 3)
// Row 6: 4 6 8 10 (splits so far: 6)
// Row 8: 3 5 9 11 (splits so far: 9)
// Row 10: 2 4 8 10 12 (splits so far: 13)
// Row 12: 1 5 11 13 (splits so far: 16)
// Row 14: 0 2 4 6 8 12 14 (splits so far: 21)

// Total splits: 21
// Final beam count: 7