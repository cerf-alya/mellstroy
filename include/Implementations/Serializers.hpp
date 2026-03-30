#pragma once
#include "Core/ISerializer.hpp"
#include "IO/Serializers/GraphVizSerializer.hpp"
#include "IO/Serializers/Program4YouSerializer.hpp"

class GraphVizSerializerImpl : public ISerializer {
public:
    void serialize(const Graph& g, const std::string& filename) override {
        GraphVizSerializer::write(g, filename);
    }
    std::string getFormat() const override { return "graphviz"; }
    std::string getExtension() const override { return ".dot"; }
};

class GraphVizCompSerializerImpl : public ISerializer {
public:
    void serialize(const Graph& g, const std::string& filename) override {
        GraphVizSerializer::writeWithComponents(g, filename);
    }
    std::string getFormat() const override { return "graphviz-comp"; }
    std::string getExtension() const override { return ".dot"; }
};

class GraphVizTreeSerializerImpl : public ISerializer {
public:
    void serialize(const Graph& g, const std::string& filename) override {
        GraphVizSerializer::writeWithRandomSpanningTree(g, filename);
    }
    std::string getFormat() const override { return "graphviz-tree"; }
    std::string getExtension() const override { return ".dot"; }
};

class GraphVizCycleSerializerImpl : public ISerializer {
public:
    void serialize(const Graph& g, const std::string& filename) override {
        GraphVizSerializer::writeWithRandomCycle(g, filename);
    }
    std::string getFormat() const override { return "graphviz-cycle"; }
    std::string getExtension() const override { return ".dot"; }
};

class Program4YouSerializerImpl : public ISerializer {
public:
    void serialize(const Graph& g, const std::string& filename) override {
        Program4YouSerializer::write(g, filename);
    }
    std::string getFormat() const override { return "program4you"; }
    std::string getExtension() const override { return ".edges"; }
};