#include <algorithm>
#include <iostream>
#include "digraph.h"

void Digraph::addEdge(int src, int dst)
{
	adjacencyMatrix[src].set(dst);
    nInEdges[dst]++;
    nOutEdges[src]++;
}

bool Digraph::isPermutationIsomorphism(const Digraph &pattern, const BitMatrix &bijection) const
{
    BitMatrix transposed = bijection.transpose();
    BitMatrix temp = bijection * adjacencyMatrix;

    temp = temp * transposed;

	return (pattern.adjacencyMatrix == temp);
}

/* Function used as a first step in the Ullman's algorithm. 
   It updates the mapping matrix, in order to filter out nodes,
   which don't feed.
 */
void Digraph::degreesCheck(const Digraph &pattern, BitMatrix &m)
{

    for (int i = 0; i < pattern.adjacencyMatrix.size(); i++) {
        for (int j = 0; j < adjacencyMatrix.size(); j++) {
            // check out edges
            if (nInEdges[j] >= pattern.nInEdges[i] && nOutEdges[j] >= pattern.nOutEdges[i])
                m[i].set(j);

        }
    }
}

/* Function checks if all of the x's neigbours can be mapped to any of the y's neigbours */
bool Digraph::checkNeighboursMapping(const Digraph &pattern, int x, int y, BitMatrix &m)
{
    const BitVector &neighboursY = adjacencyMatrix[y];
    // for all neighbours of x;
    for (int i = 0; i < pattern.adjacencyMatrix.size(); i++) {
        // there's no edge
        if (!pattern.adjacencyMatrix[x].test(i))
            continue;

        if (!m[i].bitAND(neighboursY))
            return false;
    }

    return true;
}

void Digraph::prune(const Digraph &pattern, BitMatrix &m) {
    bool changed;

    do {
        changed = false;

        // iterate over all 1 bits of the MapMatrix
        for (int i = 0; i < pattern.adjacencyMatrix.size(); i++) {
            for (int j = 0; j < adjacencyMatrix.size(); j++) {
                // we don ’t need to check those
                if (!m[i].test(j)) continue;

                // check if all the neighbours of Pi can
                // be mapped to at least one neigbour of G
                if (!checkNeighboursMapping(pattern, i, j, m)) {
                    m[i].reset(j);
                    changed = true;
                }

            }
        }
    } while (changed);

}

BitVector Digraph::matrixToVector(const BitMatrix &m) const {
    BitVector result(m[0].size());

    for (int i = 0; i < m.size(); i++) {
        result.bitOR(m[i]);
    }

    return result;
}

bool Digraph::setDisjunction(const BitVector &s)
{
    // some nodes are already used
    if (s.bitAND(this->usedNodes))
        return false;

    this->usedNodes.bitOR(s);

    return true;
}

// returns false if there is at least one row with no 1
bool Digraph::validateMapMatrix(const BitMatrix &m) const
{
    for (int i = 0; i < m.size(); i++) {
        if (!m[i].any())
            return false;
    }

    return true;
}


void Digraph::recursiveSearch(int currentRow, BitVector &usedColumns, const Digraph &pattern,
                              bool isOptimized, BitMatrix m, bool isDisjunctive, std::vector<BitVector> &result)
{
    // we finished recursing
    if (currentRow == m.size()) {
        if (isPermutationIsomorphism(pattern, m)) {

            BitVector isomorphism = matrixToVector(m);
            if (!isDisjunctive || setDisjunction(isomorphism))
                result.push_back(isomorphism);
        }

        return;
    }

    if (isOptimized) {
        prune(pattern, m);

        if (!validateMapMatrix(m))
            return;
    }

    for (int i = 0; i < usedColumns.size(); i++) {
        if (usedColumns.test(i))
            continue;
        // set just one bit in the row
        m[currentRow].setUnique(i);
        usedColumns.set(i);
        recursiveSearch(currentRow + 1, usedColumns, pattern, isOptimized, m, isDisjunctive, result);
        usedColumns.reset(i);
    }
}

Subgraph Digraph::bitVectorToSubraph(const BitVector &v) {

    Subgraph result;
    BitVector tmp = v;
    int ff = tmp.findFirstOne();
    while (ff != 0) {
        result.push_back(ff - 1);
        tmp.reset(ff - 1);
        ff = tmp.findFirstOne();
    }

    return result;
}

std::vector<Subgraph> Digraph::searchPattern(const Digraph &pattern, bool isDisjunctive, bool isOptimized)
{
    std::vector<BitVector> result;
    std::vector<Subgraph> ret;
    BitMatrix mapMatrix(pattern.adjacencyMatrix.size(), adjacencyMatrix.size());
    BitVector usedColumns(adjacencyMatrix.size());


    if (isOptimized)
        degreesCheck(pattern, mapMatrix);

    recursiveSearch(0, usedColumns, pattern, isOptimized, mapMatrix, isDisjunctive, result);

    for (int i = 0; i < result.size(); i++) {
        bool equals = false;

        for (int j = i + 1; j < result.size(); j++) {
            if (result[i].equals(result[j])) {
                equals = true;
                break;
            }
        }

        if (!equals)
            ret.emplace_back(bitVectorToSubraph(result[i]));
    }

    return ret;
}





