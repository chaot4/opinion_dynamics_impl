#include "core_periphery.h"

Coloring calculateCorePeripheryColoring(Graph const& graph)
{
	Coloring coloring(graph.getNumberOfNodes(), Color::Blue);

	std::size_t icc = 0;
	std::size_t ipp = graph.getNumberOfEdges()/2;

	for (auto node_id: graph.getNodesSortedByDegree()) {
		// update I(C,C)
		for (auto node_id: graph.getNeighborRange(node_id)) {
			icc += (coloring.get(node_id) == Color::Red ? 1 : 0);
		}
		// update I(P,P)
		for (auto node_id: graph.getNeighborRange(node_id)) {
			icc -= (coloring.get(node_id) == Color::Blue ? 1 : 0);
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
