#include <iostream>
#include <cstring>
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

    std::cin >> size;
    std::cin >> n_edges;

    Digraph p(size);

    for (int i = 0; i < n_edges; i++) {
        int src, dst;
        std::cin >> src >> dst;
        p.addEdge(src, dst);
    }

    std::vector<Subgraph> result = g.searchPattern(p, isDisjunctive);


    std::cout << "RESULT DISJUNCTIVE: " << isDisjunctive << std::endl;
    for (const auto &it : result) {
        for (const auto &it2 : it) {
            std::cout << it2 << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}