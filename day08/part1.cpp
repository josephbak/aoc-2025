#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <set>

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

int main(int argc, char** argv) {

    std::ifstream file(argv[1]);
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

    // for (const auto& p : pts) {
    //     std::cout << p.x << " " << p.y << " " << p.z << "\n";
    // }

    int n = pts.size();
    int k = 1000;

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

    // 3. Merge K closest pairs (skip if already connected)
    UnionFind uf(n);
    // consider k shortest edges
    for (int i = 0; i < k; i++) {
        const auto& e = edges[i];
        if (uf.find(e.i) != uf.find(e.j)) {
            uf.unite(e.i, e.j);  // might not always merge
        }
    }

    std::set<int> roots;
    for (int i = 0; i < n; i++)
        roots.insert(uf.find(i));

    
    // for (int x : roots) std::cout << x << " "; std::cout << "\n";

    // std::cout << "Roots: ";
    // for (int r : roots) {
    //     std::cout << r << "(size=" << uf.size_[r] << ") ";
    // }
    // std::cout << "\n";
    

    std::vector<int> sizes;
    for (int r : roots)
        // sizes.push_back(uf.size_[r]);
        sizes.push_back(uf.getSize(r));

    
    std::sort(sizes.rbegin(), sizes.rend());

    std::cout << "Sizes (descending): ";
    for (int s : sizes)
        std::cout << s << " ";
    std::cout << "\n";

    std::cout << "Top 3 product: " << sizes[0] * sizes[1] * sizes[2] << "\n";

//     for (int i = 0; i < k; i++) {
//     const auto& e = edges[i];
//     bool merged = uf.find(e.i) != uf.find(e.j);
//     std::cout << "Edge " << i << ": " << e.i << "-" << e.j 
//          << " dist=" << e.dist 
//          << (merged ? " MERGED" : " SKIP (same chunk)") << "\n";
//     if (merged) uf.unite(e.i, e.j);
// }

    return 0;
}