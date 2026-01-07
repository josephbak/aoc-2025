#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>

struct Point { int x, y, z; };

auto dist = [](const Point& a, const Point& b) -> long long {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    long long dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
};

// 1. Build all pairs with distances
struct Edge {
    int i, j;
    long long dist;
};

class UnionFind {
public:
    std::vector<int> parent, rank_, size_;
    
    UnionFind(int n) : parent(n), rank_(n, 0), size_(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }
    
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        
        // union by rank (optimization)
        if (rank_[px] < rank_[py]) std::swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        
        // track size (your requirement)
        size_[px] += size_[py];
    }
    
    int getSize(int x) { return size_[find(x)]; }
};

int main(int argc, char* argv[]) {

    std::string filename = (argc > 1) ? argv[1] : "input.txt";
    int k = (argc > 2) ? std::stoi(argv[2]) : 1000;
    int top_n = (argc > 3) ? std::stoi(argv[3]) : 3;

    std::ifstream file(filename);
    std::string line;

    std::vector<Point> pts;
    
    while (getline(file, line)) {
        int a, b, c;
        char comma1, comma2;
        std::istringstream iss(line);
        iss >> a >> comma1 >> b >> comma2 >> c;
        // data.emplace_back(a, b, c);

        pts.push_back({a, b, c});
    }

    int n = pts.size();

    std::vector<Edge> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            edges.push_back({i, j, dist(pts[i], pts[j])});
        }
    }

    // 2. Sort by distance
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.dist < b.dist;
    });

    // 3. Merge until fully connedted
    UnionFind uf(n);

    long long ans = 0;
    int merges = 0;
    for (int i = 0; i < edges.size(); i++) {
        const auto& e = edges[i];
        if (uf.find(e.i) != uf.find(e.j)) {
            uf.unite(e.i, e.j);  // might not always merge
            if (++merges == n - 1) {
                ans = (long long) pts[e.i].x * pts[e.j].x;
                break;  // n nodes need n-1 edges
            }
        }
    }

    std::cout << "The product of x coords of last two junction boxes: " << ans << std::endl;

    return 0;
}