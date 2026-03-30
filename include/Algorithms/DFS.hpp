#pragma once
#include "GraphCore/Graph.hpp"
#include <vector>
#include <functional>

class DFSVisitor {
public:
    virtual void onEnterVertex(VertexId v, int depth) {}
    virtual void onLeaveVertex(VertexId v, int depth) {}
    virtual void onTraverseEdge(VertexId u, VertexId v, int type) {}
    virtual ~DFSVisitor() = default;
};

class DFS {
public:
    static void run(const Graph& g, VertexId start, DFSVisitor& visitor) {
        std::vector<bool> visited(g.verticesCount(), false);
        dfsRecursive(g, start, visited, visitor, 0);
    }
    
private:
    static void dfsRecursive(const Graph& g, VertexId u, 
                            std::vector<bool>& visited, 
                            DFSVisitor& visitor, int depth) {
        visited[u] = true;
        visitor.onEnterVertex(u, depth);
        
        for (VertexId v : g.getNeighbors(u)) {
            if (!visited[v]) {
                visitor.onTraverseEdge(u, v, 0); // tree edge
                dfsRecursive(g, v, visited, visitor, depth + 1);
            } else {
                visitor.onTraverseEdge(u, v, 1); // back edge
            }
        }
        
        visitor.onLeaveVertex(u, depth);
    }
};