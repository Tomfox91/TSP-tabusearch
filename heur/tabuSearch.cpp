#include "tabuSearch.h"
#include <algorithm>
#include <vector>
#include <climits>
#include <iostream>
#include "util/trimatrix.h"

using std::vector;
using std::cout;
using std::cerr;
using std::endl;

static inline Move _2optTabuBest(
	const Sequence& sequence, const Params& p, const TabuParams& tp, const int iter,
	const TriMatrix<int>& lastUsed,
	const TriMatrix<int>& usedCount,
	const bool usePenalty, const int aspirationIncrement) {

	Move best;
	best.increment = Params::costMax;
	Cost bestEvaluation = Params::costMax;

	for (int a = 1; a < p.N-1; ++a) {
		int h = sequence[a-1];
		int i = sequence[a];

		for (int b = a+1; b < p.N; ++b) {
			int j = sequence[b];
			int l = sequence[b+1];

			Cost increment = -p.C[h][i] -p.C[j][l] +p.C[h][j] +p.C[i][l];
			bool aspiration = increment < aspirationIncrement;
			bool nonTabu = lastUsed(i, j) < iter - tp.tabuLength;

			if (aspiration || nonTabu) {
				Cost evaluation = increment;
				
				if (usePenalty) {
					evaluation += tp.maximumPenalty * 
						(usedCount(h, j) + usedCount(i, l)) /2 /iter;
				}

				if (evaluation < bestEvaluation) {
					bestEvaluation = evaluation;
					best.increment = increment;
					best.from = a;
					best.to = b;
				}
			}
		}
	}

	return best;
}

void tabuSearch(Sequence& sequence, const Params& p, const TabuParams& tp) {
	int bestIter = 0;
	Sequence bestSequence = sequence;
	Cost bestCost = sequence.cost;
	bool lastMoveWasImprovement = true;

	TriMatrix<int> lastUsed(p.N, INT_MIN); // iteration when the move was last used
	TriMatrix<int> usedCount(p.N, 0); // number of iterations the edge was in use
	TriMatrix<int> currentUseStarted(p.N, 1); // iteration when the edge was inserted

	bool diversifaction = false;
	int diversifactionStarted = -1;
	int intensificationStarted = 1;

	for (int iter = 1; iter <= tp.maxIter; ++iter) {

		Move best = _2optTabuBest(
			sequence, p, tp, iter, lastUsed, usedCount,
			/* usePenalty = */ diversifaction,
			/* aspirationIncrement = */ bestCost - sequence.cost);

		if (best.increment > 0) {
			if (lastMoveWasImprovement && sequence.cost < bestSequence.cost) {
				bestSequence = sequence;
			}

			if (best.increment == Params::costMax) {
				cout << "All neighbours are tabu" << endl;
				break;
			}

			lastMoveWasImprovement = false;			

		} else {
			if (sequence.cost + best.increment < bestCost) {
				bestIter = iter;
				bestCost = sequence.cost + best.increment;
				
				if (!diversifaction) 
					intensificationStarted = iter;
			}

			lastMoveWasImprovement = true;
		}

		{
			int h = sequence[best.from-1];
			int i = sequence[best.from];
			int j = sequence[best.to];
			int l = sequence[best.to+1];
			lastUsed(i, j) = iter;
			usedCount(h, i) += iter - currentUseStarted(h, i);
			usedCount(j, l) += iter - currentUseStarted(j, l);
			currentUseStarted(h, i) = -1;
			currentUseStarted(j, l) = -1;
			currentUseStarted(h, j) = iter;
			currentUseStarted(i, l) = iter;
		}
		sequence.apply(best);

		if (!diversifaction && intensificationStarted < iter - tp.maxWorseningMoves) {
			diversifaction = true;
			diversifactionStarted = iter;
		} else if (diversifaction && diversifactionStarted < iter - tp.diversificationMoves) {
			diversifaction = false;
			intensificationStarted = iter;
		}

		#ifndef SILENT_SEARCH
			printf("#%06d[%c]: ", iter, diversifaction ? 'D' : 'I');
			best.print();
			sequence.print();
		#endif
	}

	if (sequence.cost > bestSequence.cost) {
		sequence = bestSequence;
		printf("Best solution (#%06d) ", bestIter);
		sequence.print();
	}

	printf("Cost: %12.5f  ", sequence.cost * Params::costUnit);
}

