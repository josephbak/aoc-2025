#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

// int countPaths(int row, int col, std::vector<std::string>& grid) {
//     // Reached bottom → one complete path
//     if (row >= grid.size()) return 1;
    
//     // Out of bounds → no path
//     if (col < 0 || col >= grid[0].size()) return 0;
    
//     if (grid[row][col] == '^') {
//         // Split: count paths from both branches
//         return countPaths(row + 2, col - 1, grid) 
//              + countPaths(row + 2, col + 1, grid);
//     } else {
//         // Continue straight
//         return countPaths(row + 2, col, grid);
//     }
// }

std::map<std::pair<int, int>, long long> memo;

long long countPaths(int row, int col, std::vector<std::string>& grid) {
    // Reached bottom → one complete path
    if (row >= grid.size()) return 1;
    
    // Out of bounds → no path
    if (col < 0 || col >= grid[0].size()) return 0;
    
    // Already computed?
    if (memo.count({row, col})) return memo[{row, col}];
    
    long long result;
    if (grid[row][col] == '^') {
        result = countPaths(row + 2, col - 1, grid) 
               + countPaths(row + 2, col + 1, grid);
    } else {
        result = countPaths(row + 2, col, grid);
    }
    
    return memo[{row, col}] = result;
}

int find_S(std::vector<std::string>& grid) {
    for (int c = 0; c < grid[0].size(); c++) {
        if (grid[0][c] == 'S') return c;
    }
    return 0;
}

int main(int argc, char** argv) {
    std::vector<std::string> grid;
    std::ifstream file(argv[1]);
    std::string line;

    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    int start_col = find_S(grid);
    long long paths = countPaths(0, start_col, grid);
    std::cout << "Paths: " << paths << std::endl;
}