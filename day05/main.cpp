#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;

    std::vector<std::pair<long long, long long>> ranges;
    std::vector<long long> values;
    
    while (std::getline(file, line)) {
        if (line.empty() || line == "\r") break;  // break line = switch sections


        // Parsing
        long long a, b;
        char dash;
        std::stringstream ss(line);
        ss >> a >> dash >> b;
        ranges.push_back({a, b});
    }

    // Read single values
    while (std::getline(file, line)) {
        // values.push_back(std::stoi(line));
        values.push_back(std::stoll(line));
    }

    // Sort both
    std::sort(ranges.begin(), ranges.end());  // by first element (default)
    std::sort(values.begin(), values.end());

    // Two pointers: count values that fall within ANY range
    int count = 0;
    size_t r = 0;  // range pointer

    // Merge overlapping ranges after sorting
    // std::vector<std::pair<long long,long long>> merged;
    // for (auto& [lo, hi] : ranges) {
    //     if (merged.empty() || merged.back().second < lo) {
    //         merged.push_back({lo, hi});
    //     } else {
    //         merged.back().second = std::max(merged.back().second, hi);
    //     }
    // }


    for (long long v : values) {
        // Advance range pointer while current range ends before v
        while (r < ranges.size() && ranges[r].second < v) {
            r++;
        }
    
        // Check if v falls within current range [lo, hi]
        if (r < ranges.size() && ranges[r].first <= v && v <= ranges[r].second) {
            count++;
        }
    }

    std::cout << "count is: " << count << std::endl;
    return 0;

}