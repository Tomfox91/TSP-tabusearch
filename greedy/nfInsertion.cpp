#include "nfInsertion.h"
#include <list>
#include <iterator>
#include <algorithm>
#include "nearestNeighbour.h"

using std::list;

template <bool nearest>
void nfInsertionInit(Sequence& sequence, const Params& p) {
	list<int> remaining;
	for (int i = 1; i < p.N; ++i) {
		remaining.push_back(i);
	}
	
	list<int> cycle;
	cycle.push_back(0);
	cycle.push_back(extractBestNeighbour<nearest>(0, remaining, p));
	cycle.push_back(0);

	for (int n = 2; n < p.N; ++n) {
		list<int>::iterator bestNeighbour;
		Cost bestCost = nearest ? Params::costMax : 0;

		for (list<int>::iterator element = cycle.begin();
			element != cycle.end(); ++element) {

			list<int>::iterator neighbour = getBestNeighbour<nearest>(*element, remaining, p);
			Cost cost = p.C[*element][*neighbour];
			if ((nearest && (cost < bestCost)) || (!nearest && (cost > bestCost))) {
				bestCost = cost;
				bestNeighbour = neighbour;
			}
		}

		const int r = *bestNeighbour;
		remaining.erase(bestNeighbour);

		bestCost = Params::costMax;
		list<int>::iterator bestInsertion;
		list<int>::iterator i, j;
		for (i = cycle.begin(), j = std::next(i);
			j != cycle.end(); i = j, ++j) {

			Cost insertionCost = p.C[*i][r] + p.C[r][*j] - p.C[*i][*j];
			if (insertionCost < bestCost) {
				bestCost = insertionCost;
				bestInsertion = j;
			}
		}

		cycle.insert(bestInsertion, r);
	}

	std::copy(cycle.begin(), cycle.end(), std::back_inserter(sequence.sequence));
}

template void nfInsertionInit<true>(Sequence& sequence, const Params& p);
template void nfInsertionInit<false>(Sequence& sequence, const Params& p);