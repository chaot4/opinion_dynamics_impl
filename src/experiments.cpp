#include "experiments.h"

#include "core_periphery.h"
#include "defs.h"
#include "graph.h"
#include "simulation.h"

#include <fstream>
#include <sstream>

void Experiments::run()
{
	Print("Running the experiments.");

	auto experiments_data = readExperiments(experiments_file);
	for (ExperimentID id = 0; id < experiments_data.size(); ++id) {
		run(id, experiments_data[id]);
	}
}

auto Experiments::readExperiments(std::string const& experiments_file) -> ExperimentsData
{
	std::ifstream file(experiments_file);
	if (!file.is_open()) {
		Error("The experiments file couldn't be opened");
	}

	ExperimentsData experiments_data;

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}

		std::stringstream ss(line);
		std::string graph_file, dynamics_type_string, number_of_rounds_string;
		ss >> graph_file >> dynamics_type_string >> number_of_rounds_string;

		experiments_data.emplace_back(graph_file,
		                              toDynamicsType(dynamics_type_string),
		                              std::stoull(number_of_rounds_string));
	}

	return experiments_data;
}

void Experiments::run(ExperimentID id, ExperimentData const& experiment_data)
{
	Graph graph;
	graph.buildFromFile(experiment_data.graph_file);

	auto initial_coloring = calculateCorePeripheryColoring(graph);
	Simulation simulation(graph, experiment_data.dynamics_type, initial_coloring);

	Results results;
	for (std::size_t round = 0; round < experiment_data.number_of_rounds; ++round) {
		results.push_back(simulation.run());
		writeResultToFile(id, experiment_data, results.back(), round);
	}

	writeSummaryToFile(id, experiment_data, results);
}

void Experiments::writeResultToFile(ExperimentID id, ExperimentData const& experiment_data,
                                    Result const& result, std::size_t round)
{
	std::string const exp_filename = result_files_prefix + std::to_string(id);
	std::ofstream file(exp_filename, std::ios_base::app);

	if (!file.is_open()) {
		Error("The experiments file couldn't be opened. Filename: " + exp_filename);
	}

	file << result.graph_file << " "
	     << toString(result.winning_color) << " "
	     << result.number_of_rounds << "\n";
}

void Experiments::writeSummaryToFile(ExperimentID id, ExperimentData const& experiment_data,
                                     Results const& results)
{
	std::string const exp_filename = result_files_prefix + std::to_string(id);
	std::ofstream file(exp_filename, std::ios_base::app);

	file << "Summary: " << "\n"; // TODO: fill in whatever we want here
}
