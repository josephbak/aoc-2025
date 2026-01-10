#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <climits>
#include <numeric>

struct Machine {
    int n; // number of lights
    uint64_t target;    //target as bitmask
    std::vector<uint64_t> buttons;  // each button as bitmask
};

uint64_t parse_target(const std::string& token) {
    // token is like "[.##.]"
    // skip first char '[' and last char ']'
    // for each '#', set that bit
    
    uint64_t mask = 0;
    for (int i = 1; i < token.size() - 1; i++) {
        if (token[i] == '#') {
            // TODO: set bit (i-1) in mask
            mask |= (1ULL << (i-1));
        }
    }
    return mask;
}

std::vector<int> parse_button_part2(const std::string& token) {

    std::vector<int> button;
    // Extract content between braces
    std::string content = token.substr(1, token.size() - 2);
    
    // Split by comma and process each number
    std::istringstream iss(content);
    std::string num_str;
    
    while (std::getline(iss, num_str, ',')) {
        button.push_back(std::stoi(num_str));
    }
    
    return button;
}

// direct parsing approach
// uint64_t parse_button(const std::string& token) {
//     uint64_t mask = 0;
//     int num = 0;
//     bool in_num = false;
    
//     for (char c : token) {
//         if (c >= '0' && c <= '9') {
//             num = num * 10 + (c - '0');
//             in_num = true;
//         } else if (in_num) {
//             mask |= (1ULL << num);
//             num = 0;
//             in_num = false;
//         }
//     }
    
//     return mask;
// }

std::vector<int> parse_joltage(const std::string& token) {
    // token is like "{3,5,4,7}"
    // return vector {3, 5, 4, 7}
    std::vector<int> target;
    
    // Extract content between parentheses
    std::string content = token.substr(1, token.size() - 2);
    
    // Split by comma and process each number
    std::istringstream iss(content);
    std::string num_str;
    
    while (std::getline(iss, num_str, ',')) {
        int k = std::stoi(num_str);
        target.push_back(k);
    }

    for (int i = 0; i < 5; i++){

    }
    
    return target;
}

int solve_part2(const std::vector<int>& target, const std::vector<std::vector<int>>& buttons) {

    int m = buttons.size();

    // Compute per-button bound
    std::vector<int> max_press(m);
    uint64_t total_combinations = 1;
    for (int j = 0; j < m; j++) {
        int bound = INT_MAX;
        for (int idx : buttons[j]) {
            bound = std::min(bound, target[idx]);
        }
        max_press[j] = bound;
        total_combinations *= (bound +1);
    }

    std::cerr << "Buttons: " << m 
            << ", Total combinations: " << total_combinations << std::endl;


    // int K = *std::max_element(target.begin(), target.end());
    int best = INT_MAX;
    std::vector<int> presses(m, 0);  // how many times to press each button
    uint64_t iteration = 0;
    
    while (true) {
        iteration++;

        // Print every 10 million iterations
        if (iteration % 10000000 == 0) {
            std::cerr << "Progress: " << iteration << " / " << total_combinations << std::endl;
        }

        // === 1. Evaluate current combination ===
        // Build state by applying presses[j] times button j
        // Check if state == target
        // If yes, update best with sum of presses

        auto press_cnt = std::accumulate(presses.begin(), presses.end(), 0);
        if (press_cnt < best) {
            // only care if it's potentially less number of presses

            std::vector<int> curr_vec(target.size(), 0);
            for (int i = 0; i < m; ++i) {
                for (auto idx: buttons[i]) {
                    curr_vec[idx] += presses[i]; // press the positions of each button buttons[i] times
                }
            }
            if (curr_vec == target) {
                best = press_cnt;
            }
        }
        
        // === 2. Increment odometer ===
        int i = 0;
        while (i < m) {
            presses[i]++;
            // if (presses[i] <= K) break;  // no carry
            if (presses[i] <= max_press[i]) break;  // no carry
            presses[i] = 0;              // reset and carry
            i++;
        }
        if (i == m) break;  // done: all combinations exhausted
    }
    
    return best;
}

// int solve(uint64_t target, const std::vector<uint64_t>& buttons) {
//     int m = buttons.size();
//     int best = INT_MAX;  // need <climits> or use a large number
    
//     // Enumerate all subsets: 0 to 2^m - 1
//     // a mask represents a combination of buttons
//     for (uint64_t mask = 0; mask < (1ULL << m); mask++) {
//         uint64_t state = 0;
//         int presses = 0;
        
//         // Check each button
//         for (int j = 0; j < m; j++) {
//             if (mask & (1ULL << j)) {
//                 // Button j is "selected" in this subset
//                 state ^= buttons[j];
//                 presses++;
//             }
//         }
        
//         if (state == target) {
//             best = std::min(best, presses);
//         }
//     }
    
//     return best;
// }

int main(int argc, char* argv[]) {

    std::ifstream file(argv[1]);
    std::string line;

    long long total = 0;  // accumulator


    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string token;

        std::vector<int> target;
        std::vector<std::vector<int>> buttons;

        while (ss >> token) {
            // std::cout << "Token: " << token << std::endl;

            if (token[0] == '{') {
                // process target immediately
                target = parse_joltage(token);
            } else if (token[0] == '(') {
                // process button immediately
                buttons.push_back(parse_button_part2(token));
            }
            // ignore '[' tokens

        }

        // Solve this machine and add to total
        total += solve_part2(target, buttons);
    }

    std::cout << total << std::endl;

    return 0;
}