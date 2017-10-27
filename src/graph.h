#pragma once

#include "helper_types.h"

#include <string>
#include <vector>

class Graph
{
public:
	using NodeID = std::size_t;

	Graph() = default;

	void buildFromFile(std::string const& graph_file);

	std::size_t getNumberOfNodes() const;
	std::size_t degree(NodeID node_id) const;
	// Note: This function builds a new vector with the size being the number
	// of nodes. So, beware of calling this too often.
	std::vector<NodeID> getNodesSortedByDegree() const;

	NodeID getRandomNeighbor(NodeID node_id, Random& random) const;

private:
	// node structures
	std::vector<NodeID> old_ids;

	// edge structures
	std::vector<std::size_t> offsets;
	std::vector<NodeID> neighbors;

	// helper definitions and functions for buildFromFile
	using Edge = std::pair<NodeID, NodeID>;
	using Edges = std::vector<Edge>;

	Edges readEdges(std::string const& graph_file) const;
	void reduceToLargestScc(Edges& edges) const;
	void convertIDs(Edges& edges);
	void addAllReverseEdges(Edges& edges) const;
	void sortAndMakeUnique(Edges& edges) const;
	void fillOffsetsAndNeighbors(Edges const& edges);
};
