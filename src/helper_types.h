#pragma once

// TODO: rename to "basic_types" or someting
#include "defs.h"

#include <random>
#include <string>
#include <vector>

enum class DynamicsType {
	VoterModel,
	TwoChoices,
};
DynamicsType toDynamicsType(std::string const& dynamics_type_string);

struct ExperimentData
{
	std::string graph_file;
	DynamicsType dynamics_type;
	std::int64_t max_rounds;
	std::size_t number_of_exps;

	ExperimentData(std::string graph_file,
	               DynamicsType dynamics_type,
	               std::int64_t max_rounds,
				   std::size_t number_of_exps)
		: graph_file(graph_file)
		, dynamics_type(dynamics_type)
		, max_rounds(max_rounds)
		, number_of_exps(number_of_exps) {}
};
using ExperimentsData = std::vector<ExperimentData>;

enum class Color : int_fast8_t {
	Red = 0,
	Blue = 1,
	None = -1
};
std::string toString(Color color);
// FIXME: this is a bit hack-ish
std::size_t const NUMBER_OF_COLORS = 2;

class Coloring
{
public:
	Coloring(std::size_t size);
	Coloring(std::size_t size, Color color);

	std::size_t size() const { return colors.size(); }

	Color get(std::size_t index) const;
	void set(std::size_t index, Color new_color);
	void assign(Coloring const& coloring);
	void swap(Coloring& coloring);
	bool isUnimodal() const;

	Color getWinningColor() const;
	std::vector<float> getColorFractions() const;
	std::vector<float> getColorVolumes() const;

private:
	std::vector<Color> colors;
	std::vector<std::size_t> color_counts;
};

struct Result
{
	std::string graph_file;
	Color winning_color;
	std::vector<float> color_fractions;
	std::vector<float> color_volumes;
	std::size_t number_of_rounds;
};
using Results = std::vector<Result>;

class Random
{
public:
	Random();
	Random(unsigned int seed);

	unsigned int getSeed() const;

	std::size_t getSizeT(std::size_t first, std::size_t last);
	bool throwCoin();

private:
	unsigned int seed;
	std::default_random_engine generator;
};
