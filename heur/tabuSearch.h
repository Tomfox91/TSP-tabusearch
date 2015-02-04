#pragma once

#include "sequence.h"
#include "params.h"

struct TabuParams {
	int maxIter;
	int tabuLength;
	int maxWorseningMoves;
	int diversificationMoves;
	Cost maximumPenalty;
	TabuParams(int mi, int tl, int mwm, int dm, int mp) :
		maxIter(mi), tabuLength(tl), 
		maxWorseningMoves(mwm), diversificationMoves(dm),
		maximumPenalty(mp / Params::costUnit) {}
};

void tabuSearch(Sequence& sequence, const Params& p, const TabuParams& tp);


