#include <iostream>
#include <cstring>
#include <chrono>
#include "digraph.h"

void showHelp() {
    std::cout << "Wrong usage. Possible arguments: \n";
    std::cout << "-d\tSet disjunctive mode \n";
    std::cout << "-o\tUse optimized algorithm. By default, does not use the naive algorithm \n";
    std::cout << "-n\tUse naive algorithm. Option used by default\n";
    std::cout << "-t\tShow timing. By default, does not show search results \n";
    std::cout << "-r\tShwo results. Option used by default \n";
}

void showResults(std::vector<Subgraph> result) {
    for (const auto &it : result) {
        for (const auto &it2 : it) {
            std::cout << it2 << " ";
        }

        std::cout << std::endl;
    }
}

int main(int argc, char **argv)
{
    int size;
    bool isDisjunctive = false;
    bool useOptimized = false;
    bool useNaive = true;
    bool showTimings = false;
    bool doShowResults = true;
    int n_edges;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help") || strlen(argv[i]) != 2 || argv[i][0] != '-') {
            showHelp();
            return 0;
        }

        switch (argv[i][1]) {
            case 'd':
                isDisjunctive = true;
                break;
            case 'o':
                useOptimized = true;
                useNaive = false;
                break;
            case 'n':
                useNaive = true;
                break;
            case 't':
                showTimings = true;
                doShowResults = false;
                break;
            case 'r':
                doShowResults = true;
                break;
            default:
                showHelp();
                return 0;

        }

    }


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

    if (useNaive) {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<Subgraph> result = g.searchPattern(p, isDisjunctive, false);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        if (doShowResults)
            showResults(result);

        if (showTimings)
            std::cout << std::fixed << elapsed.count() << std::endl;

        if (useOptimized)
            std::cout << "---------------\n";
    }

    if (useOptimized) {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<Subgraph> result2 = g.searchPattern(p, isDisjunctive, true);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;


        if (doShowResults)
            showResults(result2);

        if (showTimings)
            std::cout << std::fixed << elapsed.count() << std::endl;
    }

    return 0;
}