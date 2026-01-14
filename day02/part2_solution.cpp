#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>

long long pow10(int n) {
    long long r = 1;
    for (int i = 0; i < n; ++i) r *= 10;
    return r;
}

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    std::getline(file, line);

    // Parse ranges, track max value
    std::vector<std::pair<long long, long long>> ranges;
    std::stringstream ss(line);
    std::string token;
    long long maxVal = 0;

    while (std::getline(ss, token, ',')) {
        size_t dash = token.find('-');
        long long L = std::stoll(token.substr(0, dash));
        long long R = std::stoll(token.substr(dash + 1));
        ranges.push_back({L, R});
        maxVal = std::max(maxVal, R);
    }

    // Generate all invalid numbers (h repeated n times, n >= 2)
    std::set<long long> invalid;

    for (int k = 1; pow10(k) <= maxVal; ++k) {
        long long minH = (k == 1) ? 1 : pow10(k - 1);
        long long maxH = pow10(k) - 1;
        long long mult = pow10(k);

        for (long long h = minH; h <= maxH; ++h) {
            long long num = h;
            for (int n = 2; ; ++n) {
                num = num * mult + h;  // append h
                if (num > maxVal) break;
                invalid.insert(num);   // set deduplicates
            }
        }
    }

    // Sum invalid IDs in each range
    long long ans = 0;
    for (auto& [L, R] : ranges) {
        for (auto it = invalid.lower_bound(L); it != invalid.upper_bound(R); ++it) {
            ans += *it;
        }
    }

    std::cout << ans << std::endl;
    return 0;
}