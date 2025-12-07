#include <fstream>
#include <string>
#include <iostream>
#include <string_view>

int highest_joltage(std::string_view sv) {
    int sv_len = sv.size();
    int first = sv[0] - '0';
    int second = sv[1] - '0';

    for (int i = 1; i < sv_len; ++i) {
        if (sv[i] - '0' > first && i < sv_len - 1) {
            // update the first
            first = sv[i] - '0';
            second = sv[i + 1] - '0';
        } else if (sv[i] - '0' > second) {
            second = sv[i] - '0';
        }
    }
    // std::cout << "First: " << first << ", Second: " << second << std::endl;
    return first * 10 + second;
};

int high_jolt(std::string_view sv) {
    int sv_len = sv.size();
    int highest = 0;
    for (int i = 0; i < sv_len - 1; ++i) {
        for (int j = i+1; j < sv_len; ++j) {
            if ((sv[i] - '0')*10 + (sv[j] - '0') > highest) {
                highest = (sv[i] - '0')*10 + (sv[j] - '0');
            }
        }
    }
    return highest;
};

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    int ans = 0;

    while (std::getline(file, line)) {
        int tmp = highest_joltage(line);
        // int tmp2 = high_jolt(line);

        // if (tmp != tmp2) {
            // std::cerr << "Mismatch: \"" << line << "\" → " << tmp << " vs " << tmp2 << std::endl;
        // }
        // std::cout << tmp << std::endl;
        // std::cout << tmp2 << std::endl;
        ans += tmp;
        // ans += highest_joltage(line);
    }

    std::cout << ans << std::endl;
    return 0;
}