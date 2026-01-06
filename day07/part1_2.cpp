#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>

int splits = 0;

int find_S(std::vector<std::string>& grid) {
    int cols = grid[0].size();
    
    for (int c = 0; c < cols; c++) {
    if (grid[0][c] == 'S') {
        return c;
        // break;
        }
    }
    return 0;
}

// Level 0: Brute Force
// void simulate(int row, int col, std::vector<std::string>& grid) {
//     if (row >= grid.size()) return;  // out of bounds
//     if (col < 0 || col >= grid[0].size()) return;
    
//     if (grid[row][col] == '^') {
//         splits++;
//         simulate(row + 2, col - 1, grid);  // go left
//         simulate(row + 2, col + 1, grid);  // go right
//     } else {
//         simulate(row + 2, col, grid);      // go straight
//     }
// }

// Level 1: Memoization
std::set<std::pair<int,int>> visited;
void simulate(int row, int col, std::vector<std::string>& grid) {
    if (row >= grid.size()) return;
    if (col < 0 || col >= grid[0].size()) return;
    
    // Already processed this position?
    if (visited.count({row, col})) return;
    visited.insert({row, col});
    
    if (grid[row][col] == '^') {
        splits++;
        simulate(row + 2, col - 1, grid);
        simulate(row + 2, col + 1, grid);
    } else {
        simulate(row + 2, col, grid);
    }
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


    int start_col = find_S(grid);
    simulate(0, start_col, grid);
    std::cout << splits << std::endl;
}