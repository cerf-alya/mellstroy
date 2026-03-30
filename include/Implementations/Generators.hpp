#pragma once
#include "Core/IGenerator.hpp"
#include "Generators/BasicGenerators.hpp"
#include "Generators/AdvancedGenerators.hpp"

class CompleteGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::complete(n);
    }
    std::string getName() const override { return "complete"; }
};

class BipartiteGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::completeBipartite(n, m > 0 ? m : n);
    }
    std::string getName() const override { return "bipartite"; }
};

class TreeGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::tree(n);
    }
    std::string getName() const override { return "tree"; }
};

class StarGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::star(n);
    }
    std::string getName() const override { return "star"; }
};

class CycleGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::cycle(n);
    }
    std::string getName() const override { return "cycle"; }
};

class PathGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::path(n);
    }
    std::string getName() const override { return "path"; }
};

class WheelGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::wheel(n);
    }
    std::string getName() const override { return "wheel"; }
};

class RandomGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return BasicGenerators::randomGnp(n, p > 0 ? p : 0.5);
    }
    std::string getName() const override { return "random"; }
};

class CubicGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return AdvancedGenerators::cubic(n);
    }
    std::string getName() const override { return "cubic"; }
};

class ComponentsGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return AdvancedGenerators::withComponents(n, m > 0 ? m : 2);
    }
    std::string getName() const override { return "components"; }
};

class BridgesGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return AdvancedGenerators::withBridges(n, m > 0 ? m : 1);
    }
    std::string getName() const override { return "bridges"; }
};

class APGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return AdvancedGenerators::withArticulationPoints(n, m > 0 ? m : 1);
    }
    std::string getName() const override { return "ap"; }
};

class HalinGenerator : public IGenerator {
public:
    Graph generate(int n, int m = 0, double p = 0.0) override {
        return AdvancedGenerators::halin(n);
    }
    std::string getName() const override { return "halin"; }
};