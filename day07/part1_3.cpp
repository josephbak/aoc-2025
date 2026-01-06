#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::vector<int>> memo;  // -1 = unvisited, 0/1 = no/yes beam

int splits = 0;

int hasBeam(int row, int col, std::vector<std::string>& grid) {
    int cols = grid[0].size();
    
    // Bounds check
    if (col < 0 || col >= cols) return 0;
    
    // Base case: row 0
    if (row == 0) return (grid[0][col] == 'S') ? 1 : 0;
    
    // Already computed?
    if (memo[row][col] != -1) return memo[row][col];
    
    int prev_row = row - 2;
    if (prev_row < 0) return memo[row][col] = 0;
    
    // Case 1: Beam came straight down
    // Previous cell was NOT '^' and had a beam
    if (grid[prev_row][col] != '^' && hasBeam(prev_row, col, grid)) {
        return memo[row][col] = 1;
    }
    
    // Case 2: Beam came from above-left (col-1)
    // Previous cell at col-1 was '^' and had a beam → split right to col
    if (col - 1 >= 0 && grid[prev_row][col-1] == '^' && hasBeam(prev_row, col-1, grid)) {
        return memo[row][col] = 1;
    }
    
    // Case 3: Beam came from above-right (col+1)
    // Previous cell at col+1 was '^' and had a beam → split left to col
    if (col + 1 < cols && grid[prev_row][col+1] == '^' && hasBeam(prev_row, col+1, grid)) {
        return memo[row][col] = 1;
    }
    
    return memo[row][col] = 0;
}

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
    
    memo.assign(rows, std::vector<int>(cols, -1));
    
    // Count splits: cells with '^' that have a beam
    int splits = 1;
    for (int r = 2; r < rows; r += 2) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '^' && hasBeam(r, c, grid)) {
                splits *= 2;
            }
        }
    }

    std::cout << "Splits: " << splits << std::endl;
}