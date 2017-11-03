#pragma once

#include "graph.h"
#include "helper_types.h"

Coloring calculateCorePeripheryColoring(Graph const& graph, CPMethod method);

std::pair<float, float> calcDominanceAndRobustness(Graph const& graph, Coloring const& coloring);
