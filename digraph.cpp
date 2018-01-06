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

	return true;
}

bool Digraph::isSubgraphIsomorphic(const Digraph &other, Subgraph &pattern) const
{
	do {
        if (isPermutationIsomorphism(other, pattern))
			return true;
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
            if (pattern.nodesList[i].getNInEdges() <= nodesList[j].getNInEdges() ||
                pattern.nodesList[i].getNOutEdges() <= nodesList[j].getNOutEdges())
                m[i].set(j);
        }
    }
}

bool Digraph::checkNeighboursMapping(const Digraph &pattern, int x, int y, BitMatrix &m)
{
    BitVector &neighboursY = m[y];
    // for all neighbours of x
    for (int i = 0; i < pattern.nodesList.size(); i++) {
        if (!nodesList[x].testInEdge(i))
            continue;

        if (!m[i].bitAND(neighboursY))
            return false;

        if (!nodesList[x].testOutEdge(i))
            continue;

        if (!m[i].bitAND(neighboursY))
            return false;
    }

    // for each x neighbour
    return true;
}

void Digraph::prune(const Digraph &pattern, BitMatrix &m) {
    bool changed = false;

    do {
        for (int i = 0; i < pattern.nodesList.size(); i++) {
            for (int j = 0; j < nodesList.size(); j++) {
                // we don ’t need to check those
                if (!m[i].test(j)) continue;

                // check if all the neighbours of Pi can
                // be mapped to at least one neigbour of G
                if (checkNeighboursMapping(pattern, i, j, m)) {
                    m[i].reset(j);
                    changed = true;
                }
            }
        }
    } while (changed);
}

std::vector<Subgraph> Digraph::searchPatternUllman(const Digraph &pattern, bool isDisjunctive)
{
    std::vector<Subgraph> result;
    BitMatrix mapMatrix;

    // initialize the matrix and fill it with 0's
    return result;
}





