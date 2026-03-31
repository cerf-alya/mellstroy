#pragma once
#include "Core/ISerializer.hpp"
#include "Implementations/Serializers.hpp"
#include <memory>
#include <vector>
#include <stdexcept>

class SerializerFactory {
public:
    static std::unique_ptr<ISerializer> create(const std::string& name) {
        if (name == "graphviz") return std::make_unique<GraphVizSerializerImpl>();
        if (name == "graphviz-comp") return std::make_unique<GraphVizCompSerializerImpl>();
        if (name == "graphviz-tree") return std::make_unique<GraphVizTreeSerializerImpl>();
        if (name == "graphviz-cycle") return std::make_unique<GraphVizCycleSerializerImpl>();
        if (name == "program4you") return std::make_unique<Program4YouSerializerImpl>();
        
        throw std::invalid_argument("Unknown serializer: " + name);
    }
    
    static std::vector<std::string> getAvailableSerializers() {
        return {"graphviz", "graphviz-comp", "graphviz-tree", "graphviz-cycle", "program4you"};
    }
};