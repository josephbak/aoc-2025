#include <fstream>
#include <string>
#include <iostream>
#include <string_view>
#include <cstdint>

int64_t select_k_digits(std::string_view sv, int k) {
    int n = sv.size();
    int64_t result = 0;
    
    int start = 0;
    for (int i = 0; i < k; ++i) {
        int end = n - (k - i);
        
        int best_pos = start;
        for (int j = start; j <= end; ++j) {
            if (sv[j] > sv[best_pos]) {
                best_pos = j;
            }
        }
        
        result = result * 10 + (sv[best_pos] - '0');
        start = best_pos + 1;
    }
    
    return result;
}

int64_t select_k_monotonic(std::string_view sv, int k) {
    int n = sv.size();
    int to_remove = n - k;
    std::string stack;
    
    for (char c : sv) {
        // While we can still remove AND stack top is smaller than current
        while (to_remove > 0 && !stack.empty() && stack.back() < c) {
            stack.pop_back();
            --to_remove;
        }
        stack += c;
    }
    
    stack.resize(k);  // trim if we didn't use all removals
    
    int64_t result = 0;
    for (char c : stack) result = result * 10 + (c - '0');
    return result;
}

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    int64_t ans = 0;

    while (std::getline(file, line)) {
        ans += select_k_digits(line, 12);
    }

    std::cout << ans << std::endl;
    return 0;
}