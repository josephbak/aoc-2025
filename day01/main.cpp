#include <fstream>
#include <string>
#include <iostream>


int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    int ans = 0;
    int curr = 50;

    while (std::getline(file, line)) {
        int steps = std::stoi(line.substr(1));

        if (line[0] == 'R') {
            // going right: we cross 0 each time we go from 99→0
            // that happens when (curr + k) % 100 == 0 for some k in [1, steps]
            // i.e., when k = (100 - curr) % 100, then k + 100, k + 200, ...
            int first_zero = (100 - curr) % 100;
            if (first_zero == 0) first_zero = 100;  // curr=0 means first zero is at step 100
            int crossings = (first_zero <= steps) ? (steps - first_zero) / 100 + 1 : 0;
            ans += crossings;
            curr = (curr + steps) % 100;
        } else {
            // going left: we cross 0 each time we go from 1→0
            // that happens when (curr - k) % 100 == 0 for some k in [1, steps]
            int first_zero = curr;
            if (first_zero == 0) first_zero = 100;  // curr=0 means first zero is at step 100
            int crossings = (first_zero <= steps) ? (steps - first_zero) / 100 + 1 : 0;
            ans += crossings;
            curr = ((curr - steps) % 100 + 100) % 100;
        }
    }
    std::cout << ans << std::endl;
    return 0;
}