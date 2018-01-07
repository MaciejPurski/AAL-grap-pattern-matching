#include <iostream>
#include <cstring>
#include <chrono>
#include "digraph.h"

int main(int argc, char **argv)
{
    uint size;
    bool isDisjunctive;
    uint n_edges;

    if (argc == 1 || strcmp(argv[1], "-d"))
        isDisjunctive = false;
    else
        isDisjunctive = true;


    std::cin >> size;
    std::cin >> n_edges;

    Digraph g(size);


    for (int i = 0; i < n_edges; i++) {
        int src, dst;
        std::cin >> src >> dst;
        g.addEdge(src, dst);
    }

    Digraph g2(g);

    std::cin >> size;
    std::cin >> n_edges;

    Digraph p(size);

    for (int i = 0; i < n_edges; i++) {
        int src, dst;
        std::cin >> src >> dst;
        p.addEdge(src, dst);
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Subgraph> result = g.searchPattern(p, isDisjunctive);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "RESULT DISJUNCTIVE: " << isDisjunctive << "Time elapsed: " << elapsed.count()<< std::endl;
    for (const auto &it : result) {
        for (const auto &it2 : it) {
            std::cout << it2 << " ";
        }

        std::cout << std::endl;
    }

    start = std::chrono::high_resolution_clock::now();
    std::vector<Subgraph> result2 = g2.searchPatternUllman(p, isDisjunctive);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;

    std::cout << "RESULT Ullman DISJUNCTIVE: " << isDisjunctive << "Time elapsed: " << elapsed.count() << std::endl;
    for (const auto &it : result2) {
        for (const auto &it2 : it) {
            std::cout << it2 << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}