#pragma once

// TODO: rename to "basic_types" or someting
#include "defs.h"

#include <random>
#include <string>
#include <vector>

enum class DynamicsType { VoterModel, TwoChoices };

struct ExperimentData
{
	std::string graph_file;
	DynamicsType dynamics_type;
	std::size_t number_of_rounds;
	// TODO: add seed?
};
using ExperimentsData = std::vector<ExperimentData>;

struct Result
{
	// TODO
};
using Results = std::vector<Result>;

enum class Color : int_fast8_t {
	Red = 0,
	Blue = 1
};

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

private:
	std::vector<Color> colors;
	std::vector<std::size_t> color_counts;
};

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
