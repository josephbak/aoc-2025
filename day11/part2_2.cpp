#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>

std::unordered_map<std::string, std::vector<std::string>> adj;
std::map<std::pair<std::string, std::string>, long long> memo;

long long countPaths(const std::string& curr, const std::string& end) {
    // base case: reached destination
    if (curr == end) return 1;

    // if it's already computed
    if (memo.count({curr, end})) return memo[{curr, end}];
    
    // sum paths through all neighbors
    long long paths = 0;
    for (auto& next : adj[curr]) {
        paths += countPaths(next, end);
    }

    // cache and return
    return memo[{curr, end}] = paths;
}

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string node, neighbor;
        
        ss >> node;
        node.pop_back();  // remove ':'
        
        while (ss >> neighbor) {
            adj[node].push_back(neighbor);
        }
    }
    
    // Debug: print adj
    // for (auto& [n, neighbors] : adj) {
    //     std::cout << n << " -> ";
    //     for (auto& nb : neighbors) std::cout << nb << " ";
    //     std::cout << "\n";
    // }
    
    std::vector<std::string> required = {"fft", "dac"};
    std::sort(required.begin(), required.end());

    long long total = 0;
    do {
        long long paths = countPaths("svr", required[0]);
        for (size_t i = 0; i + 1 < required.size(); i++) {
            paths *= countPaths(required[i], required[i + 1]);
        }
        paths *= countPaths(required.back(), "out");
        
        total += paths;
    } while (std::next_permutation(required.begin(), required.end()));

    std::cout << "Total: " << total << "\n";
}