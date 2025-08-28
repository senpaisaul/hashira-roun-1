#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct Point {
    long long x;
    long long y;
};

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: ./find_c input.json\n";
            return 1;
        }
        std::ifstream fin(argv[1]);
        if (!fin) {
            std::cerr << "Failed to open file\n";
            return 1;
        }
        json j;
        fin >> j;

        
        if (!j.contains("keys") || !j["keys"].contains("k")) {
            std::cerr << "Missing keys.k\n";
            return 1;
        }
        int k = j["keys"]["k"].get<int>();
        if (k < 3) {
            std::cerr << "Need at least 3 roots for quadratic\n";
            return 1;
        }

        // Gather points
        std::vector<Point> pts;
        for (auto it = j.begin(); it != j.end(); ++it) {
            const std::string key = it.key();
            if (key == "keys") continue;
            // key must be integer x
            char* endp = nullptr;
            long xParsed = std::strtol(key.c_str(), &endp, 10);
            if (endp == key.c_str() || *endp != '\0') continue; // non-integer key
            const json& node = it.value();
            if (!node.contains("base") || !node.contains("value")) continue;
            std::string baseStr = node["base"].get<std::string>();
            std::string valStr  = node["value"].get<std::string>();
            // parse base and value
            int base = std::stoi(baseStr);
            if (base < 2 || base > 36) continue;
            long long y = std::stoll(valStr, nullptr, base);
            pts.push_back(Point{static_cast<long long>(xParsed), y});
        }

        if (pts.size() < 3) {
            std::cerr << "Fewer than 3 valid decoded points\n";
            return 1;
        }

        
        std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b){
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        });
        std::vector<Point> uniq;
        for (const auto& p : pts) {
            if (uniq.empty() || uniq.back().x != p.x) {
                uniq.push_back(p);
            }
            if (uniq.size() == 3) break;
        }
        if (uniq.size() < 3) {
            std::cerr << "Need 3 distinct x values\n";
            return 1;
        }
        auto p0 = uniq, p1 = uniq[1], p2 = uniq[12];

        
        auto term = [](long long xi, long long yi, long long xj, long long xk) -> long double {
            long double num = (-static_cast<long double>(xj)) * (-static_cast<long double>(xk));
            long double den = (static_cast<long double>(xi) - xj) * (static_cast<long double>(xi) - xk);
            return static_cast<long double>(yi) * (num / den);
        };

        long double c_val = 0.0L;
        c_val += term(p0.x, p0.y, p1.x, p2.x);
        c_val += term(p1.x, p1.y, p0.x, p2.x);
        c_val += term(p2.x, p2.y, p0.x, p1.x);

        
        long long c_rounded = std::llround(c_val);
        std::cout << c_rounded << "\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
