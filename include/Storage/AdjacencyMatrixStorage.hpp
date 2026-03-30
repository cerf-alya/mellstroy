#pragma once
#include "Core/IGraphStorage.hpp"
#include <vector>
#include <unordered_set>
#include <stdexcept>

class AdjacencyMatrixStorage : public IGraphStorage {
public:
    void addVertex() override {
        int n = static_cast<int>(matrix_.size());
        matrix_.resize(n + 1);
        for (auto& row : matrix_) {
            row.resize(n + 1, false);
        }
    }
    
    void removeVertex(int v) override {
        if (v < 0 || v >= verticesCount()) {
            throw std::out_of_range("Invalid vertex ID");
        }
        
        int n = verticesCount();
        // удаляем все рёбра, инцидентные вершине v
        for (int i = 0; i < n; ++i) {
            if (matrix_[v][i]) {
                matrix_[v][i] = false;
                matrix_[i][v] = false;
                edges_count_--;
            }
        }
    }
    
    int verticesCount() const override {
        return static_cast<int>(matrix_.size());
    }
    
    bool addEdge(int u, int v, bool directed = false) override {
        if (u < 0 || u >= verticesCount() || v < 0 || v >= verticesCount()) {
            return false;
        }
        if (hasEdge(u, v)) {
            return false;
        }
        
        matrix_[u][v] = true;
        if (!directed) {
            matrix_[v][u] = true;
        }
        edges_count_++;
        return true;
    }
    
    bool removeEdge(int u, int v, bool directed = false) override {
        if (!hasEdge(u, v)) {
            return false;
        }
        
        matrix_[u][v] = false;
        if (!directed) {
            matrix_[v][u] = false;
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
        return matrix_[u][v];
    }
    
    std::unordered_set<int> getNeighbors(int v) const override {
        std::unordered_set<int> neighbors;
        if (v < 0 || v >= verticesCount()) {
            return neighbors;
        }
        
        for (int i = 0; i < verticesCount(); ++i) {
            if (matrix_[v][i]) {
                neighbors.insert(i);
            }
        }
        return neighbors;
    }
    
    int degree(int v) const override {
        return static_cast<int>(getNeighbors(v).size());
    }
    
    void clear() override {
        matrix_.clear();
        edges_count_ = 0;
    }
    
    std::string getStorageType() const override {
        return "AdjacencyMatrix";
    }

private:
    std::vector<std::vector<bool>> matrix_;
    int edges_count_ = 0;
};