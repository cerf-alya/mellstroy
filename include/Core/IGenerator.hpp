#pragma once
#include "GraphCore/Graph.hpp"
#include <string>
#include <memory>

class IGenerator {
public:
    virtual ~IGenerator() = default;
    virtual Graph generate(int n, int m = 0, double p = 0.0) = 0;
    virtual std::string getName() const = 0;
};