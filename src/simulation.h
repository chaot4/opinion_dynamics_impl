#pragma once

#include "dynamics.h"
#include "graph.h"
#include "helper_types.h"

#include <cstdint>

class Simulation
{
public:
	Simulation (Graph const& graph, DynamicsType dynamics_type, Coloring initial_coloring);
	Result run();

private:
	Graph const& graph;
	Dynamics dynamics;
	Coloring const initial_coloring;

	Coloring current_coloring;
	Coloring next_coloring;

	std::size_t const max_rounds = 8000; // TODO: should be parametrizable

	void clear();
};
