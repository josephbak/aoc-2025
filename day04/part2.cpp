#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <iostream>

std::set<std::pair<int, int>> helper(std::set<std::pair<int, int>> coords) {

    std::set<std::pair<int, int>> remov;

    // int ans = 0;
    int tmp = 0;

    for (const auto& [r, c] : coords) {
        tmp = 0;
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;  // skip self
                int nr = r + dr;
                int nc = c + dc;

                // use (nr, nc)
                if (coords.count({nr, nc})) {
                    // exists
                    tmp++;
                }

            }
        }
        if (tmp < 4) {
            // ans++;
            remov.insert({r, c}); // need to remove this pair
        }
    }

    return remov;
}


int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    
    int rows = 0;
    int cols = 0;
    std::set<std::pair<int, int>> coords;
    // std::vector<std::pair<int, int>> coords;  // (row, col)
    
    while (std::getline(file, line)) {
        if (rows == 0) cols = line.size();
        
        for (int c = 0; c < line.size(); c++) {
            if (line[c] == '@') {  // whatever character you're looking for
                coords.insert({rows, c});
                // coords.push_back({rows, c});
            }
        }
        rows++;
    }


    // int ans = 0;
    // int tmp = 0;

    // for (const auto& [r, c] : coords) {
    //         // std::cout << "(" << r << ", " << c << ")" << std::endl;
    //         tmp = 0;
    //         for (int dr = -1; dr <= 1; dr++) {
    //             for (int dc = -1; dc <= 1; dc++) {
    //                 if (dr == 0 && dc == 0) continue;  // skip self
    //                 int nr = r + dr;
    //                 int nc = c + dc;
    //                 // use (nr, nc)
    //                 if (coords.count({nr, nc})) {
    //                     // exists
    //                     tmp++;
    //                 }
    //             }
    //         }
    //         if (tmp < 4) {
    //             ans++;
    //         }
    // }

    int ans = 0;

    while (true) {
        auto remov = helper(coords);
        if (remov.empty()) break;

        ans += remov.size(); // get the number of elements to remove
        for (const auto& [r, c] : remov) {
            coords.erase({r, c});
        }
    }

    std::cout << "ans is: " << ans << std::endl;
    return 0;

}