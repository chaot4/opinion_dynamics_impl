#include "core_periphery.h"

#include <unordered_map>
#include <queue>

namespace
{

// FIXME: this is not correct if the graph is directed (which it isn't for now)
Coloring paperMethod(Graph const& graph)
{
	Coloring coloring(graph.getNumberOfNodes(), Color::Blue);

	std::size_t icc = 0;
	std::size_t ipp = graph.getNumberOfEdges()/2;

	for (auto node_id: graph.getNodesSortedByDegree()) {
		for (auto neighbor_id: graph.getNeighborRange(node_id)) {
			if (coloring.get(neighbor_id) == Color::Red) {
				++icc;
			}
			else {
				--ipp;
			}
		}

		// put new node into core
		coloring.set(node_id, Color::Red);

		// stop if influence of core is bigger than influence of periphery 
		if (icc > ipp) {
			break;
		}
	}

	return coloring;
}

Coloring egMethod(Graph const& graph)
{
	using NodeID = Graph::NodeID;

	Coloring coloring(graph.getNumberOfNodes(), Color::Blue);

	auto find_densest_subgraph = [&]() {
		using DegNode = std::pair<std::size_t, NodeID>;
		using PQ = std::priority_queue<DegNode, std::vector<DegNode>, std::greater<DegNode>>;

		std::unordered_map<NodeID, std::size_t> node_deg_map;
		PQ pq;
		std::size_t edge_count = 0;

		for (NodeID node_id = 0; node_id < coloring.size(); ++node_id) {
			if (coloring.get(node_id) == Color::Blue) {
				auto degree = graph.degree(node_id);
				node_deg_map.emplace(node_id, degree);
				pq.emplace(degree, node_id);

				for (auto neighbor_id: graph.getNeighborRange(node_id)) {
					if (coloring.get(neighbor_id) == Color::Blue) {
						++edge_count;
					}
				}
			}
		}
		edge_count /= 2;

		double max_density = 0;
		std::vector<NodeID> max_nodes;

		while (!node_deg_map.empty()) {
			// get node with lowest degree
			auto node_id = pq.top().second;
			pq.pop();
			if (!node_deg_map.count(node_id)) {
				continue;
			}

			// erase from map and adapt degrees
			node_deg_map.erase(node_id);
			for (auto neighbor_id: graph.getNeighborRange(node_id)) {
				auto it = node_deg_map.find(neighbor_id);
				if (it != node_deg_map.end()) {
					--(it->second);
					pq.emplace(it->second, it->first);
					--edge_count;
				}
			}

			// check for new max
			double density = (double)edge_count/node_deg_map.size();
			if (density > max_density) {
				max_density = density;
				// TODO: doing it like that seems a bit too dumb and costly.
				max_nodes.clear();
				for (auto const& node_deg: node_deg_map) {
					max_nodes.push_back(node_deg.first);
				}
			}
		}

		return max_nodes;
	};

	auto add_to_core = [&](std::vector<NodeID> const& nodes, std::size_t& vol_c, std::size_t& vol_p) {
		for (auto node_id: nodes) {
			coloring.set(node_id, Color::Red);

			auto degree = graph.degree(node_id);
			vol_p -= degree;
			vol_c += degree;
		}
	};
	
	std::size_t vol_c = 0;
	std::size_t vol_p = graph.getNumberOfEdges();
	while (vol_c < vol_p) {
		auto nodes = find_densest_subgraph();
		add_to_core(nodes, vol_c, vol_p);
	}

	return coloring;
}

} // end anonymous

Coloring calculateCorePeripheryColoring(Graph const& graph, CPMethod method)
{
	switch (method) {
	case CPMethod::EG:
		return egMethod(graph);
	case CPMethod::Paper: default:
		return paperMethod(graph);
	}
}
