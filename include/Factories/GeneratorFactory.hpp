#pragma once
#include "Core/IGenerator.hpp"
#include "Implementations/Generators.hpp"
#include <memory>
#include <vector>
#include <stdexcept>

class GeneratorFactory {
public:
    static std::unique_ptr<IGenerator> create(const std::string& name) {
        if (name == "complete") return std::make_unique<CompleteGenerator>();
        if (name == "bipartite") return std::make_unique<BipartiteGenerator>();
        if (name == "tree") return std::make_unique<TreeGenerator>();
        if (name == "star") return std::make_unique<StarGenerator>();
        if (name == "cycle") return std::make_unique<CycleGenerator>();
        if (name == "path") return std::make_unique<PathGenerator>();
        if (name == "wheel") return std::make_unique<WheelGenerator>();
        if (name == "random") return std::make_unique<RandomGenerator>();
        if (name == "cubic") return std::make_unique<CubicGenerator>();
        if (name == "components") return std::make_unique<ComponentsGenerator>();
        if (name == "bridges") return std::make_unique<BridgesGenerator>();
        if (name == "ap") return std::make_unique<APGenerator>();
        if (name == "halin") return std::make_unique<HalinGenerator>();
        
        throw std::invalid_argument("Unknown generator: " + name);
    }
    
    static std::vector<std::string> getAvailableGenerators() {
        return {"complete", "bipartite", "tree", "star", "cycle", "path", 
                "wheel", "random", "cubic", "components", "bridges", "ap", "halin"};
    }
};