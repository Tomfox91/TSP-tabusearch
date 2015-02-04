#pragma once

#include <list>
#include "sequence.h"
#include "params.h"

template <bool nearest = true>
std::list<int>::iterator getBestNeighbour(
	const int current, std::list<int>& remaining, const Params& p);

template <bool nearest = true>
int extractBestNeighbour(
	const int current, std::list<int>& remaining, const Params& p);

void nearestNeighbourInit(Sequence& sequence, const Params& p);