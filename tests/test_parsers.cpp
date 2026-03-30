#include "IO/Parsers/EdgeListParser.hpp"
#include "IO/Parsers/AdjacencyMatrixParser.hpp"
#include "IO/Parsers/DIMACSParser.hpp"
#include "IO/Serializers/GraphVizSerializer.hpp"      
#include "IO/Serializers/Program4YouSerializer.hpp"  
#include <iostream>
#include <cassert>
#include <filesystem>

void test_parsers() {
    namespace fs = std::filesystem;
    
    // создаем тестовый граф
    Graph original;
    for (int i = 0; i < 4; ++i) original.addVertex();
    original.addEdge(0, 1);
    original.addEdge(1, 2);
    original.addEdge(2, 3);
    
    // ттест EdgeListParser
    EdgeListParser::write(original, "test_edges.txt");
    Graph loaded_edge = EdgeListParser::parse("test_edges.txt");
    assert(loaded_edge.verticesCount() == 4);
    assert(loaded_edge.edgesCount() == 3);
    
    // тест AdjacencyMatrixParser
    AdjacencyMatrixParser::write(original, "test_matrix.txt");
    Graph loaded_matrix = AdjacencyMatrixParser::parse("test_matrix.txt");
    assert(loaded_matrix.verticesCount() == 4);
    assert(loaded_matrix.edgesCount() == 3);
    
    // тест DIMACSParser
    DIMACSParser::write(original, "test_dimacs.txt");
    Graph loaded_dimacs = DIMACSParser::parse("test_dimacs.txt");
    assert(loaded_dimacs.verticesCount() == 4);
    assert(loaded_dimacs.edgesCount() == 3);
    
    // тест GraphVizSerializer
    GraphVizSerializer::write(original, "test_graphviz.dot");
    GraphVizSerializer::writeWithComponents(original, "test_graphviz_components.dot");
    GraphVizSerializer::writeWithRandomSpanningTree(original, "test_graphviz_spanning.dot");
    GraphVizSerializer::writeWithRandomCycle(original, "test_graphviz_cycle.dot");
    
    assert(fs::exists("test_graphviz.dot"));
    assert(fs::exists("test_graphviz_components.dot"));
    assert(fs::exists("test_graphviz_spanning.dot"));
    assert(fs::exists("test_graphviz_cycle.dot"));
    std::cout << "GraphViz serializer test passed" << std::endl;
    
    // тест Program4YouSerializer
    Program4YouSerializer::write(original, "test_program4you.edges");
    Graph loaded_p4y = Program4YouSerializer::read("test_program4you.edges");
    assert(loaded_p4y.verticesCount() == 4);
    assert(loaded_p4y.edgesCount() == 3);
    std::cout << "Program4You serializer test passed" << std::endl;
    
    // чистим тестовые файлы
    fs::remove("test_edges.txt");
    fs::remove("test_matrix.txt");
    fs::remove("test_dimacs.txt");
    fs::remove("test_graphviz.dot");
    fs::remove("test_graphviz_components.dot");
    fs::remove("test_graphviz_spanning.dot");
    fs::remove("test_graphviz_cycle.dot");
    fs::remove("test_program4you.edges");
    
    std::cout << "test_parsers passed" << std::endl;
}

int main() {
    test_parsers();
    std::cout << "\nAll parser tests passed!" << std::endl;
    return 0;
}