#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>


std::unordered_map<std::string, std::vector<std::string>> adj;
std::unordered_map<std::string, long long> memo;

long long countPaths(const std::string& curr, const std::string& end) {
    if (curr == end) return 1;
    if (memo.count(curr)) return memo[curr];
    
    long long paths = 0;
    for (auto& next : adj[curr]) {
        paths += countPaths(next, end);
    }
    return memo[curr] = paths;
}


int main(int argc, char** argv) {

    // std::unordered_map<std::string, std::vector<std::string>> adj;
    // std::unordered_map<std::string, long long> memo;

    std::ifstream file(argv[1]);
    std::string line;

    // long long ans = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string node, neighbor;
        
        // Read "string1:"
        ss >> node;
        node.pop_back();  // remove trailing ':'
        
        // Read remaining neighbors
        while (ss >> neighbor) {
            adj[node].push_back(neighbor);
        }
    }

    // printing
    // for (auto& [node, neighbors] : adj) {
    //     std::cout << node << " -> ";
    //     for (auto& n : neighbors) {
    //         std::cout << n << " ";
    //     }
    //     std::cout << "\n";
    // }

    long long ans = countPaths("you", "out");


    std::cout << '\n';
    std::cout << "count is: " << ans << std::endl;

    return 0;

}