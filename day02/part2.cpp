#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>

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

    std::set<long long> invalid;
    int maxDigits = std::to_string(maxVal).length();


    // OUTER: pattern length
    for (int k = 1; k <= maxDigits / 2; ++k) {
        // MIDDLE: all k-digit patterns
        long long lo = (k == 1) ? 1 : (long long)pow(10, k - 1);
        long long hi = (long long)pow(10, k) - 1;
        
        for (long long pat = lo; pat <= hi; ++pat) {
            std::string ps = std::to_string(pat);
            
            // INNER: repeat r times (r >= 2)
            for (int r = 2; k * r <= maxDigits; ++r) {
                std::string rep;
                for (int i = 0; i < r; ++i) rep += ps;
                
                long long num = std::stoll(rep);
                if (num > maxVal) break;
                invalid.insert(num);
            }
        }
    }

    // Sort ranges by start, look for overlaps
    // std::sort(ranges.begin(), ranges.end());
    // for (int i = 1; i < ranges.size(); ++i) {
    //     if (ranges[i].first <= ranges[i-1].second) {
    //         std::cout << "Overlap detected!\n";
    //     }
    // }

    long long ans = 0;
    for (long long d : invalid) {
        for (auto& [lo, hi] : ranges) {
            if (d >= lo && d <= hi) {
                ans += d;
                break;  // found in one range, move on
            }
        }
    }

    std::cout << "the anser: " << ans << std::endl;

    return 0;
}