#pragma once
#include "Core/IMetric.hpp"
#include "Algorithms/Metrics.hpp"

class DensityMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return GraphMetrics::density(g);
    }
    std::string getName() const override { return "density"; }
    std::string getUnit() const override { return ""; }
};

class DiameterMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return static_cast<double>(GraphMetrics::diameter(g));
    }
    std::string getName() const override { return "diameter"; }
    std::string getUnit() const override { return "edges"; }
};

class TransitivityMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return GraphMetrics::transitivity(g);
    }
    std::string getName() const override { return "transitivity"; }
    std::string getUnit() const override { return ""; }
};

class ComponentsMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return static_cast<double>(GraphMetrics::countConnectedComponents(g));
    }
    std::string getName() const override { return "components"; }
    std::string getUnit() const override { return "components"; }
};

class ArticulationMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return static_cast<double>(GraphMetrics::countArticulationPoints(g));
    }
    std::string getName() const override { return "articulation"; }
    std::string getUnit() const override { return "points"; }
};

class BridgesMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return static_cast<double>(GraphMetrics::countBridges(g));
    }
    std::string getName() const override { return "bridges"; }
    std::string getUnit() const override { return "bridges"; }
};

class BipartiteMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return GraphMetrics::isBipartite(g) ? 1.0 : 0.0;
    }
    std::string getName() const override { return "bipartite"; }
    std::string getUnit() const override { return "yes/no"; }
};

class ChromaticMetric : public IMetric {
public:
    double calculate(const Graph& g) override {
        return static_cast<double>(GraphMetrics::chromaticNumberGreedy(g));
    }
    std::string getName() const override { return "chromatic"; }
    std::string getUnit() const override { return "colors"; }
};