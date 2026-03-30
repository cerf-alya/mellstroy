#pragma once
#include "GraphCore/Graph.hpp"
#include <random>
#include <algorithm>
#include <vector>
#include <stdexcept>

struct AdvancedGenerators {
    
    // 9. кубический граф 
    static Graph cubic(int n) {
        if (n < 4 || n % 2 != 0) {
            throw std::invalid_argument("Cubic graph requires even n >= 4");
        }
        
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // создаём цикл для основы
        for (int i = 0; i < n; ++i) {
            g.addEdge(i, (i + 1) % n);
        }
        
        // добавляем рёбра до степени 3
        std::vector<int> vertices(n);
        for (int i = 0; i < n; ++i) vertices[i] = i;
        
        for (int i = 0; i < n && g.degree(i) < 3; ++i) {
            for (int j = i + 2; j < n && g.degree(i) < 3; ++j) {
                if (g.degree(j) < 3 && !g.hasEdge(i, j) && 
                    (i == 0 || j != (i + 1) % n)) {
                    g.addEdge(i, j);
                }
            }
        }
        
        return g;
    }
    
    // 10. граф с заданным количеством компонент связности
    static Graph withComponents(int n, int k) {
        if (k < 1 || k > n) {
            throw std::invalid_argument("Invalid number of components");
        }
        
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        // распределяем вершины по компонентам
        int baseSize = n / k;
        int remainder = n % k;
        
        int start = 0;
        for (int comp = 0; comp < k; ++comp) {
            int size = baseSize + (comp < remainder ? 1 : 0);
            
            // создаём связную компоненту 
            for (int i = 1; i < size; ++i) {
                g.addEdge(start + i - 1, start + i);
            }
            
            start += size;
        }
        
        return g;
    }
    
    // 11. граф с заданным количеством мостов
    static Graph withBridges(int n, int numBridges) {
        if (numBridges < 0 || numBridges >= n) {
            throw std::invalid_argument("Invalid number of bridges");
        }
        
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        // создаём цепочку мостов
        for (int i = 0; i < numBridges && i < n - 1; ++i) {
            g.addEdge(i, i + 1);
        }
        
        // добавляем случайные рёбра (не мосты) в концах
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(numBridges, n - 1);
        
        for (int i = 0; i < 5; ++i) {
            int u = dis(gen);
            int v = dis(gen);
            if (u != v && !g.hasEdge(u, v)) {
                g.addEdge(u, v);
            }
        }
        
        return g;
    }
    
    // 12. граф с заданным количеством точек сочленения
    static Graph withArticulationPoints(int n, int numAP) {
        if (numAP < 0 || numAP >= n) {
            throw std::invalid_argument("Invalid number of articulation points");
        }
        
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        // создаём структуру "звезда" - центр будет точкой сочленения
        int center = 0;
        int leaves = std::min(numAP, n - 1);
        
        for (int i = 1; i <= leaves; ++i) {
            g.addEdge(center, i);
        }
        
        // добавляем дополнительные рёбра для оставшихся вершин
        for (int i = leaves + 1; i < n; ++i) {
            g.addEdge(center, i);
        }
        
        return g;
    }
    
    // 13. граф с заданным количеством 2-мостов 
    static Graph with2Bridges(int n, int num2Bridges) { 
        if (num2Bridges < 0 || n < 4) {
            throw std::invalid_argument("Invalid parameters for 2-bridges");
        }
        
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        // создаём несколько циклов, соединённых парами рёбер
        int blockSize = std::max(3, n / (num2Bridges + 1));
        
        for (int block = 0; block <= num2Bridges; ++block) {
            int start = block * blockSize;
            int end = std::min(start + blockSize, n);
            
            // цикл внутри блока
            for (int i = start; i < end - 1; ++i) {
                g.addEdge(i, i + 1);
            }
            if (end > start + 2) {
                g.addEdge(start, end - 1);
            }
            
            // соединяем блоки парой рёбер 
            if (block < num2Bridges && end < n) {
                g.addEdge(end - 1, end);
                if (end + 1 < n) {
                    g.addEdge(start, end + 1);
                }
            }
        }
        
        return g;
    }
    
    // 14. граф Халина 
    static Graph halin(int n) {
        if (n < 4) {
            throw std::invalid_argument("Halin graph requires n >= 4");
        }
        
        Graph g;
        for (int i = 0; i < n; ++i) g.addVertex();
        
        // создаём дерево с корнем 0
        for (int i = 1; i < n; ++i) {
            g.addEdge((i - 1) / 2, i);
        }
        
        // добавляем цикл по листьям
        std::vector<int> leaves;
        for (int i = 0; i < n; ++i) {
            if (g.degree(i) == 1) {
                leaves.push_back(i);
            }
        }
        
        for (size_t i = 0; i < leaves.size(); ++i) {
            g.addEdge(leaves[i], leaves[(i + 1) % leaves.size()]);
        }
        
        return g;
    }
};