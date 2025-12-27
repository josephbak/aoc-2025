#include <fstream>
#include <string>
#include <utility>
#include <iostream>
#include <unordered_set>

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;

    std::unordered_set<int> tmp;
    std::unordered_set<int> beams;

    long long row = 0;
    long long ans = 0;
    
    while (std::getline(file, line)) {
        // std::cout << "row: " << row << std::endl;
        if (row & 1) {
            row++;
            continue;
        }

        // only process even number rows
        if (row == 0) {
            // find S where the beam starts
            for (size_t col = 0; col < line.size(); col++) {
                if (line[col] == 'S') {
                    tmp.insert(col);
                }
            }
        } else {
            // std::cout << "row: " << row << std::endl;
            for (size_t col = 0; col < line.size(); col++) {
                if ((line[col] == '^') && beams.count(col)) {
                    // when the beam gets splitted
                    ans++;
                    tmp.insert(col-1);
                    tmp.insert(col+1);
                } else if ((line[col] == '.') && beams.count(col)) {
                    // when the beam just goes through
                    tmp.insert(col);
                }
            }
        }
        row++;
        beams = std::move(tmp);
        tmp.clear();

        std::cout << "row: " << row << " curr_count: " << ans << std::endl;
    }

    std::cout << "count is: " << ans << std::endl;
    return 0;

}