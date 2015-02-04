#include "sequence.h"
#include <algorithm>
#include <cstdio>

void orderInit(Sequence& seq, const Params& p) {
	seq.sequence.resize(p.N+1);
	for (int i = 0; i < p.N; ++i) {
		seq.sequence[i] = i;
	}
	seq.sequence[p.N] = 0;
}

Sequence::Sequence(void (*initFunction)(Sequence&, const Params&), const Params& p) {
	initFunction(*this, p);

	cost = 0;
	for (unsigned int i = 0; i < sequence.size()-1; ++i) {
		cost += p.C[sequence[i]][sequence[i+1]];
	}
}

void Sequence::apply(const Move move) {
	auto it = sequence.begin();
	std::reverse(it + move.from, it + (move.to +1));
	cost += move.increment;
}


void Sequence::print() {
	printf("[");
	for (int i : sequence) {
		printf("%4d", i);
	}
	printf("] : (%12.5f)\n\n", cost * Params::costUnit);
}