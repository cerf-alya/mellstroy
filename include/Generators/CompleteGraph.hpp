#pragma once
#include "GraphCore/Graph.hpp"

class CompleteGraphGenerator {
public:
    static Graph generate(int n) {
        Graph g;
        for (int i = 0; i < n; i++) {
            g.addVertex();
        }
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                g.addEdge(i, j);
            }
        }
        return g;
    }
};