#pragma once
#include "GraphCore/Graph.hpp"
#include <random>
#include <algorithm>

struct BasicGenerators {
    // 1. полный граф Kn
    static Graph complete(int n) {
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                g.addEdge(i, j);
        return g;
    }

    // 2. полный двудольный Kn,m
    static Graph completeBipartite(int n, int m) {
        Graph g;
        for (int i = 0; i < n + m; ++i) g.addVertex();
        for (int i = 0; i < n; ++i)
            for (int j = n; j < n + m; ++j)
                g.addEdge(i, j);
        return g;
    }

    // 3. дерево (случайное)
    static Graph tree(int n) {
        if (n < 1) throw std::invalid_argument("Tree needs at least 1 vertex");
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        for (int i = 1; i < n; ++i) {
            std::uniform_int_distribution<> dis(0, i - 1);
            g.addEdge(i, dis(gen));
        }
        return g;
    }

    // 4. звезда Sn
    static Graph star(int n) {
        if (n < 2) throw std::invalid_argument("Star needs at least 2 vertices");
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        for (int i = 1; i < n; ++i)
            g.addEdge(0, i);
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

    // 6. путь Pn
    static Graph path(int n) {
        if (n < 1) throw std::invalid_argument("Path needs at least 1 vertex");
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        for (int i = 0; i < n - 1; ++i)
            g.addEdge(i, i + 1);
        return g;
    }

    // 7. колесо Wn
    static Graph wheel(int n) {
        if (n < 4) throw std::invalid_argument("Wheel needs at least 4 vertices");
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        // цикл из n-1 вершин
        for (int i = 1; i < n - 1; ++i)
            g.addEdge(i, i + 1);
        g.addEdge(1, n - 1);
        // центр соединен со всеми
        for (int i = 1; i < n; ++i)
            g.addEdge(0, i);
        return g;
    }

    // 8. случайный G(n, p)
    static Graph randomGnp(int n, double p) {
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (dis(gen) < p)
                    g.addEdge(i, j);
        
        return g;
    }
};