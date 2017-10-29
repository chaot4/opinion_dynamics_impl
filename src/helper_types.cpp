#include "helper_types.h"

#include <chrono>

// FIXME: this is a bit hack-ish
std::size_t const number_of_colors = 2;

//
// DynamicsType
//

DynamicsType toDynamicsType(std::string const& dynamics_type_string)
{
	if (dynamics_type_string == "VoterModel") {
		return DynamicsType::VoterModel;
	}
	else if (dynamics_type_string == "TwoChoices") {
		return DynamicsType::TwoChoices;
	}

	Error("No matching dynamics type on call of toDynamicsType");
	return DynamicsType::VoterModel; // FIXME: just to avoid warning for now
}

//
// Color
//

std::string toString(Color color)
{
	switch (color) {
	case Color::Red: return "red";
	case Color::Blue: return "blue";
	case Color::None: default: return "none";
	}
}

//
// Coloring
//

Coloring::Coloring(std::size_t size)
	: Coloring(size, Color::Red)
{

}

Coloring::Coloring(std::size_t size, Color color)
	: colors(size, color), color_counts(number_of_colors, 0)
{
	color_counts[static_cast<std::size_t>(color)] = colors.size();
}

Color Coloring::get(std::size_t index) const
{
	return colors[index];
}

void Coloring::set(std::size_t index, Color new_color)
{
	auto old_color = colors[index];
	colors[index] = new_color;

	--color_counts[static_cast<std::size_t>(old_color)];
	++color_counts[static_cast<std::size_t>(new_color)];
}

void Coloring::assign(Coloring const& coloring)
{
	assert(size() == coloring.size());

	auto const& other_colors = coloring.colors;
	auto const& other_counts = coloring.color_counts;

	colors.assign(other_colors.begin(), other_colors.end());
	color_counts.assign(other_counts.begin(), other_counts.end());
}

void Coloring::swap(Coloring& coloring)
{
	colors.swap(coloring.colors);
	color_counts.swap(coloring.color_counts);
}

bool Coloring::isUnimodal() const
{
	bool seen_non_zero_color = false;
	for (auto count: color_counts) {
		if (count > 0) {
			if (seen_non_zero_color) {
				return false;
			}

			seen_non_zero_color = true;
		}
	}

	return true;
}

//
// Random
//

Random::Random()
	: Random(std::chrono::system_clock::now().time_since_epoch().count())
{

}

Random::Random(unsigned int seed)
	: seed(seed)
{
	generator.seed(seed);
}

unsigned int Random::getSeed() const
{
	return seed;
}

std::size_t Random::getSizeT(std::size_t first, std::size_t last)
{
	std::uniform_int_distribution<std::size_t> distribution(first, last);
	return distribution(generator);
}

bool Random::throwCoin()
{
	std::bernoulli_distribution distribution(0.5);
	return distribution(generator);
}
