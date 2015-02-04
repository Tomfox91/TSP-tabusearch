#include "nearestNeighbour.h"

using std::list;

template <bool nearest>
list<int>::iterator getBestNeighbour(
	const int current, list<int>& remaining, const Params& p) {

	list<int>::iterator bestNeighbour;
	Cost bestCost = nearest ? Params::costMax : 0;

	for (list<int>::iterator neighbour = remaining.begin(); 
		neighbour != remaining.end(); ++neighbour) {

		Cost cost = p.C[current][*neighbour];
		if ((nearest && (cost < bestCost)) || (!nearest && (cost > bestCost))) {
			bestCost = cost;
			bestNeighbour = neighbour;
		}		
	}

	return bestNeighbour;
}

template list<int>::iterator getBestNeighbour<true>(
	const int current, list<int>& remaining, const Params& p);
template list<int>::iterator getBestNeighbour<false>(
	const int current, list<int>& remaining, const Params& p);

template <bool nearest>
int extractBestNeighbour(const int current, list<int>& remaining, const Params& p) {
	auto bestNeighbour = getBestNeighbour<nearest>(current, remaining, p);
	int ret = *bestNeighbour;
	remaining.erase(bestNeighbour);
	return ret;
}

template int extractBestNeighbour<true>(const int current, list<int>& remaining, const Params& p);
template int extractBestNeighbour<false>(const int current, list<int>& remaining, const Params& p);

void nearestNeighbourInit(Sequence& sequence, const Params& p) {
	sequence.sequence.resize(p.N+1);

	list<int> remaining;
	for (int i = 1; i < p.N; ++i) {
		remaining.push_back(i);
	}
	
	sequence[0] = 0;

	for (int i = 1; i < p.N; ++i) {
		sequence[i] = extractBestNeighbour<true>(sequence[i-1], remaining, p);
	}

	sequence[p.N] = 0;
}