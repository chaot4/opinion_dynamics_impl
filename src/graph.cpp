#include "graph.h"

#include "defs.h"
#include "union_find.h"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>

void Graph::buildFromFile(std::string const& graph_file)
{
	filename = graph_file;

	auto edges = readEdges(graph_file);
	reduceToLargestScc(edges);
	convertIDs(edges);
	addAllReverseEdges(edges);
	sortAndMakeUnique(edges);
	fillOffsetsAndNeighbors(edges);
}

std::string const& Graph::getFilename() const
{
	return filename;
}

auto Graph::readEdges(std::string const& graph_file) const -> Edges
{
	Edges edges;

	std::ifstream file(graph_file);
	if (!file.is_open()) {
		Error("The graph file couldn't be opened");
	}

	std::string line;
	ParserNodeID source, target;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}

		std::stringstream ss(line);
		ss >> source >> target;
		edges.emplace_back(source, target);
	}

	return edges;
}

void Graph::reduceToLargestScc(Edges& edges) const
{
	std::unordered_set<ParserNodeID> nodes;
	for (auto const& edge: edges) {
		nodes.insert(edge.first);
		nodes.insert(edge.second);
	}

	auto largest_partition = UnionFind<ParserNodeID>().run(nodes, edges);

	auto not_in_largest_partition = [&](Edge const& edge) {
		assert(largest_partition.count(edge.first) == largest_partition.count(edge.second));
		return !largest_partition.count(edge.first);
	};
	auto first_to_erase = std::remove_if(edges.begin(), edges.end(), not_in_largest_partition);
	edges.erase(first_to_erase, edges.end());
}

void Graph::convertIDs(Edges& edges)
{
	// First fill to_id map while assigning IDs ...
	std::unordered_map<ParserNodeID, NodeID> to_id;
	NodeID current_id = 0;
	for (auto const& edge: edges) {
		if (!to_id.count(edge.first)) {
			to_id.emplace(edge.first, current_id);
			old_ids.push_back(edge.first);

			++current_id;
		}

		if (!to_id.count(edge.second)) {
			to_id.emplace(edge.second, current_id);
			old_ids.push_back(edge.second);

			++current_id;
		}
	}

	// ... then renew all the values in edges
	for (auto& edge: edges) {
		edge.first = to_id.at(edge.first);
		edge.second = to_id.at(edge.second);
	}
}

void Graph::addAllReverseEdges(Edges& edges) const
{
	// Note: We use this type of loop as we cannot use a range-based loop due to
	// possible iterator invalidation on push.
	auto old_size = edges.size();
	for (std::size_t i = 0; i < old_size; ++i) {
		auto const& edge = edges[i];
		edges.emplace_back(edge.second, edge.first);
	}
}

void Graph::sortAndMakeUnique(Edges& edges) const
{
	// Note: Pairs are sorted lexicographically and thus exactly as we want.
	std::sort(edges.begin(), edges.end());
	auto first_to_erase = std::unique(edges.begin(), edges.end());
	edges.erase(first_to_erase, edges.end());
}

void Graph::fillOffsetsAndNeighbors(Edges const& edges)
{
	NodeID current_source = 0;
	offsets.push_back(current_source);

	std::size_t i;
	for (i = 0; i < edges.size(); ++i) {
		auto const& edge = edges[i];

		// offsets
		while (current_source != edge.first) {
			offsets.push_back(i);
			++current_source;
		}

		// neighbors
		neighbors.push_back(edge.second);
	}
	offsets.push_back(neighbors.size());
}

std::size_t Graph::getNumberOfNodes() const
{
	return old_ids.size();
}

std::size_t Graph::getNumberOfEdges() const
{
	return neighbors.size();
}

std::size_t Graph::degree(NodeID node_id) const
{
	return offsets[node_id + 1] - offsets[node_id];
}

auto Graph::getNodesSortedByDegree() const -> std::vector<NodeID>
{
	std::vector<NodeID> node_ids(getNumberOfNodes());
	std::iota(node_ids.begin(), node_ids.end(), 0);

	auto comp_degree = [&](NodeID node_id1, NodeID node_id2) {
		return degree(node_id1) > degree(node_id2);
	};
	std::sort(node_ids.begin(), node_ids.end(), comp_degree);

	return node_ids;
}

auto Graph::getNeighborRange(NodeID node_id) const -> NeighborRange
{
	auto const begin = neighbors.cbegin() + offsets[node_id];
	auto const end = neighbors.cbegin() + offsets[node_id+1];
	return NeighborRange(begin, end);
}

auto Graph::getRandomNeighbor(NodeID node_id, Random& random) const -> NodeID
{
	assert(degree(node_id) != 0);

	auto neighbor_offset = random.getSizeT(0, degree(node_id) - 1);
	auto neighbor_index = offsets[node_id] + neighbor_offset;

	return neighbors[neighbor_index];
}
