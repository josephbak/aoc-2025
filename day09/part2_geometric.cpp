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

    auto point_status = [&](long long px, long long py) -> int {
    // return  0: on boundary
        for (int i = 0; i < n; ++i) {
            if (reds[i].first == reds[(i+1)%n].first) {
                // same x coordinates -> vertical edge
                if (px == reds[i].first && std::min(reds[i].second, reds[(i+1)%n].second) <= py && py <= std::max(reds[i].second, reds[(i+1)%n].second)) {
                    return 0;
                }
            } else {
                // same y coordinates
                if (py == reds[i].second && std::min(reds[i].first, reds[(i+1)%n].first) <= px && px <= std::max(reds[i].first, reds[(i+1)%n].first)) {
                    return 0;
                }
            }
        }
    // return -1: outside
    // return  1: inside
        int crossings = 0;
        for (int i = 0; i < n; ++i) {
            if (reds[i].first == reds[(i+1)%n].first) {
                // vectical edge
                if (px < reds[i].first && std::min(reds[i].second, reds[(i+1)%n].second) < py && py < std::max(reds[i].second, reds[(i+1)%n].second)) {
                    crossings++;
                }
            }
        }
        return (crossings % 2 == 1) ? 1 : -1;
    };

    auto edge_cuts_rect = [&](long long ex1, long long ey1, long long ex2, long long ey2,
                          long long rx1, long long ry1, long long rx2, long long ry2) -> bool {
    // edge: (ex1, ey1) to (ex2, ey2)
    // rectangle corners: (rx1, ry1) and (rx2, ry2)
    // return true if edge cuts through rectangle INTERIOR
        long long rmin_x = std::min(rx1, rx2), rmax_x = std::max(rx1, rx2);
        long long rmin_y = std::min(ry1, ry2), rmax_y = std::max(ry1, ry2);

        long long emin_x = std::min(ex1, ex2), emax_x = std::max(ex1, ex2);
        long long emin_y = std::min(ey1, ey2), emax_y = std::max(ey1, ey2);

        if (ex1 == ex2) {
            // vertical edge
            return (rmin_x < ex1 && ex1 < rmax_x) && (emin_y < rmax_y && rmin_y < emax_y);
        }
        // horizontal edge
        return (rmin_y < ey1 && ey1 < rmax_y) && (emin_x < rmax_x && rmin_x < emax_x);
    };

    auto is_valid_rect = [&](long long x1, long long y1, long long x2, long long y2) -> bool {
        // (x1, y1) and (x2, y2) are opposite corners (both red points)

        auto third_pt = point_status(x1, y2);   // corner 3
        auto fourth_pt = point_status(x2, y1);  // corner 4
        if (third_pt < 0 || fourth_pt < 0) {
            // at least one of them is outside of the boundary
            return false;
        }

        for (int i = 0; i < n; ++i) {
            if (edge_cuts_rect(reds[i].first, reds[i].second, 
                            reds[(i+1)%n].first, reds[(i+1)%n].second, 
                            x1, y1, x2, y2)) {
                return false;  // found an edge that cuts through
            }
        }
        return true;  // no edge cuts through
    };

    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (reds[i].first == reds[j].first || reds[i].second == reds[j].second) continue;

            if (is_valid_rect(reds[i].first, reds[i].second, reds[j].first, reds[j].second)) {
                long long area = (std::abs(reds[j].first - reds[i].first) + 1) 
                            * (std::abs(reds[j].second - reds[i].second) + 1);
                ans = std::max(ans, area);
            }
        }
    }

    std::cout << ans << std::endl;
    return 0;
}