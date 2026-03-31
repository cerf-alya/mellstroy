#pragma once
#include "GraphCore/Graph.hpp"
#include <fstream>
#include <sstream>
#include <string>

class DIMACSParser {
public:
    static Graph parse(const std::string& filename) {
        Graph g;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string line;
        int n = 0, m = 0;
        std::vector<std::pair<int, int>> edges;
        
        while (std::getline(file, line)) {
            // пропускаем пустые строки
            if (line.empty()) continue;
            
            if (line[0] == 'c') continue;
            
            if (line[0] == 'p') {
                std::istringstream iss(line);
                std::string p, edge;
                iss >> p >> edge >> n >> m;
                
                // добавляем вершины 
                for (int i = 0; i < n; ++i) {
                    g.addVertex();
                }
                continue;
            }
            
            // рёбра: e u v
            if (line[0] == 'e') {
                std::istringstream iss(line);
                std::string e;
                int u, v;
                iss >> e >> u >> v;

                edges.push_back({u - 1, v - 1});
            }
        }
        
        // добавляем все рёбра
        for (const auto& edge : edges) {
            g.addEdge(edge.first, edge.second);
        }
        
        return g;
    }
    
    static void write(const Graph& g, const std::string& filename) {
        std::ofstream file(filename);
        int n = g.verticesCount();
        int m = g.edgesCount();
        
        // заголовок
        file << "c DIMACS graph file\n";
        file << "p edge " << n << " " << m << "\n";
        
        // рёбра 
        for (int u = 0; u < n; ++u) {
            for (int v : g.getNeighbors(u)) {
                if (u < v) { 
                    file << "e " << (u + 1) << " " << (v + 1) << "\n";
                }
            }
        }
    }
};