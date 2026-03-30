#include "GraphCore/Graph.hpp"
#include <iostream>
#include <cassert>
#include "Algorithms/Metrics.hpp"
#include "Generators/BasicGenerators.hpp"
#include "Generators/AdvancedGenerators.hpp"  

void test_generators() {
    // тест полного графа
    Graph g_complete = BasicGenerators::complete(4);
    assert(g_complete.verticesCount() == 4);
    assert(g_complete.edgesCount() == 6); 
    
    // тест звезды
    Graph g_star = BasicGenerators::star(5);
    assert(g_star.verticesCount() == 5);
    assert(g_star.edgesCount() == 4);
    assert(g_star.degree(0) == 4); 
    
    // тест цикла
    Graph g_cycle = BasicGenerators::cycle(5);
    assert(g_cycle.verticesCount() == 5);
    assert(g_cycle.edgesCount() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(g_cycle.degree(i) == 2);
    }
    
    // тест дерева: N вершин, N-1 ребро
    Graph g_tree = BasicGenerators::tree(10);
    assert(g_tree.verticesCount() == 10);
    assert(g_tree.edgesCount() == 9);
    
    std::cout << "test_generators passed" << std::endl;
}

void test_advanced_generators() {
    // тест графа с компонентами связности
    Graph g_comp = AdvancedGenerators::withComponents(10, 3);
    assert(g_comp.verticesCount() == 10);
    assert(GraphMetrics::countConnectedComponents(g_comp) == 3);
    std::cout << "withComponents test passed" << std::endl;
    
    // тест графа с мостами
    Graph g_bridge = AdvancedGenerators::withBridges(8, 3);
    assert(g_bridge.verticesCount() == 8);
    std::cout << "withBridges test passed" << std::endl;
    
    // тест графа с точками сочленения
    Graph g_ap = AdvancedGenerators::withArticulationPoints(6, 2);
    assert(g_ap.verticesCount() == 6);
    std::cout << "withArticulationPoints test passed" << std::endl;
    
    // тест кубического графа
    Graph g_cubic = AdvancedGenerators::cubic(6);
    assert(g_cubic.verticesCount() == 6);
    std::cout << "cubic test passed" << std::endl;
    
    std::cout << "test_advanced_generators passed" << std::endl;
}

void test_metrics() {
    // тест плотности: полный граф K3 имеет плотность 1.0
    Graph g_complete;
    for(int i=0; i<3; ++i) g_complete.addVertex();
    g_complete.addEdge(0,1); g_complete.addEdge(0,2); g_complete.addEdge(1,2);
    
    assert(std::abs(GraphMetrics::density(g_complete) - 1.0) < 0.001);
    std::cout << "Density test passed" << std::endl;

    // тест компонент: граф с 2 компонентами
    Graph g_multi;
    for(int i=0; i<4; ++i) g_multi.addVertex();
    g_multi.addEdge(0,1);
    g_multi.addEdge(2,3);
    
    assert(GraphMetrics::countConnectedComponents(g_multi) == 2);
    std::cout << "Components test passed" << std::endl;

    // тест двудольности: цикл из 4 вершин - двудольный, из 3 - нет
    Graph g_cycle4; 
    for(int i=0; i<4; ++i) g_cycle4.addVertex();
    g_cycle4.addEdge(0,1); g_cycle4.addEdge(1,2); g_cycle4.addEdge(2,3); g_cycle4.addEdge(3,0);
    assert(GraphMetrics::isBipartite(g_cycle4) == true);

    Graph g_cycle3;
    for(int i=0; i<3; ++i) g_cycle3.addVertex();
    g_cycle3.addEdge(0,1); g_cycle3.addEdge(1,2); g_cycle3.addEdge(2,0);
    assert(GraphMetrics::isBipartite(g_cycle3) == false);
    
    std::cout << "Bipartite test passed" << std::endl;

    // тест диаметра: путь из 5 вершин имеет диаметр 4
    Graph g_path_diam;  
    for(int i=0; i<5; ++i) g_path_diam.addVertex();
    for(int i=0; i<4; ++i) g_path_diam.addEdge(i, i+1);
    
    assert(GraphMetrics::diameter(g_path_diam) == 4);
    std::cout << "Diameter test passed" << std::endl;

    // тест диаметра: полный граф имеет диаметр 1
    assert(GraphMetrics::diameter(g_complete) == 1);
    std::cout << "Diameter complete graph test passed" << std::endl;

    // тест транзитивности: полный граф K3 имеет транзитивность 1.0
    assert(std::abs(GraphMetrics::transitivity(g_complete) - 1.0) < 0.001);
    std::cout << "Transitivity test passed" << std::endl;

    // тест транзитивности: путь имеет транзитивность 0 (нет треугольников)
    assert(GraphMetrics::transitivity(g_path_diam) == 0.0);
    std::cout << "Transitivity path test passed" << std::endl;

    // тест точек сочленения: звезда S4 имеет 1 точку сочленения (центр)
    Graph g_star;
    for(int i=0; i<5; ++i) g_star.addVertex();
    for(int i=1; i<5; ++i) g_star.addEdge(0, i);
    
    assert(GraphMetrics::countArticulationPoints(g_star) == 1);
    std::cout << "Articulation Points test passed" << std::endl;

    // тест мостов: путь из 5 вершин имеет 4 моста
    Graph g_path_bridges;  //
    for(int i=0; i<5; ++i) g_path_bridges.addVertex();
    for(int i=0; i<4; ++i) g_path_bridges.addEdge(i, i+1);
    
    assert(GraphMetrics::countBridges(g_path_bridges) == 4);
    std::cout << "Bridges test passed" << std::endl;

    // тест мостов: цикл не имеет мостов
    Graph g_cycle_bridges;  
    for(int i=0; i<5; ++i) g_cycle_bridges.addVertex();
    for(int i=0; i<5; ++i) g_cycle_bridges.addEdge(i, (i+1) % 5);
    
    assert(GraphMetrics::countBridges(g_cycle_bridges) == 0);
    std::cout << "Bridges cycle test passed" << std::endl;

    // тест хроматического числа: двудольный граф имеет χ ≤ 2
    assert(GraphMetrics::chromaticNumberGreedy(g_cycle4) <= 2);
    std::cout << "Chromatic Number bipartite test passed" << std::endl;

    // тест хроматического числа: K3 имеет χ = 3
    assert(GraphMetrics::chromaticNumberGreedy(g_complete) == 3);
    std::cout << "Chromatic Number K3 test passed" << std::endl;
}


void test_add_vertex() {
    Graph g;
    assert(g.verticesCount() == 0);
    g.addVertex();
    assert(g.verticesCount() == 1);
    g.addVertex();
    g.addVertex();
    assert(g.verticesCount() == 3);
    std::cout << "test_add_vertex passed" << std::endl;
}

void test_add_edge() {
    Graph g;
    g.addVertex(); 
    g.addVertex(); 
    g.addVertex(); 
    
    assert(g.edgesCount() == 0);
    g.addEdge(0, 1);
    assert(g.edgesCount() == 1);
    assert(g.hasEdge(0, 1) == true);
    assert(g.hasEdge(1, 0) == true); 
    std::cout << "test_add_edge passed" << std::endl;
}

void test_invariants() {
    Graph g;
    for (int i = 0; i < 5; i++) g.addVertex();
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    
    assert(g.verticesCount() == 5);
    assert(g.edgesCount() == 4); // N-1
    std::cout << "test_invariants passed" << std::endl;
}
void test_remove_vertex() {
    Graph g;
    for(int i = 0; i < 4; ++i) g.addVertex(); 
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
   
    assert(g.edgesCount() == 3);
    
    g.removeVertex(1); 
    
    assert(g.edgesCount() == 1); 
    assert(g.hasEdge(0, 1) == false);
    assert(g.hasEdge(1, 2) == false);
    assert(g.hasEdge(2, 3) == true);
    
    std::cout << "test_remove_vertex passed" << std::endl;
}

int main() {
    test_add_vertex();
    test_add_edge();
    test_invariants();
    test_remove_vertex();
    test_generators();
    test_advanced_generators();  
    test_metrics();
    
    std::cout << "\n All tests passed!" << std::endl;
    return 0;
}
