#pragma once
#include "GraphCore/Graph.hpp"
#include "Algorithms/Connectivity.hpp"
#include <fstream>
#include <sstream>
#include <random>
#include <vector>
#include <set>

class GraphVizSerializer {
public:
    // базовая сериализация в .dot формат
    static void write(const Graph& g, const std::string& filename) {
        std::ofstream file(filename);
        file << "graph G {\n";
        file << "    rankdir=LR;\n";
        file << "    node [shape=circle, style=filled, fillcolor=lightblue];\n";
        
        for (int u = 0; u < g.verticesCount(); ++u) {
            for (int v : g.getNeighbors(u)) {
                if (u < v) {
                    file << "    " << u << " -- " << v << ";\n";
                }
            }
        }
        
        file << "}\n";
    }

    // с визуализацией компонент связности (кластерами)
    static void writeWithComponents(const Graph& g, const std::string& filename) {
        std::ofstream file(filename);
        file << "graph G {\n";
        file << "    rankdir=LR;\n";
        file << "    compound=true;\n";
        
        // получаем компоненты связности
        auto components = ConnectivityFinder::findComponents(g);
        
        // цвета для разных компонент
        std::vector<std::string> colors = {
            "lightblue", "lightgreen", "lightyellow", "lightpink", 
            "lavender", "peachpuff", "plum", "thistle"
        };
        
        // визуализируем каждую компоненту в отдельном кластере
        for (size_t i = 0; i < components.size(); ++i) {
            std::string color = colors[i % colors.size()];
            file << "    subgraph cluster_" << i << " {\n";
            file << "        label=\"Component " << i << "\";\n";
            file << "        style=filled;\n";
            file << "        fillcolor=" << color << ";\n";
            file << "        node [style=filled, fillcolor=" << color << "];\n";
            
            for (int v : components[i]) {
                file << "        " << v << ";\n";
            }
            
            // рёбра внутри компоненты
            for (int u : components[i]) {
                for (int v : g.getNeighbors(u)) {
                    if (u < v && std::find(components[i].begin(), components[i].end(), v) != components[i].end()) {
                        file << "        " << u << " -- " << v << ";\n";
                    }
                }
            }
            
            file << "    }\n";
        }
        
        file << "}\n";
    }

    // с визуализацией случайного остова
    static void writeWithRandomSpanningTree(const Graph& g, const std::string& filename) {
        std::ofstream file(filename);
        file << "graph G {\n";
        file << "    rankdir=LR;\n";
        file << "    node [shape=circle, style=filled, fillcolor=lightblue];\n";
        
        // строим случайный остов 
        std::vector<bool> visited(g.verticesCount(), false);
        std::set<std::pair<int, int>> treeEdges;
        std::vector<int> stack;
        
        if (g.verticesCount() > 0) {
            stack.push_back(0);
            visited[0] = true;
            
            while (!stack.empty()) {
                int u = stack.back();
                stack.pop_back();
                
                auto neighbors = g.getNeighbors(u);
                std::vector<int> neighborList(neighbors.begin(), neighbors.end());
                
                // перемешиваем соседей для случайности
                std::random_device rd;
                std::mt19937 gen(rd());
                std::shuffle(neighborList.begin(), neighborList.end(), gen);
                
                for (int v : neighborList) {
                    if (!visited[v]) {
                        visited[v] = true;
                        treeEdges.insert({std::min(u, v), std::max(u, v)});
                        stack.push_back(v);
                    }
                }
            }
        }
        
        // рисуем все рёбра
        for (int u = 0; u < g.verticesCount(); ++u) {
            for (int v : g.getNeighbors(u)) {
                if (u < v) {
                    bool isTreeEdge = treeEdges.count({u, v}) > 0;
                    if (isTreeEdge) {
                        file << "    " << u << " -- " << v << " [color=red, penwidth=2.0];\n";
                    } else {
                        file << "    " << u << " -- " << v << " [color=gray];\n";
                    }
                }
            }
        }
        
        file << "}\n";
    }

    // с визуализацией случайного цикла 
    static void writeWithRandomCycle(const Graph& g, const std::string& filename) {
        std::ofstream file(filename);
        file << "graph G {\n";
        file << "    rankdir=LR;\n";
        file << "    node [shape=circle, style=filled, fillcolor=lightblue];\n";
        
        // ищем случайный цикл 
        std::set<std::pair<int, int>> cycleEdges;
        std::vector<int> parent(g.verticesCount(), -1);
        std::vector<bool> visited(g.verticesCount(), false);
        std::vector<bool> recStack(g.verticesCount(), false);
        bool found = false;
        
        // находим первый цикл
        for (int start = 0; start < g.verticesCount() && !found; ++start) {
            if (!visited[start]) {
                std::vector<int> stack;
                stack.push_back(start);
                
                while (!stack.empty() && !found) {
                    int u = stack.back();
                    
                    if (!visited[u]) {
                        visited[u] = true;
                        recStack[u] = true;
                    }
                    
                    bool pushed = false;
                    for (int v : g.getNeighbors(u)) {
                        if (!visited[v]) {
                            parent[v] = u;
                            stack.push_back(v);
                            pushed = true;
                            break;
                        } else if (recStack[v] && v != parent[u]) {
                            cycleEdges.insert({std::min(u, v), std::max(u, v)});
                            int curr = u;
                            while (curr != v) {
                                cycleEdges.insert({std::min(curr, parent[curr]), std::max(curr, parent[curr])});
                                curr = parent[curr];
                            }
                            found = true;
                            break;
                        }
                    }
                    
                    if (!pushed && !found) {
                        recStack[u] = false;
                        stack.pop_back();
                    }
                }
            }
        }
        
        // рисуем все рёбра
        for (int u = 0; u < g.verticesCount(); ++u) {
            for (int v : g.getNeighbors(u)) {
                if (u < v) {
                    bool isCycleEdge = cycleEdges.count({u, v}) > 0;
                    if (isCycleEdge) {
                        file << "    " << u << " -- " << v << " [color=green, penwidth=2.0];\n";
                    } else {
                        file << "    " << u << " -- " << v << " [color=gray];\n";
                    }
                }
            }
        }
        
        file << "}\n";
    }
};