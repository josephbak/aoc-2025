#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

int main(int argc, char** argv) {
    std::vector<std::pair<long long, long long>> reds;
    std::ifstream file(argv[1]);
    std::string line;

    while (std::getline(file, line)) {
        long long a, b;
        char comma;
        std::stringstream ss(line);
        ss >> a >> comma >> b;
        reds.push_back({a, b});
    }

    int n = reds.size();

    // Point-in-polygon test (ray casting)
    // Returns: -1 = outside, 0 = on boundary, 1 = inside
    auto point_status = [&](long long px, long long py) -> int {
        // First check if point is on any edge
        for (int i = 0; i < n; ++i) {
            long long x1 = reds[i].first, y1 = reds[i].second;
            long long x2 = reds[(i+1) % n].first, y2 = reds[(i+1) % n].second;
            
            if (x1 == x2) {  // vertical edge
                if (px == x1 && std::min(y1,y2) <= py && py <= std::max(y1,y2)) {
                    return 0;  // on boundary
                }
            } else {  // horizontal edge
                if (py == y1 && std::min(x1,x2) <= px && px <= std::max(x1,x2)) {
                    return 0;  // on boundary
                }
            }
        }
        
        // Ray casting for interior test
        int crossings = 0;
        for (int i = 0; i < n; ++i) {
            long long x1 = reds[i].first, y1 = reds[i].second;
            long long x2 = reds[(i+1) % n].first, y2 = reds[(i+1) % n].second;
            
            // Only vertical edges can be crossed by horizontal ray
            if (x1 == x2) {
                long long edge_x = x1;
                long long min_y = std::min(y1, y2);
                long long max_y = std::max(y1, y2);
                
                // Ray goes right from (px, py)
                // Crosses if edge is to the right and py is strictly between endpoints
                if (edge_x > px && min_y < py && py < max_y) {
                    crossings++;
                }
            }
        }
        return (crossings % 2 == 1) ? 1 : -1;
    };

    // Check if a polygon edge intersects the INTERIOR of a rectangle
    // (not just touches the boundary)
    auto edge_cuts_rect = [&](long long ex1, long long ey1, long long ex2, long long ey2,
                               long long rx1, long long ry1, long long rx2, long long ry2) -> bool {
        // Normalize rectangle corners
        long long rmin_x = std::min(rx1, rx2), rmax_x = std::max(rx1, rx2);
        long long rmin_y = std::min(ry1, ry2), rmax_y = std::max(ry1, ry2);
        
        if (ex1 == ex2) {
            // Vertical edge at x = ex1, from min(ey1,ey2) to max(ey1,ey2)
            long long edge_x = ex1;
            long long emin_y = std::min(ey1, ey2);
            long long emax_y = std::max(ey1, ey2);
            
            // Edge must be strictly inside rectangle's x range
            // and must span into the rectangle's y range
            if (rmin_x < edge_x && edge_x < rmax_x) {
                // Check if edge overlaps rectangle's y range
                if (emin_y < rmax_y && emax_y > rmin_y) {
                    return true;
                }
            }
        } else {
            // Horizontal edge at y = ey1, from min(ex1,ex2) to max(ex1,ex2)
            long long edge_y = ey1;
            long long emin_x = std::min(ex1, ex2);
            long long emax_x = std::max(ex1, ex2);
            
            // Edge must be strictly inside rectangle's y range
            if (rmin_y < edge_y && edge_y < rmax_y) {
                // Check if edge overlaps rectangle's x range
                if (emin_x < rmax_x && emax_x > rmin_x) {
                    return true;
                }
            }
        }
        return false;
    };

    // Check if rectangle is fully valid
    auto is_valid_rect = [&](long long x1, long long y1, long long x2, long long y2) -> bool {
        // Check the other two corners
        long long c3_x = x1, c3_y = y2;
        long long c4_x = x2, c4_y = y1;
        
        if (point_status(c3_x, c3_y) < 0) return false;  // outside
        if (point_status(c4_x, c4_y) < 0) return false;  // outside
        
        // Check if any polygon edge cuts through rectangle interior
        for (int i = 0; i < n; ++i) {
            long long ex1 = reds[i].first, ey1 = reds[i].second;
            long long ex2 = reds[(i+1) % n].first, ey2 = reds[(i+1) % n].second;
            
            if (edge_cuts_rect(ex1, ey1, ex2, ey2, x1, y1, x2, y2)) {
                return false;
            }
        }
        return true;
    };

    long long ans = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            long long x1 = reds[i].first, y1 = reds[i].second;
            long long x2 = reds[j].first, y2 = reds[j].second;
            
            // Skip if same row or column (degenerate rectangle)
            if (x1 == x2 || y1 == y2) continue;
            
            if (is_valid_rect(x1, y1, x2, y2)) {
                long long area = (std::abs(x2 - x1) + 1) * (std::abs(y2 - y1) + 1);
                ans = std::max(ans, area);
            }
        }
    }

    std::cout << ans << std::endl;
    return 0;
}