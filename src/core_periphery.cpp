#include "core_periphery.h"

namespace
{

void updateICC(std::size_t& icc, Graph::NodeID node_id)
{
	// TODO
}

void updateIPP(std::size_t& ipp, Graph::NodeID node_id)
{
	// TODO
}

void updateICP(std::size_t& icp, Graph::NodeID node_id)
{
	// TODO
}

} // end anonymous

Coloring calculateCorePeripheryColoring(Graph const& graph)
{
	Coloring coloring(graph.getNumberOfNodes(), Color::Blue);

	std::size_t icc = 0;
	std::size_t ipp = 0; // FIXME: init correctly
	std::size_t icp = 0;

	for (auto node_id: graph.getNodesSortedByDegree()) {
		// update influences
		updateICC(icc, node_id);
		updateIPP(ipp, node_id);
		updateICP(icp, node_id);

		// put new node into core
		coloring.set(node_id, Color::Red);

		// stop if influence of core is bigger than influence of periphery 
		if (icc > ipp) {
			break;
		}
	}

	return coloring;
}
