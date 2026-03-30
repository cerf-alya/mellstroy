#pragma once
#include "GraphCore/Graph.hpp"
#include <vector>
#include <unordered_set>

class IGraphStorage {
public:
    virtual ~IGraphStorage() = default;
    
    // управление вершинами
    virtual void addVertex() = 0;
    virtual void removeVertex(int v) = 0;
    virtual int verticesCount() const = 0;
    
    // ууправление рёбрами
    virtual bool addEdge(int u, int v, bool directed = false) = 0;
    virtual bool removeEdge(int u, int v, bool directed = false) = 0;
    virtual int edgesCount() const = 0;
    virtual bool hasEdge(int u, int v) const = 0;
    
    // доступ к соседям
    virtual std::unordered_set<int> getNeighbors(int v) const = 0;
    virtual int degree(int v) const = 0;
    
    // очистка
    virtual void clear() = 0;
    
    // тип хранилища
    virtual std::string getStorageType() const = 0;
};