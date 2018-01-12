#ifndef AAL_PROJEKT_DIGRAPH_H
#define AAL_PROJEKT_DIGRAPH_H

#include <vector>
#include "BitMatrix.h"

typedef std::vector<uint> Subgraph;

class Digraph
{
private:
	BitMatrix adjacencyMatrix;
    BitVector usedNodes;
    std::vector<int> nInEdges, nOutEdges;


	bool isPermutationIsomorphism(const Digraph &pattern, const BitMatrix &bijection) const;
    BitVector matrixToVector(const BitMatrix &m) const;
    bool validateMapMatrix(const BitMatrix &m) const;
    bool setDisjunction(const BitVector &s);
    void degreesCheck(const Digraph &pattern, BitMatrix &m);
    void prune(const Digraph &pattern, BitMatrix &m);
    void recursiveSearch(int currentRow, BitVector &usedColumns, const Digraph &pattern,
    bool isOptimized, BitMatrix m, bool isDisjunctive, std::vector<BitVector> &result);
    bool checkNeighboursMapping(const Digraph &pattern, int x, int y, BitMatrix &m);
public:
	explicit Digraph(int nVertices) : adjacencyMatrix(nVertices, nVertices), usedNodes(nVertices), nInEdges(nVertices, 0), nOutEdges(nVertices, 0) { }

	void addEdge(int src, int dst);
    std::vector<Subgraph> searchPattern(const Digraph &pattern, bool isDisjunctive, bool isOptimized);
    Subgraph bitVectorToSubraph(const BitVector &v);


    void show() const;
};

#endif