#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <set>
#include <z3++.h>


using Cell = std::pair<int, int>;
using Shape = std::vector<Cell>;

Shape normalize(const Shape& cells) {
    int min_r = INT_MAX, min_c = INT_MAX;
    for (auto& [r, c] : cells) {
        min_r = std::min(min_r, r);
        min_c = std::min(min_c, c);
    }
    
    Shape result;
    for (auto& [r, c] : cells) {
        result.push_back({r - min_r, c - min_c});
    }
    std::sort(result.begin(), result.end());  // for deduplication
    return result;
}

Shape rotate(const Shape& cells) {
    Shape result;
    for (auto& [r, c] : cells) {
        result.push_back({c, -r});
    }
    return result;
}

Shape flip(const Shape& cells) {
    Shape result;
    for (auto& [r, c] : cells) {
        result.push_back({r, -c});
    }
    return result;
}

std::vector<Shape> all_orientations(const Shape& base) {
    std::set<Shape> unique;  // set auto-deduplicates
    
    Shape current = base;
    for (int i = 0; i < 4; i++) {
        unique.insert(normalize(current));
        unique.insert(normalize(flip(current)));
        current = rotate(current);
    }
    
    return std::vector<Shape>(unique.begin(), unique.end());
}

std::vector<Shape> valid_placements(const Shape& orientation, int W, int H) {
    std::vector<Shape> result;
    
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            // Shift orientation to anchor (r, c)
            Shape placed;
            bool valid = true;
            
            for (auto& [dr, dc] : orientation) {
                int nr = r + dr;
                int nc = c + dc;
                
                if (nr < 0 || nr >= H || nc < 0 || nc >= W) {
                    valid = false;
                    break;
                }
                placed.push_back({nr, nc});
            }
            
            if (valid) {
                result.push_back(placed);
            }
        }
    }
    
    return result;
}

bool is_region_line(const std::string& line) {
    return line.find('x') != std::string::npos && 
           line.find(':') != std::string::npos;
}

int main(int argc, char* argv[]) {
    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cerr << "Cannot open file\n";
        return 1;
    }
    
    std::map<int, Shape> shapes;
    std::string line;
    
    int shape_num = 0;
    int row = 0;

    // ===== PARSE SHAPES =====
    // while (std::getline(fin, line)) {
    //     if (line.empty()) continue;
    //     if (is_region_line(line)) break;  // done with shapes
        
    //     int colonIndex = line.find(':');
    //     if (colonIndex != std::string::npos) {
    //         shape_num = std::stoi(line.substr(0, colonIndex));
    //         row = 0; // reset rwo for new shape
    //     } else {
    //         for (int col = 0; col < (int)line.size(); col++) {
    //             if (line[col] == '#') {
    //                 shapes[shape_num].push_back({row, col});
    //             }
    //         }
    //         row++; // move to next row
    //     }
    // }

    while (std::getline(fin, line)) {
    std::cout << "LINE [" << line << "] len=" << line.size() << std::endl;
    
    if (line.empty()) continue;
    if (is_region_line(line)) break;
    
    int colonIndex = line.find(':');
    if (colonIndex != std::string::npos) {
        shape_num = std::stoi(line.substr(0, colonIndex));
        row = 0;
        std::cout << "  NEW SHAPE " << shape_num << std::endl;
    } else {
        std::cout << "  ROW " << row << ": ";
        for (int col = 0; col < (int)line.size(); col++) {
            std::cout << "[" << col << "]=" << line[col] << " ";
            if (line[col] == '#') {
                shapes[shape_num].push_back({row, col});
            }
        }
        std::cout << std::endl;
        row++;
    }
}

    // Calculate cells per shape (do this once after parsing)
    std::map<int, int> shape_size;
    for (auto& [idx, cells] : shapes) {
        shape_size[idx] = cells.size();
    }


    // debug
    // After parsing shapes
    std::cout << "=== SHAPES ===" << std::endl;
    for (auto& [idx, cells] : shapes) {
        std::cout << "Shape " << idx << ": ";
        for (auto& [r, c] : cells) {
            std::cout << "(" << r << "," << c << ") ";
        }
        std::cout << std::endl;
    }

    
    // ===== PARSE REGIONS =====
    // 'line' already holds first region line from above loop
    std::vector<std::tuple<int, int, std::vector<int>>> regions;
    
    do {
        if (line.empty()) continue;
        // Line: "12x5: 1 0 1 0 2 2"

        int xIndex = line.find('x');
        int colonIndex = line.find(':');

        int W = std::stoi(line.substr(0, xIndex));
        int H = std::stoi(line.substr(xIndex + 1, colonIndex - xIndex - 1));

        // For counts: start stringstream from after ':'
        std::stringstream ss(line.substr(colonIndex + 1));

        std::vector<int> counts;
        int c;
        while (ss >> c) {
            counts.push_back(c);
        }

        regions.push_back({W, H, counts});
    } while (std::getline(fin, line));
    
    // ===== FOR EACH REGION, SOLVE =====
    int solvable_count = 0;

   // Precompute outside the region loop (orientations don't depend on W, H)
    std::map<int, std::vector<Shape>> shape_orientations;
    for (auto& [idx, base_shape] : shapes) {
        shape_orientations[idx] = all_orientations(base_shape);
    } 

    // debug
    // After generating orientations
    std::cout << "=== ORIENTATIONS ===" << std::endl;
    for (auto& [idx, orients] : shape_orientations) {
        std::cout << "Shape " << idx << " has " << orients.size() << " orientations" << std::endl;
    }

    for (auto& [W, H, counts] : regions) {

    // ===== AREA CHECK FIRST =====
    int total_cells_needed = 0;
    for (int i = 0; i < (int)counts.size(); i++) {
        total_cells_needed += counts[i] * shape_size[i];
    }

    if (total_cells_needed > W * H) {
        std::cout << "=== REGION " << W << "x" << H << " === SKIP (area)" << std::endl;
        continue;  // skip BEFORE doing any work
    }


    // ===== STEP 3: Generate all valid placements for this region =====
    // placements[shape_idx] = list of cell sets, each representing one way to place that shape
    std::map<int, std::vector<Shape>> placements;
    
    for (auto& [shape_idx, orientations] : shape_orientations) {
        for (auto& orient : orientations) {
            for (auto& placed_cells : valid_placements(orient, W, H)) {
                placements[shape_idx].push_back(placed_cells);
            }
        }
    }

    // debug
    // Inside region loop, after generating placements
    std::cout << "=== REGION " << W << "x" << H << " ===" << std::endl;
    for (auto& [shape_idx, plist] : placements) {
        std::cout << "Shape " << shape_idx << " has " << plist.size() << " placements" << std::endl;
    }


    
    // ===== STEP 4: Set up Z3 solver =====
    z3::context ctx;
    z3::solver solver(ctx);
    
    // Create one boolean variable per placement
    // placement_vars[shape_idx][i] = "is the i-th placement of shape used?"
    std::map<int, std::vector<z3::expr>> placement_vars;
    
    for (auto& [shape_idx, plist] : placements) {
        for (int i = 0; i < (int)plist.size(); i++) {
            std::string name = "p_" + std::to_string(shape_idx) + "_" + std::to_string(i);
            placement_vars[shape_idx].push_back(ctx.bool_const(name.c_str()));
        }
    }
    
    // ===== STEP 5A: Each cell covered at most once =====
    // Map each grid cell to all placement variables that would cover it
    std::map<Cell, std::vector<z3::expr>> cell_to_vars;
    
    for (auto& [shape_idx, plist] : placements) {
        for (int i = 0; i < (int)plist.size(); i++) {
            for (auto& cell : plist[i]) {
                cell_to_vars[cell].push_back(placement_vars[shape_idx][i]);
            }
        }
    }
    
    // Add constraint: at most one placement can cover each cell
    for (auto& [cell, vars] : cell_to_vars) {
        z3::expr_vector vec(ctx);
        for (auto& v : vars) {
            vec.push_back(v);
        }
        solver.add(z3::atmost(vec, 1));
    }
    
    // ===== STEP 5B: Exactly required count of each shape =====
    for (int shape_idx = 0; shape_idx < (int)counts.size(); shape_idx++) {
        int required = counts[shape_idx];
        
        // Handle case where no valid placements exist
        if (placements.find(shape_idx) == placements.end() || placements[shape_idx].empty()) {
            if (required > 0) {
                solver.add(ctx.bool_val(false));  // impossible to satisfy
            }
            continue;
        }
        
        // Build sum of boolean vars (true=1, false=0)
        z3::expr sum = ctx.int_val(0);
        for (auto& v : placement_vars[shape_idx]) {
            sum = sum + z3::ite(v, ctx.int_val(1), ctx.int_val(0));
        }
        
        // Exactly 'required' placements must be true
        solver.add(sum == required);
    }
    
    // ===== STEP 6: Check satisfiability =====
    if (solver.check() == z3::sat) {
        solvable_count++;
    }

    // After solver.check()
    std::cout << "Result: " << (solver.check() == z3::sat ? "SAT" : "UNSAT") << std::endl;
}
    
    std::cout << solvable_count << "\n";
    
    return 0;
}