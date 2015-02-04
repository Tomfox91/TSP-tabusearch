#pragma once
#include <vector>
#include <iostream>
#include "params.h"

using std::vector;

struct Move {
	int from;
	int to;
	Cost increment;
	void print() {
		printf("{%4d-%4d} ", from, to);
	}
};

class Sequence {
public:
	vector<int> sequence;
	Cost cost;

	Sequence(void (*initFunction)(Sequence&, const Params&), const Params& p);

	void apply(const Move move);

	void print();

	inline int& operator[](const int i)       {return sequence[i];}
	inline int  operator[](const int i) const {return sequence[i];}
};

void orderInit(Sequence&, const Params&); // 0 1 2 ... n-1 0