#include "localSearch.h"

static Move _2optBest(const Sequence& sequence, const Params& p) {
	Move best;
	best.increment = Params::costMax;

	for (int a = 1; a < p.N-1; ++a) {
		int h = sequence[a-1];
		int i = sequence[a];

		for (int b = a+1; b < p.N; ++b) {
			int j = sequence[b];
			int l = sequence[b+1];

			Cost increment = -p.C[h][i] -p.C[j][l] +p.C[h][j] +p.C[i][l];

			if (increment < best.increment) {
				best.increment = increment;
				best.from = a;
				best.to = b;
			}
		}
	}

	return best;
}

void localSearch(Sequence& sequence, const Params& p) {
	#ifndef SILENT_SEARCH
		int iter = 0;
	#endif

	while (true) {
		Move best = _2optBest(sequence, p);

		if (best.increment >= 0) break;

		sequence.apply(best);

		#ifndef SILENT_SEARCH
			printf("#%04d: ", ++iter);
			best.print();
			sequence.print();
		#endif
	}

	printf("Cost: %12.5f  ", sequence.cost * Params::costUnit);
}

