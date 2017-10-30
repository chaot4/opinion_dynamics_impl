#pragma once

#include "helper_types.h"

#include <string>
#include <vector>

class Graph
{
public:
	// member types
	using NodeID = std::size_t;
	 // TODO: This should best be a 128 bit type.
	using ParserNodeID = long long unsigned int;
	using Neighbors = std::vector<NodeID>;
	class NeighborRange
	{
		using const_iterator = Neighbors::const_iterator;
		const_iterator const _begin;
		const_iterator const _end;

	public:
		NeighborRange(const_iterator begin, const_iterator end)
			: _begin(begin), _end(end) {}

		const_iterator begin() const { return _begin; }
		const_iterator end() const { return _end; }
	};

	// member functions

	Graph() = default;

	void buildFromFile(std::string const& graph_file);

	std::string const& getFilename() const;
	std::size_t getNumberOfNodes() const;
	std::size_t getNumberOfEdges() const;
	std::size_t degree(NodeID node_id) const;
	// Note: This function builds a new vector with the size being the number
	// of nodes. So, beware of calling this too often.
	std::vector<NodeID> getNodesSortedByDegree() const;

	NeighborRange getNeighborRange(NodeID node_id) const;
	NodeID getRandomNeighbor(NodeID node_id, Random& random) const;

private:
	std::string filename;

	// node structures
	std::vector<NodeID> old_ids;

	// edge structures
	std::vector<std::size_t> offsets;
	Neighbors neighbors;

	// helper definitions and functions for buildFromFile
	using Edge = std::pair<ParserNodeID, ParserNodeID>;
	using Edges = std::vector<Edge>;

	Edges readEdges(std::string const& graph_file) const;
	void reduceToLargestScc(Edges& edges) const;
	void convertIDs(Edges& edges);
	void addAllReverseEdges(Edges& edges) const;
	void sortAndMakeUnique(Edges& edges) const;
	void fillOffsetsAndNeighbors(Edges const& edges);
};
