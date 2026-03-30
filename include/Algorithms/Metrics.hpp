#pragma once
#include "GraphCore/Graph.hpp"
#include <cmath>
#include <queue>
#include <algorithm>
#include <vector>
#include <functional>
#include <unordered_set>

class GraphMetrics {
public:
    // 1. плотность графа
    static double density(const Graph& g) {
        int n = g.verticesCount();
        if (n < 2) return 0.0;
        int m = g.edgesCount();
        return (2.0 * m) / (static_cast<double>(n) * (n - 1));
    }

    // 2. количество компонент связности 
    static int countConnectedComponents(const Graph& g) {
        int n = g.verticesCount();
        std::vector<bool> visited(n, false);
        int components = 0;

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                components++;
                std::vector<int> q;
                q.push_back(i);
                visited[i] = true;
                
                size_t head = 0;
                while(head < q.size()){
                    int u = q[head++];
                    for (int v : g.getNeighbors(u)) {
                        if (!visited[v]) {
                            visited[v] = true;
                            q.push_back(v);
                        }
                    }
                }
            }
        }
        return components;
    }

    // 3. проверка на двудольность 
    static bool isBipartite(const Graph& g) {
        int n = g.verticesCount();
        std::vector<int> color(n, -1);

        for (int start = 0; start < n; ++start) {
            if (color[start] != -1) continue;

            std::vector<int> q;
            q.push_back(start);
            color[start] = 0;

            size_t head = 0;
            while(head < q.size()){
                int u = q[head++];
                for (int v : g.getNeighbors(u)) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push_back(v);
                    } else if (color[v] == color[u]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // 4. диаметр графа 
    static int diameter(const Graph& g) {
        int n = g.verticesCount();
        if (n == 0) return 0;
        if (n == 1) return 0;
        
        int maxDist = 0;
        
        for (int start = 0; start < n; ++start) {
            std::vector<int> dist(n, -1);
            std::queue<int> q;
            
            dist[start] = 0;
            q.push(start);
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                maxDist = std::max(maxDist, dist[u]);
                
                for (int v : g.getNeighbors(u)) {
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
        }
        
        return maxDist;
    }

    // 5. транзитивность графа 
    static double transitivity(const Graph& g) {
        int n = g.verticesCount();
        if (n < 3) return 0.0;
        
        int triangles = 0;
        int triads = 0;
        
        // считаем треугольники и триады
        for (int u = 0; u < n; ++u) {
            auto neighbors = g.getNeighbors(u);
            std::vector<int> neighborList(neighbors.begin(), neighbors.end());
            int degree = neighborList.size();
            
            triads += degree * (degree - 1) / 2;
            
            for (size_t i = 0; i < neighborList.size(); ++i) {
                for (size_t j = i + 1; j < neighborList.size(); ++j) {
                    int v = neighborList[i];
                    int w = neighborList[j];
                    if (g.hasEdge(v, w)) {
                        triangles++;
                    }
                }
            }
        }
        triangles /= 3;
        if (triads == 0) return 0.0;
        return (3.0 * triangles) / triads;  // ✅
    }

    // 6. количество точек сочленения 
    static int countArticulationPoints(const Graph& g) {
        int n = g.verticesCount();
        if (n < 3) return 0;
        
        std::vector<int> tin(n, -1), low(n, -1);
        std::vector<bool> visited(n, false);
        std::vector<bool> isAP(n, false);
        int timer = 0;
        
        std::function<void(int, int)> dfs = [&](int u, int p = -1) {
            visited[u] = true;
            tin[u] = low[u] = timer++;
            int children = 0;
            
            for (int v : g.getNeighbors(u)) {
                if (v == p) continue;
                
                if (visited[v]) {
                    low[u] = std::min(low[u], tin[v]);
                } else {
                    children++;
                    dfs(v, u);
                    low[u] = std::min(low[u], low[v]);
                    
                    if (low[v] >= tin[u] && p != -1) {
                        isAP[u] = true;
                    }
                }
            }
            
            if (p == -1 && children > 1) {
                isAP[u] = true;
            }
        };
        
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfs(i, -1);
            }
        }
        
        int count = 0;
        for (int i = 0; i < n; ++i) {
            if (isAP[i]) count++;
        }
        
        return count;
    }

    // 7. количество мостов 
    static int countBridges(const Graph& g) {
        int n = g.verticesCount();
        if (n < 2) return 0;
        
        std::vector<int> tin(n, -1), low(n, -1);
        std::vector<bool> visited(n, false);
        int timer = 0;
        int bridgeCount = 0;
        
        std::function<void(int, int)> dfs = [&](int u, int p = -1) {
            visited[u] = true;
            tin[u] = low[u] = timer++;
            
            for (int v : g.getNeighbors(u)) {
                if (v == p) continue;
                
                if (visited[v]) {
                    low[u] = std::min(low[u], tin[v]);
                } else {
                    dfs(v, u);
                    low[u] = std::min(low[u], low[v]);
                    
                    if (low[v] > tin[u]) {
                        bridgeCount++;
                    }
                }
            }
        };
        
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfs(i, -1);
            }
        }
        
        return bridgeCount;
    }

    // 8. хроматическое число 
    static int chromaticNumberGreedy(const Graph& g) {
        int n = g.verticesCount();
        if (n == 0) return 0;
        if (n == 1) return 1;
        
        std::vector<int> color(n, -1);
        int maxColor = 0;
        
        // красим вершины по порядку 
        for (int u = 0; u < n; ++u) {
            std::unordered_set<int> neighborColors;
            
            for (int v : g.getNeighbors(u)) {
                if (color[v] != -1) {
                    neighborColors.insert(color[v]);
                }
            }
            
            // находим минимальный доступный цвет
            int c = 0;
            while (neighborColors.count(c) > 0) {
                c++;
            }
            
            color[u] = c;
            maxColor = std::max(maxColor, c);
        }
        
        return maxColor + 1; // +1 потому что цвета с 0
    }
};