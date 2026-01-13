#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <set>

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

    int red_count = reds.size();
    std::set<std::pair<long long, long long>> boundaries;
    for (size_t i = 0; i < red_count; ++i) {
        long long prev_x = reds[i].first;
        long long prev_y = reds[i].second;
        long long curr_x = reds[(i+1)%red_count].first;
        long long curr_y = reds[(i+1)%red_count].second;

        if (prev_x == curr_x) {
            // same x coordinates
            for (long long y = std::min(prev_y, curr_y); y <= std::max(prev_y, curr_y); ++y) {
                boundaries.insert({curr_x, y});
            }
        } else {
            // same y coordinates
            for (long long x = std::min(prev_x, curr_x); x <= std::max(prev_x, curr_x); ++x) {
                boundaries.insert({x, curr_y});
            }
        }
    }



    long long min_x = LLONG_MAX, max_x = LLONG_MIN;
    long long min_y = LLONG_MAX, max_y = LLONG_MIN;

    for (auto& [x, y] : boundaries) {
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }

    // Printing/debugging (y is row, x is column)
    // for (long long y = min_y; y <= max_y; ++y) {
    //     for (long long x = min_x; x <= max_x; ++x) {
    //         if (boundaries.count({x, y})) {
    //             // Distinguish red vs green if you want
    //             bool is_red = std::find(reds.begin(), reds.end(), 
    //                                     std::make_pair(x, y)) != reds.end();
    //             std::cout << (is_red ? '#' : 'X');
    //         } else {
    //             std::cout << '.';
    //         }
    //     }
    //     std::cout << '\n';
    // }

    std::set<std::pair<long long, long long>> exterior;
    std::queue<std::pair<long long, long long>> q;
    exterior.insert({min_x - 1, min_y - 1});  // mark start as visited
    q.push({min_x - 1, min_y - 1}); 

    // helper lambda to check inside the box
    auto in_bounds = [&](long long x, long long y) {
        return (min_x - 1) <= x && x <= (max_x + 1) 
            && (min_y - 1) <= y && y <= (max_y + 1);
    };
    std::vector<std::pair<int,int>> dirs = {{0,1}, {0,-1}, {1,0}, {-1,0}};

    // BFS to get the exterir points
    while (!q.empty()) {
        auto [x, y] = q.front();      // peek front
        q.pop();                    // dequeue from front

        for (auto [ddx, ddy] : dirs) {
            long long nx = x + ddx;
            long long ny = y + ddy;

            if (in_bounds(nx, ny) && !exterior.count({nx, ny}) && !boundaries.count({nx, ny})) {
                exterior.insert({nx, ny}); // mark visited before pushing
                q.push({nx, ny});
            }
        }
    }

    // getting all the valid tiles
    std::set<std::pair<long long, long long>> valid;
    for (long long y = min_y; y <= max_y; ++y) {
        for (long long x = min_x; x <= max_x; ++x) {
            if (!exterior.count({x, y})) {
                valid.insert({x, y});
            }
        }
    }

    // Shift coordinates so min becomes 0
    int W = max_x - min_x + 1;
    int H = max_y - min_y + 1;

    std::vector<std::vector<int>> grid(W, std::vector<int>(H, 0));

    for (auto& [x, y] : valid) {
        grid[x - min_x][y - min_y] = 1;
    }

    std::vector<std::vector<long long>> P(W + 1, std::vector<long long>(H + 1, 0));

    for (int x = 1; x <= W; ++x) {
        for (int y = 1; y <= H; ++y) {
            P[x][y] = grid[x-1][y-1] + P[x-1][y] + P[x][y-1] - P[x-1][y-1];
        }
    }

    // Query using REAL coordinates
    auto rect_count = [&](long long rx1, long long ry1, long long rx2, long long ry2) {
        // Convert to 1-indexed prefix array coords
        int x1 = std::min(rx1, rx2) - min_x + 1;
        int y1 = std::min(ry1, ry2) - min_y + 1;
        int x2 = std::max(rx1, rx2) - min_x + 1;
        int y2 = std::max(ry1, ry2) - min_y + 1;
        
        return P[x2][y2] - P[x1-1][y2] - P[x2][y1-1] + P[x1-1][y1-1];
    };

    long long ans = 0;
    for (size_t i = 0; i < reds.size(); ++i) {
        for (size_t j = i + 1; j < reds.size(); ++j) {
            long long x1 = reds[i].first, y1 = reds[i].second;
            long long x2 = reds[j].first, y2 = reds[j].second;
            
            long long area = (std::abs(x2 - x1) + 1) * (std::abs(y2 - y1) + 1);
            long long valid_count = rect_count(x1, y1, x2, y2);
            
            if (valid_count == area) {
                ans = std::max(ans, area);
            }
        }
    }


    // long long ans = 0;
    // for (size_t i = 0; i < reds.size(); ++i) {
    //     for (size_t j = i + 1; j < reds.size(); ++j) {
    //         long long x1 = reds[i].first, y1 = reds[i].second;
    //         long long x2 = reds[j].first, y2 = reds[j].second;
            
    //         // Check if all tiles in rectangle are valid
    //         bool ok = true;
    //         for (long long x = std::min(x1,x2); x <= std::max(x1,x2) && ok; ++x) {
    //             for (long long y = std::min(y1,y2); y <= std::max(y1,y2) && ok; ++y) {
    //                 if (!valid.count({x, y})) {
    //                     ok = false;
    //                 }
    //             }
    //         }
            
    //         if (ok) {
    //             long long area = (std::abs(x2 - x1) + 1) * (std::abs(y2 - y1) + 1);
    //             ans = std::max(ans, area);
    //         }
    //     }
    // }

    std::cout << ans << std::endl;


    // for (size_t i = 0; i < vec.size(); i++) {
    //     // std::cout << vec[i].first << ", " << vec[i].second << std::endl;
    //     for (size_t j = i + 1; j < vec.size(); j++) {
    //         // vec[i] and vec[j] are always distinct
    //         // each pair visited exactly once
    //         long long area = (std::abs(vec[i].first - vec[j].first) + 1) * (std::abs(vec[i].second - vec[j].second) + 1);
    //         ans = (ans > area) ? ans : area;
    //     }
    // }
    
    // std::cout << vec.size() << std::endl;
    // std::cout << "count is: " << ans << std::endl;

    return 0;

}