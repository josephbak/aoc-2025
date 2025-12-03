#include <fstream>
#include <string>
#include <iostream>


int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    int ans = 0;
    int curr = 50;

    while (std::getline(file, line)) {
        // std::cout << line << std::endl;
        if (line[0] == 'R'){
            curr = (curr + std::stoi(line.substr(1))) % 100;
        }
        else {
            curr = ((curr - std::stoi(line.substr(1))) + 100) % 100;
        }

        // std::cout << curr << std::endl;

        if (curr == 0){
            ans++;
        }
    }

    std::cout << ans << std::endl;

    return 0;
}