#include "core_periphery.h"

#include <algorithm>
#include <unordered_map>
#include <queue>

namespace
{

// FIXME: this is not correct if the graph is directed (which it isn't for now)
Coloring calcKRichClub(Graph const& graph)
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

// TODO: clean up this mess!
Coloring calcDensestCore(Graph const& graph)
{
	using NodeID = Graph::NodeID;

	Coloring coloring(graph.getNumberOfNodes(), Color::Blue);

	auto in_place_set_minus = [&](std::vector<NodeID>& vec1, std::vector<NodeID> const& vec2) {
		auto vec2_it = vec2.begin();
		auto pred =  [&](NodeID node_id) -> bool {
			while  (vec2_it != vec2.end() && *vec2_it < node_id) {
				++vec2_it;
			}
			return vec2_it != vec2.end() && *vec2_it == node_id;
		};
		auto first_to_erase = std::remove_if(vec1.begin(), vec1.end(), pred);
		vec1.erase(first_to_erase, vec1.end());
	};

	auto find_densest_subgraph = [&](std::size_t vol_c, std::size_t vol_p) {
		using DegNode = std::pair<std::size_t, NodeID>;
		using PQ = std::priority_queue<DegNode, std::vector<DegNode>, std::greater<DegNode>>;

		std::unordered_map<NodeID, std::size_t> node_deg_map;
		PQ pq;
		std::size_t edge_count = 0;

		double max_density = 0;
		std::vector<NodeID> max_nodes;
		std::vector<NodeID> max_nodes_diff;

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

				max_nodes.push_back(node_id);
				vol_c += degree;
				vol_p -= degree;
			}
		}
		edge_count /= 2;

		while (!node_deg_map.empty()) {
			// get node with lowest degree
			auto node_id = pq.top().second;
			pq.pop();
			if (!node_deg_map.count(node_id)) {
				continue;
			}

			// erase from map and adapt degrees
			node_deg_map.erase(node_id);
			max_nodes_diff.push_back(node_id);
			for (auto neighbor_id: graph.getNeighborRange(node_id)) {
				auto it = node_deg_map.find(neighbor_id);
				if (it != node_deg_map.end()) {
					--(it->second);
					pq.emplace(it->second, it->first);
					--edge_count;
				}
			}
			
			// adapt vol_c, vol_p and check if we passed threshold
			auto degree = graph.degree(node_id);
			vol_c -= degree;
			vol_p += degree;
			if (vol_c > vol_p) { continue; } // TODO: make parametrizable from the exp file

			// check for new max
			double density = (double)edge_count/node_deg_map.size();
			if (density > max_density) {
				max_density = density;
				std::sort(max_nodes_diff.begin(), max_nodes_diff.end());
				in_place_set_minus(max_nodes, max_nodes_diff);
				max_nodes_diff.clear();
			}
		}

		// if max_density is still zero, return an empty vector
		return max_density == 0 ? decltype(max_nodes)() : max_nodes;
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
	bool core_changed = true;
	while (core_changed) {
		auto nodes = find_densest_subgraph(vol_c, vol_p);
		add_to_core(nodes, vol_c, vol_p);
		core_changed = !nodes.empty();
	}

	return coloring;
}

} // end anonymous

Coloring calculateCorePeripheryColoring(Graph const& graph, CPMethod method)
{
	switch (method) {
	case CPMethod::DensestCore:
		return calcDensestCore(graph);
	case CPMethod::KRichClub: default:
		return calcKRichClub(graph);
	}
}

std::pair<float, float> calcDominanceAndRobustness(Graph const& graph, Coloring const& coloring)
{
	using NodeID = Graph::NodeID;

	std::size_t cc_count = 0;
	std::size_t cp_count = 0;
	std::size_t pp_count = 0;

	for (NodeID source_id = 0; source_id < graph.getNumberOfNodes(); ++source_id) {
		auto source_color = coloring.get(source_id);

		for (auto target_id: graph.getNeighborRange(source_id)) {
			auto target_color = coloring.get(target_id);

			// this implicitly assumes that there are only two colors
			if (source_color != target_color) {
				++cp_count;
			}
			else if (source_color == Color::Blue) {
				++pp_count;
			}
			else {
				++cc_count;
			}
		}
	}

	float dominance = (float)cp_count/pp_count;
	float robustness = (float)cc_count/cp_count;
	return {dominance, robustness};
}
