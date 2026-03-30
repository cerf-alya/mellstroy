#pragma once
#include "GraphCore/Graph.hpp"
#include <vector>

class ConnectivityFinder {
public:
    static std::vector<std::vector<VertexId>> findComponents(const Graph& g) {
        std::vector<std::vector<VertexId>> components;
        std::vector<bool> visited(g.verticesCount(), false);
        
        for (VertexId v = 0; v < g.verticesCount(); v++) {
            if (!visited[v]) {
                std::vector<VertexId> component;
                dfsComponent(g, v, visited, component);
                components.push_back(component);
            }
        }
        return components;
    }
    
private:
    static void dfsComponent(const Graph& g, VertexId u, 
                            std::vector<bool>& visited,
                            std::vector<VertexId>& component) {
        visited[u] = true;
        component.push_back(u);
        
        for (VertexId v : g.getNeighbors(u)) {
            if (!visited[v]) {
                dfsComponent(g, v, visited, component);
            }
        }
    }
};