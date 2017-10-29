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

Result Simulation::run()
{
	clear();

	// run simulation
	std::size_t round = 0;
	while (round < max_rounds && !current_coloring.isUnimodal()) {

		dynamics.simulateOneRound(current_coloring, next_coloring);
		current_coloring.swap(next_coloring);
		++round;
	}

	assert(current_coloring.size() > 0);
	auto winning_color = (round == max_rounds ? Color::None : current_coloring.get(0));
	return Result{graph.getFilename(), winning_color, round};
}

void Simulation::clear()
{
	current_coloring.assign(initial_coloring);
}
