#pragma once
#include "GraphCore/Graph.hpp"
#include <string>
#include <memory>

class IMetric {
public:
    virtual ~IMetric() = default;
    virtual double calculate(const Graph& g) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getUnit() const { return ""; }
};