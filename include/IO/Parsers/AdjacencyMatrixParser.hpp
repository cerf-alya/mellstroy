#pragma once
#include "GraphCore/Graph.hpp"
#include <fstream>
#include <sstream>

class AdjacencyMatrixParser {
public:
    static Graph parse(const std::string& filename) {
        Graph g;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        int n;
        file >> n;
        
        for (int i = 0; i < n; ++i) {
            g.addVertex();
        }
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int val;
                file >> val;
                if (val != 0 && i < j) { 
                    g.addEdge(i, j);
                }
            }
        }
        
        return g;
    }
    
    static void write(const Graph& g, const std::string& filename) {
        std::ofstream file(filename);
        int n = g.verticesCount();
        file << n << "\n";
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                file << (g.hasEdge(i, j) ? 1 : 0);
                if (j < n - 1) file << " ";
            }
            file << "\n";
        }
    }
};