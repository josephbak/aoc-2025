#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char** argv) {
    // ... read grid ...
    std::vector<std::string> grid;
    std::ifstream file(argv[1]);
    std::string line;

    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    int rows = grid.size();
    int cols = grid[0].size();
    
    // dp[r][c] = 1 if beam present at (r, c)
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));
    
    // Initialize: find S
    for (int c = 0; c < cols; c++) {
        if (grid[0][c] == 'S') {
            dp[0][c] = 1;
        }
    }
    
    int splits = 0;
    
    // Fill table row by row
    for (int r = 2; r < rows; r += 2) {
        int prev_r = r - 2;
        
        for (int c = 0; c < cols; c++) {
            // Where could beam come from?
            
            // From directly above (previous cell was not '^')
            if (grid[prev_r][c] != '^' && dp[prev_r][c]) {
                dp[r][c] = 1;
            }
            
            // From above-left (previous cell was '^')
            if (c > 0 && grid[prev_r][c-1] == '^' && dp[prev_r][c-1]) {
                dp[r][c] = 1;
            }
            
            // From above-right (previous cell was '^')
            if (c < cols-1 && grid[prev_r][c+1] == '^' && dp[prev_r][c+1]) {
                dp[r][c] = 1;
            }
        }
        
        // Count splits this row
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '^' && dp[r][c]) {
                splits++;
            }
        }
    }
    
    std::cout << splits << std::endl;
}