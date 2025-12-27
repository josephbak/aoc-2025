#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>


int main(int argc, char** argv) {

    std::vector<std::pair<long long, long long>> vec;
    std::ifstream file(argv[1]);
    std::string line;

    long long ans = 0;

    while (std::getline(file, line)) {
        long long a, b;
        char comma;
        std::stringstream ss(line);
        ss >> a >> comma >> b;
        vec.push_back({a, b});
    }


    for (size_t i = 0; i < vec.size(); i++) {
        // std::cout << vec[i].first << ", " << vec[i].second << std::endl;
        for (size_t j = i + 1; j < vec.size(); j++) {
            // vec[i] and vec[j] are always distinct
            // each pair visited exactly once
            long long area = (std::abs(vec[i].first - vec[j].first) + 1) * (std::abs(vec[i].second - vec[j].second) + 1);
            ans = (ans > area) ? ans : area;
        }
    }
    
    std::cout << vec.size() << std::endl;
    std::cout << "count is: " << ans << std::endl;

    return 0;

}