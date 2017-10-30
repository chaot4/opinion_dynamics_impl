#include "simulation.h"

#include "defs.h"

Simulation::Simulation(Graph const& graph, DynamicsType dynamics_type,
                       Coloring initial_coloring)
	: graph(graph), dynamics(dynamics_type, graph), initial_coloring(initial_coloring),
	current_coloring(graph.getNumberOfNodes()), next_coloring(graph.getNumberOfNodes())
{
	assert(initial_coloring.size() == graph.getNumberOfNodes());

	clear();
}

Result Simulation::run(std::int64_t max_rounds)
{
	clear();
	max_rounds = (max_rounds == -1 ? graph.getNumberOfNodes() : max_rounds);

	// run simulation
	std::size_t round = 0;
	while (round < (std::size_t)max_rounds && !current_coloring.isUnimodal()) {
		dynamics.simulateOneRound(current_coloring, next_coloring);
		current_coloring.swap(next_coloring);
		++round;
	}

	assert(current_coloring.size() > 0);
	return Result{
		graph.getFilename(),
		current_coloring.getWinningColor(),
		current_coloring.getColorFractions(),
		getColorVolumes(),
		round
	};
}

std::vector<float> Simulation::getColorVolumes() const
{

	// count
	std::vector<std::size_t> counts(NUMBER_OF_COLORS);
	for (Graph::NodeID node_id = 0; node_id < graph.getNumberOfNodes(); ++node_id) {
		std::size_t color_index = static_cast<std::size_t>(current_coloring.get(node_id));
		counts[color_index] += graph.degree(node_id);
	}

	// normalize
	std::vector<float> volume(NUMBER_OF_COLORS);
	auto sum = std::accumulate(counts.begin(), counts.end(), 0);
	for (std::size_t i = 0; i < volume.size(); ++i) {
		volume[i] = (float)counts[i]/sum;
	}

	return volume;
}

void Simulation::clear()
{
	current_coloring.assign(initial_coloring);
}
