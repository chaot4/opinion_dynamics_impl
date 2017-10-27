#pragma once

#include "helper_types.h"

#include <string>

class Experiments
{
public:
	Experiments(std::string const& experiments_file, std::string const& result_files_prefix)
		: experiments_file(experiments_file), result_files_prefix(result_files_prefix) {}
	void run();

private:
	std::string const experiments_file;
	std::string const result_files_prefix;

	ExperimentsData readExperiments(std::string const& experiments_file);
	void run(ExperimentData const& experiment_data);
	void writeResultToFile(ExperimentData const& experiment_data,
	                       Result const& result, std::size_t round);
	void writeSummaryToFile(ExperimentData const& experiment_data,
	                        Results const& results);
};
