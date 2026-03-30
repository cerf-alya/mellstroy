#pragma once
#include "GraphCore/Graph.hpp"
#include <fstream>
#include <sstream>

class EdgeListParser {
public:
    static Graph parse(const std::string& filename) {
        Graph g;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        int maxVertex = -1;
        std::vector<std::pair<int, int>> edges;
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            int u, v;
            if (iss >> u >> v) {
                edges.push_back({u, v});
                maxVertex = std::max(maxVertex, std::max(u, v));
            }
        }
        
        // добавляем все вершины
        for (int i = 0; i <= maxVertex; ++i) {
            g.addVertex();
        }
        
        // добавляем ребра
        for (const auto& edge : edges) {
            g.addEdge(edge.first, edge.second);
        }
        
        return g;
    }
    
    static void write(const Graph& g, const std::string& filename) {
        std::ofstream file(filename);
        for (int u = 0; u < g.verticesCount(); ++u) {
            for (int v : g.getNeighbors(u)) {
                if (u < v) { 
                    file << u << " " << v << "\n";
                }
            }
        }
    }
};