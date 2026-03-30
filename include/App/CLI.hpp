#pragma once
#include "GraphCore/Graph.hpp"
#include "IO/Parsers/EdgeListParser.hpp"
#include "IO/Parsers/AdjacencyMatrixParser.hpp"
#include "IO/Parsers/DIMACSParser.hpp"
#include "Factories/GeneratorFactory.hpp"
#include "Factories/MetricFactory.hpp"
#include "Factories/SerializerFactory.hpp"
#include "Storage/AdjacencyListStorage.hpp"
#include "Storage/AdjacencyMatrixStorage.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <cctype>

class CLI {
public:
    static void run(int argc, char* argv[]) {
        if (argc < 2) {
            printHelp();
            return;
        }

        std::string command = argv[1];
        
        if (command == "--help" || command == "-h") {
            printHelp();
            return;
        }

        try {
            if (command == "--generate") {
                handleGenerate(argc, argv);
            } else if (command == "--load") {
                handleLoad(argc, argv);
            } else if (command == "--metric") {
                handleMetric(argc, argv);
            } else {
                std::cerr << "Unknown command: " << command << std::endl;
                printHelp();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return;
        }
    }

private:
    // проверка, является ли строка числом
    static bool isNumber(const std::string& s) {
        if (s.empty()) return false;
        if (s[0] == '-') return false;
        bool hasDot = false;
        for (char c : s) {
            if (c == '.') {
                if (hasDot) return false;
                hasDot = true;
            } else if (!std::isdigit(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        return true;
    }

    static void printHelp() {
        std::cout << R"(
GraphoDro4 - Graph Analysis Tool (OOP + Backend Switching)
==========================================================

Usage:
  grapho <command> [options]

Commands:
  --generate <type> <n> [m] [p]  Generate a graph
  --load <filename>              Load graph from file
  --metric <name>                Calculate metric
  --serialize <format> <output>  Serialize graph
  --backend <type>               Change storage backend (list/matrix)
  --help, -h                     Show this help

Available Generators: )" << std::endl;
        for (const auto& g : GeneratorFactory::getAvailableGenerators()) {
            std::cout << "    " << g << std::endl;
        }
        std::cout << R"(
Available Metrics: )" << std::endl;
        for (const auto& m : MetricFactory::getAvailableMetrics()) {
            std::cout << "    " << m << std::endl;
        }
        std::cout << R"(
Available Serializers: )" << std::endl;
        for (const auto& s : SerializerFactory::getAvailableSerializers()) {
            std::cout << "    " << s << std::endl;
        }
        std::cout << R"(
Backend Types:
    list    - Adjacency List (default, O(V+E) space)
    matrix  - Adjacency Matrix (O(V²) space, faster edge lookup)

Examples:
  grapho --generate complete 5 --metric density --metric diameter
  grapho --load graph.txt --serialize graphviz output.dot
  grapho --generate cycle 10 --backend matrix --metric diameter
  grapho --generate random 20 0.3 --metric density --backend list
)" << std::endl;
    }

    static void handleGenerate(int argc, char* argv[]) {
        if (argc < 4) {
            std::cerr << "Usage: --generate <type> <n> [m] [p]" << std::endl;
            return;
        }

        std::string type = argv[2];
        std::string n_str = argv[3];
        
        if (!isNumber(n_str)) {
            std::cerr << "Error: Expected number for n, got: " << n_str << std::endl;
            return;
        }
        
        int n = 0;
        int m = 0;
        double p = 0.0;
        
        try {
            n = std::stoi(n_str);
            
            if (argc > 4 && isNumber(argv[4])) {
                m = std::stoi(argv[4]);
                
                if (argc > 5 && isNumber(argv[5])) {
                    p = std::stod(argv[5]);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing arguments: " << e.what() << std::endl;
            std::cerr << "Usage: --generate <type> <n> [m] [p]" << std::endl;
            return;
        }

        try {
            auto generator = GeneratorFactory::create(type);
            Graph g = generator->generate(n, m, p);

            std::cout << "Generated " << type << " graph with " 
                      << g.verticesCount() << " vertices and " 
                      << g.edgesCount() << " edges" << std::endl;
            std::cout << "Current storage: " << g.getStorageType() << std::endl;

            processAdditionalCommands(argc, argv, g);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    static void handleLoad(int argc, char* argv[]) {
        if (argc < 3) {
            std::cerr << "Usage: --load <filename>" << std::endl;
            return;
        }

        std::string filename = argv[2];
        Graph g;

        if (filename.find(".mtx") != std::string::npos || 
            filename.find(".matrix") != std::string::npos) {
            g = AdjacencyMatrixParser::parse(filename);
        } else if (filename.find(".dimacs") != std::string::npos ||
                   filename.find(".col") != std::string::npos) {
            g = DIMACSParser::parse(filename);
        } else {
            g = EdgeListParser::parse(filename);
        }

        std::cout << "Loaded graph with " 
                  << g.verticesCount() << " vertices and " 
                  << g.edgesCount() << " edges" << std::endl;
        std::cout << "Current storage: " << g.getStorageType() << std::endl;

        processAdditionalCommands(argc, argv, g);
    }

    static void handleMetric(int argc, char* argv[]) {
        std::cerr << "--metric requires --generate or --load first" << std::endl;
    }

    static void handleBackend(const std::string& type, Graph& g) {
        if (type == "list") {
            g.changeStorage(std::make_unique<AdjacencyListStorage>());
            std::cout << "Changed to Adjacency List storage" << std::endl;
        } else if (type == "matrix") {
            g.changeStorage(std::make_unique<AdjacencyMatrixStorage>());
            std::cout << "Changed to Adjacency Matrix storage" << std::endl;
        } else {
            std::cerr << "Unknown backend type: " << type << std::endl;
            std::cerr << "Available: list, matrix" << std::endl;
        }
        std::cout << "Current storage: " << g.getStorageType() << std::endl;
    }

    static void processAdditionalCommands(int argc, char* argv[], Graph& g) {
        for (int i = 4; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "--metric" && i + 1 < argc) {
                std::string metric = argv[++i];
                calculateMetric(g, metric);
            } else if (arg == "--serialize" && i + 2 < argc) {
                std::string format = argv[++i];
                std::string output = argv[++i];
                serializeGraph(g, format, output);
            } else if (arg == "--backend" && i + 1 < argc) {
                std::string backend = argv[++i];
                handleBackend(backend, g);
            }
        }
    }

    static void calculateMetric(const Graph& g, const std::string& name) {
        auto metric = MetricFactory::create(name);
        double result = metric->calculate(g);
        
        std::cout << metric->getName() << ": " << result;
        if (!metric->getUnit().empty()) {
            std::cout << " " << metric->getUnit();
        }
        std::cout << std::endl;
    }

    static void serializeGraph(const Graph& g, const std::string& format, 
                               const std::string& output) {
        auto serializer = SerializerFactory::create(format);
        serializer->serialize(g, output);
        std::cout << "Serialized to " << format << ": " << output << std::endl;
    }
};