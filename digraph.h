#include <vector>
#include "Node.h"
#include "BitVector.h"

typedef std::vector<uint> Subgraph;
typedef unsigned int uint;

class Digraph
{
private:
	typedef std::vector<BitVector> BitMatrix;
	std::vector<Node> nodesList;

	bool isPermutationIsomorphism(const Digraph &other, const std::vector<uint> &perm) const;
    void
    recurse(const Digraph &pattern, bool isDisjunctive, Subgraph &combination, std::vector<Subgraph> &result, int offset,
            int k);
    void degreesCheck(const Digraph &pattern, BitMatrix &m);
    void prune(const Digraph &pattern, BitMatrix &m);
    bool checkNeighboursMapping(const Digraph &pattern, int x, int y, BitMatrix &m);
public:
	explicit Digraph(uint nVertices) {
        for (uint i = 0; i < nVertices; i++)
            nodesList.emplace_back(Node(i, nVertices));
    }


	void addEdge(uint src, uint dst);
	void removeEdge(uint src, uint dst);
    void addNode();
    void removeNode(uint index);
	uint getOutDegree(uint vertex) const;
	uint getInDegree(uint vertex) const;
	bool isSubgraphIsomorphic(const Digraph &other, Subgraph &pattern) const;
    std::vector<Subgraph> searchPattern(const Digraph &pattern, bool isDisjuntive);
    std::vector<Subgraph> searchPatternUllman(const Digraph &pattern, bool isDisjunctive);
};

