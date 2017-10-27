#include "experiments.h"

#include "core_periphery.h"
#include "defs.h"
#include "graph.h"
#include "simulation.h"

void Experiments::run()
{
	Print("Running the experiments.");

	auto experiment_data = readExperiments(experiments_file);
	// TODO: openmp?
	for (auto const& experiment: experiment_data) {
		run(experiment);
	}
}

auto Experiments::readExperiments(std::string const& experiments_file) -> ExperimentsData 
{
	return ExperimentsData(); // TODO
}

void Experiments::run(ExperimentData const& experiment_data)
{
	Graph graph;
	graph.buildFromFile(experiment_data.graph_file);

	auto initial_coloring = calculateCorePeripheryColoring(graph);
	Simulation simulation(graph, experiment_data.dynamics_type, initial_coloring);

	Results results;
	for (std::size_t round = 0; round < experiment_data.number_of_rounds; ++round) {
		results.push_back(simulation.run());
		writeResultToFile(experiment_data, results.back(), round);
	}

	writeSummaryToFile(experiment_data, results);
}

void Experiments::writeResultToFile(ExperimentData const& experiment_data,
                                    Result const& result, std::size_t round)
{
	// TODO
}

void Experiments::writeSummaryToFile(ExperimentData const& experiment_data,
                                     Results const& results)
{
	// TODO
}
