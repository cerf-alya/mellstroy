#pragma once
#include "Core/IGraphStorage.hpp"
#include <vector>
#include <unordered_set>
#include <stdexcept>

class AdjacencyListStorage : public IGraphStorage {
public:
    void addVertex() override {
        adj_list_.emplace_back();
    }
    
    void removeVertex(int v) override {
        if (v < 0 || v >= verticesCount()) {
            throw std::out_of_range("Invalid vertex ID");
        }
        
        // удаляем все рёбра, инцидентные вершине v
        auto neighbors_copy = adj_list_[v];
        for (int u : neighbors_copy) {
            adj_list_[u].erase(v);
            edges_count_--;
        }
        adj_list_[v].clear();
    }
    
    int verticesCount() const override {
        return static_cast<int>(adj_list_.size());
    }
    
    bool addEdge(int u, int v, bool directed = false) override {
        if (u < 0 || u >= verticesCount() || v < 0 || v >= verticesCount()) {
            return false;
        }
        if (hasEdge(u, v)) {
            return false;
        }
        
        adj_list_[u].insert(v);
        if (!directed) {
            adj_list_[v].insert(u);
        }
        edges_count_++;
        return true;
    }
    
    bool removeEdge(int u, int v, bool directed = false) override {
        if (!hasEdge(u, v)) {
            return false;
        }
        
        adj_list_[u].erase(v);
        if (!directed) {
            adj_list_[v].erase(u);
        }
        edges_count_--;
        return true;
    }
    
    int edgesCount() const override {
        return edges_count_;
    }
    
    bool hasEdge(int u, int v) const override {
        if (u < 0 || u >= verticesCount() || v < 0 || v >= verticesCount()) {
            return false;
        }
        return adj_list_[u].count(v) > 0;
    }
    
    std::unordered_set<int> getNeighbors(int v) const override {
        if (v < 0 || v >= verticesCount()) {
            return {};
        }
        return adj_list_[v];
    }
    
    int degree(int v) const override {
        return static_cast<int>(getNeighbors(v).size());
    }
    
    void clear() override {
        adj_list_.clear();
        edges_count_ = 0;
    }
    
    std::string getStorageType() const override {
        return "AdjacencyList";
    }

private:
    std::vector<std::unordered_set<int>> adj_list_;
    int edges_count_ = 0;
};