#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include "Core/IGraphStorage.hpp"
#include "Storage/AdjacencyListStorage.hpp"

using VertexId = int;

class GraphVisitor {
public:
    virtual ~GraphVisitor() = default;
    virtual void onEnterVertex(VertexId v, int depth) {}
    virtual void onLeaveVertex(VertexId v, int depth) {}
    virtual void onTraverseEdge(VertexId u, VertexId v, int type) {}
};

class Graph {
public:
    // конструкторы
    Graph() : storage_(std::make_unique<AdjacencyListStorage>()) {}
    
    explicit Graph(int num_vertices) : storage_(std::make_unique<AdjacencyListStorage>()) {
        for (int i = 0; i < num_vertices; ++i) {
            addVertex();
        }
    }
    
    explicit Graph(std::unique_ptr<IGraphStorage> storage) 
        : storage_(std::move(storage)) {}
    
    ~Graph() = default;
    
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph(Graph&&) = default;
    Graph& operator=(Graph&&) = default;

    //  управление вершинами 
    VertexId addVertex() {
        storage_->addVertex();
        return storage_->verticesCount() - 1;
    }

    void removeVertex(VertexId v) {  //
        storage_->removeVertex(v);
    }

    int verticesCount() const {
        return storage_->verticesCount();
    }

    //  управление ребрами 
    bool addEdge(VertexId u, VertexId v, bool is_directed = false) {
        return storage_->addEdge(u, v, is_directed);
    }

    bool removeEdge(VertexId u, VertexId v, bool is_directed = false) {
        return storage_->removeEdge(u, v, is_directed);
    }

    int edgesCount() const {
        return storage_->edgesCount();
    }

    bool hasEdge(VertexId u, VertexId v) const {
        return storage_->hasEdge(u, v);
    }

    bool hasVertex(VertexId v) const {
        return v >= 0 && v < verticesCount();
    }

    //  доступ к структуре 
    std::unordered_set<VertexId> getNeighbors(VertexId v) const {
        return storage_->getNeighbors(v);
    }
    
    int degree(VertexId v) const {
        return storage_->degree(v);
    }

    bool isLeaf(VertexId v) const {
        return degree(v) == 1;
    }

    // смена бекенда 
    void changeStorage(std::unique_ptr<IGraphStorage> newStorage) {
        int n = verticesCount();
        for (int i = 0; i < n; ++i) {
            newStorage->addVertex();
        }
        
        for (int u = 0; u < n; ++u) {
            for (int v : getNeighbors(u)) {
                if (u < v) {
                    newStorage->addEdge(u, v, false);
                }
            }
        }
        
        storage_ = std::move(newStorage);
    }
    
    std::string getStorageType() const {
        return storage_->getStorageType();
    }

    // очистка 
    void clear() {
        storage_->clear();
    }

    // алгоритмы обхода
    void dfs(VertexId start, GraphVisitor& visitor) const {
        if (!hasVertex(start)) return;
        std::vector<bool> visited(verticesCount(), false);
        dfsRecursive(start, visited, visitor, 0);
    }
    
    void bfs(VertexId start, std::function<void(VertexId, int)> processVertex) const {
        if (!hasVertex(start)) return;
        std::vector<bool> visited(verticesCount(), false);
        std::vector<std::pair<VertexId, int>> queue;
        queue.push_back({start, 0});
        visited[start] = true;
        size_t head = 0;
        while(head < queue.size()){
            auto [u, depth] = queue[head++];
            processVertex(u, depth);
            for (VertexId v : getNeighbors(u)) {
                if (!visited[v]) {
                    visited[v] = true;
                    queue.push_back({v, depth + 1});
                }
            }
        }
    }

private:
    std::unique_ptr<IGraphStorage> storage_;

    void dfsRecursive(VertexId u, std::vector<bool>& visited, 
                      GraphVisitor& visitor, int depth) const {
        visited[u] = true;
        visitor.onEnterVertex(u, depth);
        for (VertexId v : getNeighbors(u)) {
            if (!visited[v]) {
                visitor.onTraverseEdge(u, v, 0);
                dfsRecursive(v, visited, visitor, depth + 1);
            } else {
                visitor.onTraverseEdge(u, v, 1);
            }
        }
        visitor.onLeaveVertex(u, depth);
    }
};