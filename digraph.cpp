#include <algorithm>
#include <iostream>
#include "digraph.h"

void Digraph::addEdge(uint src, uint dst)
{
	nodesList[src].addOutEdge(dst);
    nodesList[dst].addInEdge(src);
}

bool Digraph::isPermutationIsomorphism(const Digraph &other, const std::vector<uint> &perm) const
{
	for (uint i = 0; i < perm.size(); i++) {
        /* left and right bijection operands, which are to be compared */
        const Node &left = other.nodesList[i];
        const Node &right = this->nodesList[perm[i]];

        for (int i = 0; i < perm.size(); i++) {
            if (!left.testInEdge(i))
                continue;

            uint permutedEdge = perm[i];

            if (!right.testInEdge(permutedEdge))
                return false;
        }

        for (int i = 0; i < perm.size(); i++) {
            if (!left.testOutEdge(i))
                continue;

            uint permutedEdge = perm[i];

            if (!right.testOutEdge(permutedEdge))
                return false;
        }
    }

    // disjunction check
    for (uint i : perm) {
        if (nodesList[i].isUsed)
            return false;
    }

	return true;
}

bool Digraph::isSubgraphIsomorphic(const Digraph &other, Subgraph &pattern) const
{
	do {
        if (isPermutationIsomorphism(other, pattern)) {
            return true;
        }
	} while (std::next_permutation(pattern.begin(), pattern.end()));
	
	return false;
}

void Digraph::recurse(const Digraph &pattern, bool isDisjunctive, Subgraph &combination, std::vector<Subgraph> &result,
             int offset, int k)
{
    // if the combination's size already reached pattern's size
    if (k == 0) {

        if (isSubgraphIsomorphic(pattern, combination)) {
            result.emplace_back(combination);

            if (isDisjunctive) {
                for (uint i : combination) {
                    this->nodesList[i].isUsed = true;
                }
            }
        }

        return;
    }

    for (int i = offset; i <= nodesList.size() - k; ++i) {
        // omit the node if it was already matched
        if (nodesList[i].isUsed)
            continue;
        combination.push_back(i);
        recurse(pattern, isDisjunctive, combination, result, i+1, k-1);
        combination.pop_back();
    }
}

std::vector<Subgraph> Digraph::searchPattern(const Digraph &pattern, bool isDisjuntive)
{
    std::vector<Subgraph> result;
    Subgraph combination;

    recurse(pattern, isDisjuntive, combination, result, 0, pattern.nodesList.size());

    return result;
}


void Digraph::degreesCheck(const Digraph &pattern, BitMatrix &m)
{
    for (int i = 0; i < pattern.nodesList.size(); i++) {
        for (int j = 0; j < nodesList.size(); j++) {
            if (pattern.nodesList[i].getNInEdges() <= nodesList[j].getNInEdges() &&
                pattern.nodesList[i].getNOutEdges() <= nodesList[j].getNOutEdges())
                m[i].set(j);
        }
    }
}

void Digraph::show() const
{
        for (int i = 0; i < nodesList.size(); i++) {
            std::cout << i << ": " << "IN: " << nodesList[i].inEdges.get() << " OUT: " << nodesList[i].outEdges.get() << std::endl;
        }
}

bool Digraph::checkNeighboursMapping(const Digraph &pattern, int x, int y, BitMatrix &m)
{
    const BitVector &neighboursInY = nodesList[y].inEdges;
    const BitVector &neighboursOutY = nodesList[y].outEdges;
    // for all neighbours of x;
    for (int i = 0; i < pattern.nodesList.size(); i++) {
        if (!pattern.nodesList[x].testInEdge(i))
            continue;

        if (!m[i].bitAND(neighboursInY))
            return false;
    }

    for (int i = 0; i < pattern.nodesList.size(); i++) {
        if (!pattern.nodesList[x].testOutEdge(i))
            continue;

        if (!m[i].bitAND(neighboursOutY))
            return false;
    }

    // for each x neighbour
    return true;
}

void Digraph::prune(const Digraph &pattern, BitMatrix &m) {
    bool changed;

    do {
        changed = false;

        for (int i = 0; i < pattern.nodesList.size(); i++) {
            for (int j = 0; j < nodesList.size(); j++) {
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

bool Digraph::isMapMatrixIsomorphic(const Digraph &other, const BitMatrix &m) const {
    // TODO: sprawdzanie izomorfizmu na macierzach

    Subgraph sub;

    for (int i = 0; i < m.size(); i++)
        sub.emplace_back(m[i].findFirstOne() - 1);

    return isPermutationIsomorphism(other, sub);
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
    for (int i = 0; i < s.size(); i++) {
        if (s.test(i) && nodesList[i].isUsed)
            return false;
    }

    for (int i = 0; i < s.size(); i++) {
        if (s.test(i))
            nodesList[i].isUsed = true;
    }

    return true;
}

// returns false if there is at least one row with no 1
bool Digraph::validateMapMatrix(const BitMatrix &m) const
{
    for (const auto &it : m) {
        if (!it.any())
            return false;
    }

    return true;
}


void Digraph::recursiveSearch(int currentRow, BitVector &usedColumns, const Digraph &pattern,
                              BitMatrix m, bool isDisjunctive, std::vector<BitVector> &result)
{
    if (currentRow == m.size()) {
        if (isMapMatrixIsomorphic(pattern, m)) {

            BitVector isomorphism = matrixToVector(m);
            if (!isDisjunctive || setDisjunction(isomorphism))
                result.push_back(isomorphism);
        }

        return;
    }

    prune(pattern, m);

    if (!validateMapMatrix(m))
        return;

    for (int i = 0; i < usedColumns.size(); i++) {
        if (usedColumns.test(i))
            continue;
        m[currentRow].setUnique(i);
        usedColumns.set(i);
        recursiveSearch(currentRow + 1, usedColumns, pattern, m, isDisjunctive, result);
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

std::vector<Subgraph> Digraph::searchPatternUllman(const Digraph &pattern, bool isDisjunctive)
{
    std::vector<BitVector> result;
    std::vector<Subgraph> ret;
    BitMatrix mapMatrix;
    BitVector usedColumns(nodesList.size());

    for (int i = 0; i < pattern.nodesList.size(); i++)
        mapMatrix.emplace_back(BitVector(nodesList.size()));

    degreesCheck(pattern, mapMatrix);

    recursiveSearch(0, usedColumns, pattern, mapMatrix, isDisjunctive, result);

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





