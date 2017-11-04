#pragma once

#include "defs.h"

#include <string>
#include <vector>

//
// DynamicsType
//

enum class DynamicsType {
	VoterModel,
	TwoChoices,
};
DynamicsType toDynamicsType(std::string const& dynamics_type_string);
std::string toString(DynamicsType dynamics_type);

//
// CPMethod
//

enum class CPMethod { // TODO: choose less-dumb names
	Paper,
	EG // Emilio-Giacomo method (named after its famous inventors)
};
CPMethod toCPMethod(std::string const& cp_method_string);
std::string toString(CPMethod cp_method);

//
// ExperimentData
//

struct ExperimentData
{
	std::string graph_file;
	DynamicsType dynamics_type;
	CPMethod cp_method;
	std::int64_t max_rounds;
	float win_threshold;
	std::size_t number_of_exps;

	ExperimentData(std::string graph_file,
	               DynamicsType dynamics_type,
	               CPMethod cp_method,
	               std::int64_t max_rounds,
	               float win_threshold,
	               std::size_t number_of_exps)
		: graph_file(graph_file)
		, dynamics_type(dynamics_type)
		, cp_method(cp_method)
		, max_rounds(max_rounds)
		, win_threshold(win_threshold)
		, number_of_exps(number_of_exps) {}
};
using ExperimentsData = std::vector<ExperimentData>;

//
// Color
//

enum class Color : int_fast8_t {
	Red = 0,
	Blue = 1,
	None = -1
};
std::string toString(Color color);
// FIXME: this is a bit hack-ish
std::size_t const NUMBER_OF_COLORS = 2;

//
// Result
//

struct Result
{
	std::string graph_file;
	Color winning_color;
	std::vector<float> color_fractions;
	std::vector<float> color_volumes;
	std::size_t number_of_rounds;
};
using Results = std::vector<Result>;
