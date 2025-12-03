#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
    long long ans{};

    std::ifstream file(argv[1]);
    std::string line;

    std::getline(file, line);

    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ',')) {
        size_t dash = token.find('-');
        long long start = std::stoll(token.substr(0, dash));
        long long end = std::stoll(token.substr(dash + 1));

        // process start, end
        std::cout << "Start: " << start << " End: " << end << std::endl;
        for (long long num = start; num <= end; ++num) {
            std::string s = std::to_string(num);
            bool flag = true;
            if (s.size() % 2 == 0){
                for (int i = 0; i < s.size()/2; ++i){
                    if (s[i] != s[i + s.size()/2]){
                        flag = false;
                        break;
                    }
                }
                if (flag){
                    ans += num;
                }
            }
        }
    }


    std::cout << ans << std::endl;
    return 0;
}