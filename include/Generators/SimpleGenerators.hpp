#pragma once
#include "GraphCore/Graph.hpp"

struct Generators {
    // 1. полный граф Kn
    static Graph complete(int n) {
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                g.addEdge(i, j);
        return g;
    }

    // 5. цикл Cn
    static Graph cycle(int n) {
        if (n < 3) throw std::invalid_argument("Cycle needs at least 3 vertices");
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        for (int i = 0; i < n; ++i)
            g.addEdge(i, (i + 1) % n);
        return g;
    }
    
};