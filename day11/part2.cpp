#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

std::unordered_map<std::string, std::vector<std::string>> adj;
std::unordered_set<std::string> required = {"fft", "dac"};

long long countPaths(const std::string& curr,
                     const std::string& end,
                     std::unordered_set<std::string>& visited) {
    // Base case: reached destination
    if (curr == end) {
        // Check ALL required nodes were visited
        for (auto& r : required) {
            if (!visited.count(r)) return 0;  // missing required node
        }
        return 1;  // valid path!
    }
    
    // Mark current as visited
    visited.insert(curr);
    
    // Explore all neighbors
    long long paths = 0;
    for (auto& next : adj[curr]) {
        if (!visited.count(next)) {  // no revisits (handles cycles)
            paths += countPaths(next, end, visited);
        }
    }
    
    // Backtrack: unmark current
    visited.erase(curr);
    
    return paths;
}

// Quick cycle detection
std::unordered_set<std::string> visiting, visited;

bool hasCycle(const std::string& node) {
    if (visiting.count(node)) return true;  // back edge = cycle
    if (visited.count(node)) return false;
    
    visiting.insert(node);
    for (auto& next : adj[node]) {
        if (hasCycle(next)) return true;
    }
    visiting.erase(node);
    visited.insert(node);
    return false;
}

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string node, neighbor;
        
        ss >> node;
        node.pop_back();
        
        while (ss >> neighbor) {
            adj[node].push_back(neighbor);
        }
    }
    
    // std::unordered_set<std::string> visited;
    // long long ans = countPaths("svr", "out", visited);
    // std::cout << ans << "\n";

    // Quick cycle detection
    std::unordered_set<std::string> visiting, visited;

    // In main:
    if (hasCycle("svr")) {
        std::cout << "Graph has cycles!" << std::endl;
    } else {
        std::cout << "Graph has no cycles!" << std::endl;
    }
}