#pragma once

#include "graph.h"
#include "helper_types.h"

Coloring calculateCorePeripheryColoring(Graph const& graph, CPMethod method = CPMethod::EG);
