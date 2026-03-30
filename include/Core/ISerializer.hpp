#pragma once
#include "GraphCore/Graph.hpp"
#include <string>
#include <memory>

class ISerializer {
public:
    virtual ~ISerializer() = default;
    virtual void serialize(const Graph& g, const std::string& filename) = 0;
    virtual std::string getFormat() const = 0;
    virtual std::string getExtension() const = 0;
};