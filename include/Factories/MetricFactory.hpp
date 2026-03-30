#pragma once
#include "Core/IMetric.hpp"
#include "Implementations/Metrics.hpp"
#include <memory>
#include <vector>
#include <stdexcept>

class MetricFactory {
public:
    static std::unique_ptr<IMetric> create(const std::string& name) {
        if (name == "density") return std::make_unique<DensityMetric>();
        if (name == "diameter") return std::make_unique<DiameterMetric>();
        if (name == "transitivity") return std::make_unique<TransitivityMetric>();
        if (name == "components") return std::make_unique<ComponentsMetric>();
        if (name == "articulation") return std::make_unique<ArticulationMetric>();
        if (name == "bridges") return std::make_unique<BridgesMetric>();
        if (name == "bipartite") return std::make_unique<BipartiteMetric>();
        if (name == "chromatic") return std::make_unique<ChromaticMetric>();
        
        throw std::invalid_argument("Unknown metric: " + name);
    }
    
    static std::vector<std::string> getAvailableMetrics() {
        return {"density", "diameter", "transitivity", "components", 
                "articulation", "bridges", "bipartite", "chromatic"};
    }
};