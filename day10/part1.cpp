#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <climits>

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

uint64_t parse_button(const std::string& token) {
    // token is like "(1,3)" or "(0,2,3,4)"
    uint64_t mask = 0;
    
    // Extract content between parentheses
    std::string content = token.substr(1, token.size() - 2);
    
    // Split by comma and process each number
    std::istringstream iss(content);
    std::string num_str;
    
    while (std::getline(iss, num_str, ',')) {
        int k = std::stoi(num_str);
        // TODO: set bit k
        mask |= (1ULL << k);
    }
    
    return mask;
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

int solve(uint64_t target, const std::vector<uint64_t>& buttons) {
    int m = buttons.size();
    int best = INT_MAX;  // need <climits> or use a large number
    
    // Enumerate all subsets: 0 to 2^m - 1
    for (uint64_t mask = 0; mask < (1ULL << m); mask++) {
        uint64_t state = 0;
        int presses = 0;
        
        // Check each button
        for (int j = 0; j < m; j++) {
            if (mask & (1ULL << j)) {
                // Button j is "selected" in this subset
                state ^= buttons[j];
                presses++;
            }
        }
        
        if (state == target) {
            best = std::min(best, presses);
        }
    }
    
    return best;
}

int main(int argc, char* argv[]) {

    std::ifstream file(argv[1]);
    std::string line;

    long long total = 0;  // accumulator


    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string token;

        uint64_t target;
        std::vector<uint64_t> buttons;

        while (ss >> token) {
            // std::cout << "Token: " << token << std::endl;

            if (token[0] == '[') {
                // process target immediately
                target = parse_target(token);
            } else if (token[0] == '(') {
                // process button immediately
                buttons.push_back(parse_button(token));
            }
            // ignore '{' tokens

        }

        // Solve this machine and add to total
        total += solve(target, buttons);

    }

    std::cout << total << std::endl;

    return 0;
}