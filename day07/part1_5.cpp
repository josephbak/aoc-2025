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
    
    std::vector<int> dp(cols, 0);      // current row
    std::vector<int> next(cols, 0);    // next row
    
    // Initialize
    for (int c = 0; c < cols; c++) {
        if (grid[0][c] == 'S') dp[c] = 1;
    }
    
    int splits = 0;
    
    for (int r = 2; r < rows; r += 2) {
        std::fill(next.begin(), next.end(), 0);  // reset next
        
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
        
        std::swap(dp, next);
    }
    
    std::cout << splits << std::endl;
}